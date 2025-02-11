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

class Snake {
public:
    vector<pair<int, int>> body;
    Direction direction;

    Snake() {
        for (int i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
            body.push_back({GRID_SIZE / 2, GRID_SIZE / 2 - i});
        }
        direction = RIGHT;
    }

    void move() {
        pair<int, int> newHead = body[0];

        switch (direction) {
            case UP: newHead.second--; break;
            case DOWN: newHead.second++; break;
            case LEFT: newHead.first--; break;
            case RIGHT: newHead.first++; break;
        }

        body.insert(body.begin(), newHead);
        body.pop_back(); 
    }

    void grow() {
        body.push_back(body.back()); 
    }

    bool hasCollided() {
        pair<int, int> head = body[0];

        if (head.first < 0 || head.first >= GRID_SIZE || head.second < 0 || head.second >= GRID_SIZE) {
            return true; // H
        }

        for (size_t i = 1; i < body.size(); i++) {
            if (body[i] == head) return true; // Hits itself
        }

        return false;
    }
};

class Food {
public:
    pair<int, int> position;

    Food() {
        spawn();
    }

    void spawn() {
        position.first = rand() % GRID_SIZE;
        position.second = rand() % GRID_SIZE;
    }
};

class Game {
private:
    Snake snake;
    Food food;
    bool gameOver;
    int score;
    int speed;
    int highscore;

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
        cout << "===========================\n";
        cout << "    WELCOME TO SNAKE GAME  \n";
        cout << "===========================\n";
        cout << "   Use W A S D to move  \n";
        cout << "   Press ENTER to start \n";
        cout << "===========================\n";

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
        cout << "2. Moderate (Speed: 150ms)\n";
        cout << "3. Hard     (Speed: 100ms)\n";
        cout << "4. Extreme  (Speed: 50ms)\n";
        cout << "Enter your choice (1-4): ";
        cin >> choice;

        switch (choice) {
            case 1: speed = 200; break;
            case 2: speed = 150; break;
            case 3: speed = 100; break;
            case 4: speed = 50; break;
            default:
                cout << "Invalid choice! Defaulting to Moderate (120ms).\n";
                speed = 150;
        }
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
                for (auto segment : snake.body) {
                    if (segment.first == x && segment.second == y) {
                        isSnakeSegment = true;
                        break;
                    }
                }
                if (isSnakeSegment) {
                    cout << "O ";
                } else if (x == food.position.first && y == food.position.second) {
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

        cout << "Score: " << score <<"highscore: "<< highscore<< endl;
    }

    void handleInput() {
        if (_kbhit()) {
            char key = _getch();
            switch (key) {
                case 'w': if (snake.direction != DOWN) snake.direction = UP; break;
                case 's': if (snake.direction != UP) snake.direction = DOWN; break;
                case 'a': if (snake.direction != RIGHT) snake.direction = LEFT; break;
                case 'd': if (snake.direction != LEFT) snake.direction = RIGHT; break;
                case 'x': exit(0);
            }
        }
    }

    void updateGame() {
        snake.move();
       
        if (score > highScore) {
            highScore = score;
        }
        if (snake.hasCollided()) {
            gameOver = true;
            return;
        }

        if (snake.body[0] == food.position) {
            score++;
            snake.grow();
            food.spawn();
        }
    }

    void showGameOverScreen() {
        cout << "\nGame Over! Your final score: " << score <<"highscore: "<< highscore<< endl;
        cout << "Press 'R' to restart\n";
        cout << "Press 'L' to reselect level\n";
        cout << "Press 'X' to exit\n";

        while (true) {
            if (_kbhit()) {
                char key = _getch();
                if (key == 'r' || key == 'R') {
                    restart();
                    return;
                } else if (key == 'l' || key == 'L') {
                    chooseDifficulty();
                    restart();
                    return;
                } else if (key == 'x' || key == 'X') {
                    exit(0);
                }
            }
        }
    }

public:
    Game() : score(0), gameOver(false), speed(120), highscore(0) {}

    void restart() {
        snake = Snake();
        food.spawn();
        score = 0;
        gameOver = false;
    }

    void run() {
        system("mode con cols=100 lines=40");
        hideCursor();

        showWelcomeScreen();
        chooseDifficulty();
        restart();

        while (true) {
            while (!gameOver) {
                displayGrid();
                handleInput();
                updateGame();
                Sleep(speed);
            }
            showGameOverScreen();
        }
    }
};

int main() {
    srand(time(0));
    Game game;
    game.run();
    return 0;
}
