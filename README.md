# Snake-project

# Snake Game - Terminal-Based C++ Implementation

This is a terminal-based implementation of the classic Snake game, developed in C++ using basic standard libraries. The game supports different difficulty levels and user controls via the keyboard.

## Features
- Classic snake movement using WASD keys
- Dynamic difficulty selection
- Food spawning and score tracking
- Collision detection and game-over handling

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

## Data Structure Analysis
- **Snake Representation:** Implemented as a `vector<pair<int, int>>`, where each pair represents a segment of the snake's body.
- **Grid Representation:** The game board is a conceptual 2D grid, managed implicitly through snake coordinates.
- **Food Spawning:** Randomized using `rand()` within the grid boundaries.
- **Collision Detection:** Checks if the snake collides with walls or itself.

## Code Structure
- `initializeGame()`: Sets up the game environment.
- `displayGrid()`: Renders the game board.
- `updateGame()`: Moves the snake and handles collisions.
- `handleInput()`: Captures user input.


