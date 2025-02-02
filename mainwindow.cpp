#include "mainwindow.h"
#include <QGraphicsRectItem>
#include <QGraphicsEllipseItem>
#include <QThread>
#include <QApplication>
#include <QStyleFactory>


MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent), N(8), delay(500), currentCol(0), solving(false) {
    setFixedSize(650, 700);

    QString style = "Fusion";
    QApplication::setStyle(style);

    // UI Layout
    centralWidget = new QWidget(this);
    layout = new QVBoxLayout(centralWidget);

    // Input Field
    inputField = new QLineEdit(centralWidget);
    inputField->setPlaceholderText("Enter number of queens (4-20)");
    //layout->addWidget(inputField);

    // Button Layout
    buttonLayout = new QHBoxLayout();
    startButton = new QPushButton("Start", centralWidget);
    resetButton = new QPushButton("Reset", centralWidget);
    buttonLayout->addWidget(inputField);
    buttonLayout->addWidget(startButton);
    buttonLayout->addWidget(resetButton);

    // Set equal stretch factors for each widget
    buttonLayout->setStretch(0, 1); // inputField
    buttonLayout->setStretch(1, 1); // startButton
    buttonLayout->setStretch(2, 1); // resetButton


    layout->addLayout(buttonLayout);

    // Speed Control Layout
    QHBoxLayout *speedLayout = new QHBoxLayout();
    speedLabel = new QLabel("Animation Speed:", centralWidget);
    speedSlider = new QSlider(Qt::Horizontal, centralWidget);
    speedSlider->setMinimum(1);
    speedSlider->setMaximum(10);
    speedSlider->setValue(5);  // Default value
    speedLayout->addWidget(speedLabel);
    speedLayout->addWidget(speedSlider);
    layout->addLayout(speedLayout);

    // Connect buttons
    connect(startButton, &QPushButton::clicked, this, &MainWindow::startSolving);
    connect(resetButton, &QPushButton::clicked, this, &MainWindow::resetBoard);
    connect(speedSlider, &QSlider::valueChanged, this, &MainWindow::updateSpeed);

    // Graphics View
    scene = new QGraphicsScene(this);
    view = new QGraphicsView(scene, this);
    view->setHorizontalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setVerticalScrollBarPolicy(Qt::ScrollBarAlwaysOff);
    view->setViewportUpdateMode(QGraphicsView::FullViewportUpdate);
    layout->addWidget(view);
    setCentralWidget(centralWidget);

    // Timer for Animation
    timer = new QTimer(this);
    connect(timer, &QTimer::timeout, this, &MainWindow::nextStep);

    // Initialize boardItems
    boardItems = QVector<QVector<QGraphicsItem *>>();
}

void MainWindow::resetBoard() {
    timer->stop();
    scene->clear();
    board.clear();
    boardItems.clear();
    queenStack = std::stack<State>();
    currentCol = 0;
    solving = false;
    startButton->setEnabled(true);
}

void MainWindow::startSolving() {
    bool ok;
    int inputN = inputField->text().toInt(&ok);
    if (!ok || inputN < 4 || inputN > 20) {
        inputField->setText("Enter a valid N (4-20)");
        return;
    }

    resetBoard();
    N = inputN;
    board = QVector<QVector<int>>(N, QVector<int>(N, 0));
    boardItems = QVector<QVector<QGraphicsItem *>>(N, QVector<QGraphicsItem *>(N, nullptr));

    // Create initial board
    visualizeBoard();
    solving = true;
    startButton->setEnabled(false);
    timer->start(delay);
}

bool MainWindow::isSafe(int row, int col) {
    // Check row on left side
    for (int i = 0; i < col; i++)
        if (board[row][i] == 1) {
            highlightConflicts(row, i);
            return false;
        }

    // Check upper diagonal on left side
    for (int i = row, j = col; i >= 0 && j >= 0; i--, j--)
        if (board[i][j] == 1) {
            highlightConflicts(i, j);
            return false;
        }

    // Check lower diagonal on left side
    for (int i = row, j = col; i < N && j >= 0; i++, j--)
        if (board[i][j] == 1) {
            highlightConflicts(i, j);
            return false;
        }

    return true;
}

void MainWindow::nextStep() {
    if (!solving || currentCol >= N) {
        timer->stop();
        return;
    }

    clearHighlights();
    static int currentRow = 0;

    // Calculate current sizes
    int viewHeight = view->height();
    int viewWidth = view->width();
    int boardSize = qMin(viewWidth, viewHeight);
    int squareSize = boardSize / N;
    int offsetX = (viewWidth - (squareSize * N)) / 2;
    int offsetY = ((viewHeight - (squareSize * N)) / 2);

    bool placed = false;

    if (currentRow < N) {
        placeQueen(currentRow, currentCol, true, squareSize, offsetX, offsetY);
        QApplication::processEvents();
        QThread::msleep(delay/2);

        if (isSafe(currentRow, currentCol)) {
            board[currentRow][currentCol] = 1;
            queenStack.push({currentRow, currentCol, false});
            placeQueen(currentRow, currentCol, false, squareSize, offsetX, offsetY);
            currentCol++;
            currentRow = 0;
            placed = true;
        } else {
            removeQueen(currentRow, currentCol);
            currentRow++;
        }
    }

    if (!placed && currentRow >= N) {
        if (queenStack.empty()) {
            timer->stop();
            solving = false;
            return;
        }

        auto last = queenStack.top();
        queenStack.pop();
        board[last.row][last.col] = 0;
        removeQueen(last.row, last.col);
        currentCol = last.col;
        currentRow = last.row + 1;
    }
}

void MainWindow::placeQueen(int row, int col, bool isAttempt, int squareSize, int offsetX, int offsetY) {
    if (boardItems[row][col]) {
        scene->removeItem(boardItems[row][col]);
        delete boardItems[row][col];
    }

    QGraphicsEllipseItem* queen = createQueenSymbol(row, col, isAttempt, squareSize, offsetX, offsetY);
    boardItems[row][col] = queen;
}

void MainWindow::removeQueen(int row, int col) {
    if (boardItems[row][col]) {
        scene->removeItem(boardItems[row][col]);
        delete boardItems[row][col];
        boardItems[row][col] = nullptr;
    }
}

void MainWindow::highlightConflicts(int row, int col) {
    if (boardItems[row][col]) {
        QGraphicsEllipseItem *queen = qgraphicsitem_cast<QGraphicsEllipseItem *>(boardItems[row][col]);
        if (queen) {
            queen->setBrush(QBrush(CONFLICT_COLOR));
        }
    }
}

void MainWindow::clearHighlights() {
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            if (board[i][j] == 1 && boardItems[i][j]) {
                QGraphicsEllipseItem *queen = qgraphicsitem_cast<QGraphicsEllipseItem *>(boardItems[i][j]);
                if (queen) {
                    queen->setBrush(QBrush(PLACED_COLOR));
                }
            }
        }
    }
}

QGraphicsEllipseItem* MainWindow::createQueenSymbol(int row, int col, bool isAttempt, int squareSize, int offsetX, int offsetY) {
    QGraphicsEllipseItem* queen = scene->addEllipse(
            col * squareSize + offsetX + (squareSize * 0.2),
            row * squareSize + offsetY + (squareSize * 0.2),
            squareSize * 0.6,
            squareSize * 0.6
    );
    queen->setBrush(QBrush(isAttempt ? ATTEMPT_COLOR : PLACED_COLOR));
    queen->setPen(QPen(Qt::black));
    return queen;
}

QGraphicsRectItem* MainWindow::createCell(int row, int col, int squareSize, int offsetX, int offsetY) {
    QGraphicsRectItem* rect = scene->addRect(
            col * squareSize + offsetX,
            row * squareSize + offsetY,
            squareSize,
            squareSize
    );
    rect->setBrush((row + col) % 2 == 0 ? BOARD_LIGHT : BOARD_DARK);
    return rect;
}

void MainWindow::visualizeBoard() {
    scene->clear();

    // Calculate sizes to fit the view perfectly
    int viewHeight = view->height();
    int viewWidth = view->width();
    int boardSize = qMin(viewWidth, viewHeight); // Leave space for controls
    int squareSize = boardSize / N;

    // Recalculate board size based on square size
    boardSize = squareSize * N;

    // Center the board
    int offsetX = (viewWidth - boardSize) / 2;
    int offsetY = ((viewHeight - boardSize) / 2); // Adjust for controls at top

    // Create board cells
    for (int i = 0; i < N; i++) {
        for (int j = 0; j < N; j++) {
            createCell(i, j, squareSize, offsetX, offsetY);
            if (board[i][j] == 1) {
                placeQueen(i, j, false, squareSize, offsetX, offsetY);
            }
        }
    }

    // Set the scene rect to exactly match the view size
    scene->setSceneRect(0, 0, viewWidth, viewHeight);
}

void MainWindow::updateSpeed(int value) {
    // Convert slider value (1-10) to delay (1000ms - 100ms)
    // Higher slider value = lower delay = faster animation
    delay = 1100 - (value * 100);
    if (timer->isActive()) {
        timer->setInterval(delay);
    }
}

MainWindow::~MainWindow() {
    delete scene;
    delete view;
    delete timer;
}