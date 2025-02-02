#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QGraphicsScene>
#include <QGraphicsView>
#include <QTimer>
#include <QLineEdit>
#include <QPushButton>
#include <QVBoxLayout>
#include <QGraphicsRectItem>
#include <QGraphicsTextItem>
#include <QSlider>
#include <QLabel>
#include <stack>

class MainWindow : public QMainWindow {
Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private slots:
    void startSolving();   // Starts the visualization
    void nextStep();       // Handles step-by-step solving with backtracking
    void resetBoard();     // Resets the board for a new solution
    void updateSpeed(int value); // Updates animation speed

private:
    QGraphicsScene *scene;
    QGraphicsView *view;
    QWidget *centralWidget;
    QVBoxLayout *layout;
    QHBoxLayout *buttonLayout;
    QHBoxLayout *speedLayout;

    QLineEdit *inputField;
    QPushButton *startButton;
    QPushButton *resetButton;
    QSlider *speedSlider;
    QLabel *speedLabel;
    int N;
    QVector<QVector<int>> board;          // Chessboard matrix
    QVector<QVector<QGraphicsItem*>> boardItems;  // Store graphical items
    QTimer *timer;
    int delay;                            // Delay for visualization

    struct State {
        int row, col;
        [[maybe_unused]] bool isAttempt;    // True if this was just an attempt, false if confirmed placement
    };
    std::stack<State> queenStack;         // Stack to track backtracking
    int currentCol;                       // Tracks current column
    bool solving;                         // Indicates if solution in progress

    bool isSafe(int row, int col);
    void visualizeBoard();
    void placeQueen(int row, int col, bool isAttempt = false, int squareSize = 40, int offsetX = 50, int offsetY = 50);
    void removeQueen(int row, int col);
    void highlightConflicts(int row, int col);
    void clearHighlights();

    // Constants for visualization
    const QColor ATTEMPT_COLOR = Qt::blue;
    const QColor PLACED_COLOR = Qt::green;
    const QColor CONFLICT_COLOR = Qt::red;
    const QColor BOARD_LIGHT = QColor(240, 217, 181);
    const QColor BOARD_DARK = QColor(181, 136, 99);

    QGraphicsEllipseItem* createQueenSymbol(int row, int col, bool isAttempt, int squareSize, int offsetX, int offsetY);
    QGraphicsRectItem* createCell(int row, int col, int squareSize, int offsetX, int offsetY);
};

#endif // MAINWINDOW_H