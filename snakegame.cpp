#include <iostream>
#include <vector>                     //for using vector
#include <conio.h>                    //for grtting input from user's keyboard
#include <windows.h>                  //for sleep() function which determines snake's speed
#include <cstdlib>                    // for using rand function to generate food
#include <ctime>                      //for time(0) used in rand to generate food in different place every time

using namespace std;

const int GRID_SIZE = 20;
const int SNAKE_INITIAL_LENGTH = 3;

enum Direction { UP, DOWN, LEFT, RIGHT };

string Hs(int level) {
    switch (level) {
        case 0: return "Highscore-easy";
        case 1: return "Highscore-moderate";
        case 2: return "Highscore-hard";
        case 3: return "Highscore-extreme";
        default: return "Unknown level";
    }
}

class Snake {
public:
    vector<pair<int, int>> body;
    Direction direction;

    Snake() {
        for (int i = 0; i < SNAKE_INITIAL_LENGTH; i++) {
            body.push_back({GRID_SIZE / 2, GRID_SIZE / 2 - i});  //initial snake length
        }
        direction = RIGHT;
    }

    void move() {
        pair<int, int> newHead = body[0];

        switch (direction) {
            case UP: newHead.second--; break;
            case DOWN: newHead.second++; break;
            case LEFT: newHead.first--; break;      //constantly makes snake moves
            case RIGHT: newHead.first++; break;
        }

        body.insert(body.begin(), newHead);
        body.pop_back();                          
    }

    void grow() {
        body.push_back(body.back()); //when food is eaten
    }

    bool hasCollided() {
        pair<int, int> head = body[0];

        if (head.first < 0 || head.first >= GRID_SIZE || head.second < 0 || head.second >= GRID_SIZE) {  //when it goes outside of greed
            return true;
        }

        for (size_t i = 1; i < body.size(); i++) {
            if (body[i] == head) return true;  //when snake touches itself
        }

        return false;
    }
};

class Food {
public:
    pair<int, int> position;

    Food(Snake& snake) {
        spawn(snake);  // Pass the snake reference to the spawn function
    }

    void spawn(Snake& snake) {  // Accept Snake reference as a parameter
        bool validPosition = false;

        while (!validPosition) {
            position.first = rand() % GRID_SIZE;
            position.second = rand() % GRID_SIZE;

            validPosition = true;
            // Check if food spawns on the snake's body
            for (auto segment : snake.body) {  // Correctly use snake.body
                if (segment.first == position.first && segment.second == position.second) {
                    validPosition = false;
                    break;
                }
            }
        }
    }
};
        

class Game {
private:
    Snake snake;
    Food food;
    bool gameOver;
    int score;
    int speed;
    int highScore[4]; // Store high scores for each level
    int currentLevel;
    char grid[GRID_SIZE + 2][GRID_SIZE + 2];

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
            case 1: 
                speed = 200; 
                currentLevel = 0;
                break;
            case 2: 
                speed = 150; 
                currentLevel = 1;
                break;
            case 3: 
                speed = 100; 
                currentLevel = 2;
                break;
            case 4: 
                speed = 50; 
                currentLevel = 3;
                break;
            default:
                cout << "Invalid choice! Defaulting to Moderate (150ms).\n";
                speed = 150;
                currentLevel = 1; 
        }
    }

    void initializeGrid() {
        for (int y = 0; y < GRID_SIZE + 2; y++) {
            for (int x = 0; x < GRID_SIZE + 2; x++) {
                if (y == 0 || y == GRID_SIZE + 1) grid[y][x] = '#';
                else if (x == 0 || x == GRID_SIZE + 1) grid[y][x] = '|';  
                else grid[y][x] = ' ';
            }
        }

        for (auto segment : snake.body) {
            grid[segment.second + 1][segment.first + 1] = 'O';
        }

        grid[food.position.second + 1][food.position.first + 1] = 'F';
    }

    void displayGrid() {
        setCursorPosition(0, 0);
        initializeGrid(); 

        for (int y = 0; y < GRID_SIZE + 2; y++) {
            for (int x = 0; x < GRID_SIZE + 2; x++) {
                cout << grid[y][x] << " ";
            }
            cout << endl;
        }

        string highscoreString = Hs(currentLevel); 
        cout << "Score: " << score << "  " << highscoreString << ": " << highScore[currentLevel] << endl;
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

    void updateGame() {      //constantly updates game so snake moves and makes snake grow and game over in case if second condition becomes true
        snake.move();

        if (score > highScore[currentLevel]) {
            highScore[currentLevel] = score;
        }

        if (snake.hasCollided()) {
            gameOver = true;
            return;
        }

        if (snake.body[0] == food.position) {
            score++;
            snake.grow();
            food.spawn(snake); // Pass the snake to spawn to avoid food on snake body
        }
    }

    void showGameOverScreen() {
        cout << "\nGame Over! Your final score: " << score << "  High Score: " << highScore[currentLevel] << endl;
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
    Game() : score(0), gameOver(false), speed(150), highScore{0, 0, 0, 0}, currentLevel(1),food(snake) {} //initializing before game starts

    void restart() {
        system("cls"); //clears terminal everytime game restarts
        snake = Snake(); 
        food.spawn(snake); // Pass snake reference here
        score = 0;
        gameOver = false;
    }

    void run() {
        system("mode con cols=100 lines=40");
        hideCursor();

        showWelcomeScreen();
        chooseDifficulty();
        restart();

        while (true) { //runs infinite loops
            while (!gameOver) {
                displayGrid();
                handleInput();
                updateGame();
                Sleep(speed); //makes delaye in ms between these infinite loops
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
