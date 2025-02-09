#include <iostream>
#include <vector>
#include <conio.h>
#include <windows.h>
#include <cstdlib>
#include <ctime>

using namespace std;

const int GRID_SIZE = 20;
const int SNAKE_INITIAL_LENGTH = 3;

enum Direction { UP, DOWN, LEFT, RIGHT };
enum Difficulty { EASY, MODERATE, HARD, EXTREME };

vector<pair<int, int>> snake;
pair<int, int> food;
Direction direction = RIGHT;
int score = 0;
bool gameOver = false;
int speed = 200;

void setCursorPosition(int x, int y) {
    COORD coord;
    coord.X = x;
    coord.Y = y;
    SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), coord);
}

void hideCursor() {
    CONSOLE_CURSOR_INFO cursorInfo;
    cursorInfo.dwSize = 10;
    cursorInfo.bVisible = FALSE;
    SetConsoleCursorInfo(GetStdHandle(STD_OUTPUT_HANDLE), &cursorInfo);
}

void showWelcomeScreen() {
    system("cls");
    cout << "========================\n";
    cout << "    WELCOME TO SNAKE    \n";
    cout << "========================\n";
    cout << "   Use W A S D to move  \n";
    cout << "   Press ENTER to start \n";
    cout << "========================\n";

    while (true) {
        if (_kbhit() && _getch() == '\r') {
            break;
        }
    }
}

void chooseDifficulty() {
    int choice;
    system("cls");
    cout << "Choose Difficulty Level:\n";
    cout << "1. Easy     (Speed: 200ms)\n";
    cout << "2. Moderate (Speed: 120ms)\n";
    cout << "3. Hard     (Speed: 80ms)\n";
    cout << "4. Extreme  (Speed: 50ms)\n";
    cout << "Enter your choice (1-4): ";
    cin >> choice;

    switch (choice) {
        case 1: speed = 200; break;
        case 2: speed = 120; break;
        case 3: speed = 80; break;
        case 4: speed = 50; break;
        default:
            cout << "Invalid choice! Defaulting to Moderate (120ms).\n";
            speed = 120;
    }
}

void initializeGame() {
    snake.clear();
    for (int i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
        snake.push_back({GRID_SIZE / 2, GRID_SIZE / 2 - i});
    }

    srand(time(0));
    food.first = rand() % GRID_SIZE;
    food.second = rand() % GRID_SIZE;
    score = 0;
    gameOver = false;
    direction = RIGHT;
}

void displayGrid() {
    setCursorPosition(0, 0);

    for (int x = 0; x < GRID_SIZE + 2; x++) {
        cout << "_ ";
    }
    cout << endl;

    for (int y = 0; y < GRID_SIZE; y++) {
        cout << "| ";
        for (int x = 0; x < GRID_SIZE; x++) {
            bool isSnakeSegment = false;
            for (auto segment : snake) {
                if (segment.first == x && segment.second == y) {
                    isSnakeSegment = true;
                    break;
                }
            }
            if (isSnakeSegment) {
                cout << "O ";
            } else if (x == food.first && y == food.second) {
                cout << "F ";
            } else {
                cout << "  ";
            }
        }
        cout << "|" << endl;
    }

    for (int x = 0; x < GRID_SIZE + 2; x++) {
        cout << "_ ";
    }
    cout << endl;

    cout << "Score: " << score << endl;
}

void updateGame() {
    pair<int, int> newHead = snake[0];
    switch (direction) {
        case UP: newHead.second--; break;
        case DOWN: newHead.second++; break;
        case LEFT: newHead.first--; break;
        case RIGHT: newHead.first++; break;
    }

    if (newHead.first < 0 || newHead.first >= GRID_SIZE || newHead.second < 0 || newHead.second >= GRID_SIZE) {
        gameOver = true;
        return;
    }
    for (auto segment : snake) {
        if (newHead == segment) {
            gameOver = true;
            return;
        }
    }

    if (newHead == food) {
        score++;
        snake.insert(snake.begin(), newHead);
        food.first = rand() % GRID_SIZE;
        food.second = rand() % GRID_SIZE;
    } else {
        snake.insert(snake.begin(), newHead);
        snake.pop_back();
    }
}

void handleInput() {
    if (_kbhit()) {
        char key = _getch();
        switch (key) {
            case 'w': if (direction != DOWN) direction = UP; break;
            case 's': if (direction != UP) direction = DOWN; break;
            case 'a': if (direction != RIGHT) direction = LEFT; break;
            case 'd': if (direction != LEFT) direction = RIGHT; break;
            case 'x': exit(0);
        }
    }
}

void showGameOverScreen() {
    cout << "\nGame Over! Your final score: " << score << endl;
    cout << "Press 'R' to restart\n";
    cout << "Press 'L' to reselect level\n";
    cout << "Press 'X' to exit\n";

    while (true) {
        if (_kbhit()) {
            char key = _getch();
            if (key == 'r' || key == 'R') {
                initializeGame();
                return;
            } else if (key == 'l' || key == 'L') {
                chooseDifficulty();
                initializeGame();
                return;
            } else if (key == 'x' || key == 'X') {
                exit(0);
            }
        }
    }
}

int main() {
    system("mode con cols=100 lines=40");
    hideCursor();

    showWelcomeScreen();
    chooseDifficulty();

    while (true) {
        initializeGame();
        while (!gameOver) {
            displayGrid();
            handleInput();
            updateGame();
            Sleep(speed);
        }
        showGameOverScreen();
    }

    return 0;
}
