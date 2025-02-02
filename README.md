# N-Queens Visualization with Qt

This project provides an interactive visualization of the N-Queens problem solving process using backtracking. It demonstrates how the algorithm works by showing each step of queen placement and backtracking in real-time.

## Features

- Interactive visualization of the N-Queens solving process
- Adjustable board size (4-20 queens)
- Real-time animation with configurable speed
- Color-coded visualization:
    - Green: Successfully placed queens
    - Blue: Currently attempting placement
    - Red: Conflict detection
- Chessboard-style interface with professional design
- Start/Reset functionality
- Dynamic resizing to fit different window sizes

## Requirements

- Qt 6.x or later
- C++17 compatible compiler
- CMake 3.16 or later

## Building the Project

1. Clone the repository:
```bash
git clone https://github.com/imkiptoo/n-queens-visualization.git
cd n-queens-visualization
```

2. Create a build directory:
```bash
mkdir build
cd build
```

3. Configure and build:
```bash
cmake ..
make
```

## Usage

1. Launch the application
2. Enter the desired number of queens (4-20) in the input field
3. Click "Start" to begin the visualization
4. Use the speed slider to adjust the animation speed:
    - Slide left for slower animation
    - Slide right for faster animation
5. Click "Reset" to stop the current solution and clear the board

## How It Works

The visualization demonstrates the backtracking algorithm for solving the N-Queens problem:

1. Queens are placed column by column
2. For each placement attempt:
    - A blue queen indicates the current attempt
    - If the placement is valid, the queen turns green
    - If a conflict is detected, conflicting queens are highlighted in red
3. When no valid placement is found in a column:
    - The algorithm backtracks to the previous column
    - Previous placements are removed
    - New positions are attempted
4. The process continues until either:
    - A valid solution is found
    - All possibilities are exhausted

## Technical Details

- Built using Qt's Graphics View Framework
- Uses QGraphicsScene for rendering
- Implements smooth animations with QTimer
- Handles dynamic resizing while maintaining board proportions
- Efficient state management for backtracking visualization

## Project Structure

```
.
├── CMakeLists.txt
├── src/
│   ├── main.cpp
│   ├── mainwindow.cpp
│   └── mainwindow.h
└── README.md
```

## Contributions

Contributions are welcome! Please feel free to submit a Pull Request.

## License

This project is licensed under the MIT License - see the LICENSE file for details.

## Acknowledgments

- Inspired by the classic N-Queens problem
- Built with Qt's excellent graphics framework
- Thanks to all contributors and testers

## Future Improvements

- Add ability to manually place queens
- Save and load board configurations
- Export solutions to file
- Multiple solution display
- Statistical analysis of solving process# n-queens-visualization
