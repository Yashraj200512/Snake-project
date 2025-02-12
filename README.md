# Snake-project

# Snake Game - Terminal-Based C++ Implementation

This is a terminal-based implementation of the classic Snake game, developed in C++ using basic standard libraries. The game supports different difficulty levels and user controls via the keyboard,this game has simple Ui and instructions are provided for every function for user to understand and play the game without any difficulty.

## Features
- Classic snake movement using WASD keys
- Dynamic difficulty selection
- Food spawning and score tracking and highscore tracking
- Collision detection and game-over handling
- Restart game, reselection of level and exit game features

## Installation
To compile and run the game, use the following commands:
```sh
g++ snake_game.cpp -o snake_game.exe
./snake_game.exe
```

## Usage
**Controls:**
- **W** - Move Up
- **A** - Move Left
- **S** - Move Down
- **D** - Move Right
- **X** - Exit Game

## other keys usage
- **R** - Restart
- **L** - reselect level

## Data Structure Analysis
- **Snake Representation:** : vector<pair<int, int>>
The snake’s body is stored as a vector of pair<int, int>, here each pair represents the x, y coordinates of the snake segment.

The head of the snake is always at body[0](at very front) and new positions are inserted at the front and tail is removed to maintain length
vector<pair<int, int>> body;
Operations:

Movement: Insert a new head at the front and remove the tail's last segment.
Growth(food): Add a new segment(o) at the tail without removing any part of snake.

- **Grid Representation:** The game board is a 2D grid, managed  through 2d array.
The grid is 20x20, and any position (x, y) of head of snake outside [0, GRID_SIZE-1] results in a collision.
- **Food Spawning:** Randomized using `rand()` within the grid boundaries and used srand(time(0)) to avoid spawming food at same place
- **Collision Detection:** Checks if the snake collides with walls or itself.

## Code Structure
- 'initializeGame()' → Sets up the game environment.
- 'displayGrid()' → Prints the grid, snake, and food.
- 'handleInput()' → Checks keyboard input (_kbhit() detects if there's any keypress, _getch() reads the char key).
- 'updateGame()' → Moves the snake and handles collisions, growth, and food consumption.
- 'showGameOverScreen()' → Displays the "Game Over" message.
- 'restart()' → Resets the game while only storing highscore/score