#include <conio.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// Constants
#define WIDTH 40
#define HEIGHT 20
#define PACMAN 'C'
#define WALL '#'
#define FOOD '.'
#define EMPTY ' '
#define DEMON 'X'

// Global Variables
int res = 0;
int score = 0;
int pacman_x, pacman_y;
char board[HEIGHT][WIDTH];
int food = 0;
int curr = 0;
int high_score = 0;

// Function to load the high score from a file
void load_high_score() {
    FILE *file = fopen("highscore.txt", "r");
    if (file != NULL) {
        fscanf(file, "%d", &high_score);
        fclose(file);
    }
}

// Function to save the high score to a file
void save_high_score() {
    FILE *file = fopen("highscore.txt", "w");
    if (file != NULL) {
        fprintf(file, "%d", high_score);
        fclose(file);
    }
}

// Function to log the game result to a file
void log_game_result(const char *status) {
    FILE *file = fopen("gamelog.txt", "a");
    if (file != NULL) {
        fprintf(file, "Game Result: %s | Score: %d\n", status, score);
        fclose(file);
    }
}

// Initialize the game
void initialize() {
    // Setup walls, food, demons, and Pac-Man position
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (i == 0 || j == 0 || i == HEIGHT - 1 || j == WIDTH - 1) {
                board[i][j] = WALL;
            } else {
                board[i][j] = EMPTY;
            }
        }
    }

    // Random walls inside the game
    int count = 50;
    while (count--) {
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        if (board[i][j] == EMPTY) {
            board[i][j] = WALL;
        }
    }

    // Place demons
    count = 10;
    while (count--) {
        int i = rand() % HEIGHT;
        int j = rand() % WIDTH;
        if (board[i][j] == EMPTY) {
            board[i][j] = DEMON;
        }
    }

    // Place food
    food = 0;
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            if (board[i][j] == EMPTY && i % 2 == 0 && j % 2 == 0) {
                board[i][j] = FOOD;
                food++;
            }
        }
    }

    // Initialize Pac-Man
    pacman_x = WIDTH / 2;
    pacman_y = HEIGHT / 2;
    board[pacman_y][pacman_x] = PACMAN;
}

// Draw the game board
void draw() {
    system("cls");
    for (int i = 0; i < HEIGHT; i++) {
        for (int j = 0; j < WIDTH; j++) {
            printf("%c", board[i][j]);
        }
        printf("\n");
    }
    printf("Score: %d\n", score);
    printf("High Score: %d\n", high_score);
}

// Handle Pac-Man movement
void move(int move_x, int move_y) {
    int x = pacman_x + move_x;
    int y = pacman_y + move_y;

    if (board[y][x] != WALL) {
        if (board[y][x] == FOOD) {
            score++;
            food--;
            curr++;
            if (food == 0) {
                res = 2; // Win condition
                return;
            }
        } else if (board[y][x] == DEMON) {
            res = 1; // Loss condition
        }

        board[pacman_y][pacman_x] = EMPTY;
        pacman_x = x;
        pacman_y = y;
        board[pacman_y][pacman_x] = PACMAN;
    }
}

// Main Function
int main() {
    // Load the high score from the file
    load_high_score();

    initialize();
    char ch;

    // Instructions
    printf(" Use buttons: w(up), a(left), s(down), d(right)\n");
    printf(" Press q to quit.\n");
    printf("Press Y to start the game: ");
    ch = getch();
    if (ch != 'Y' && ch != 'y') {
        printf("Game exited.\n");
        return 0;
    }

    // Game loop
    while (1) {
        draw();
        printf("Food remaining: %d\n", food);
        printf("Food eaten: %d\n", curr);

        // Check game result
        if (res == 1) {
            system("cls");
            printf("Game Over! Killed by a Demon.\nYour Score: %d\n", score);

            // Log result and check high score
            log_game_result("Lost");
            if (score > high_score) {
                high_score = score;
                save_high_score();
                printf("New High Score!\n");
            }
            return 0;
        } else if (res == 2) {
            system("cls");
            printf("You Win!\nYour Score: %d\n", score);

            // Log result and check high score
            log_game_result("Won");
            if (score > high_score) {
                high_score = score;
                save_high_score();
                printf("New High Score!\n");
            }
            return 0;
        }

        // Take input and move Pac-Man
        ch = getch();
        switch (ch) {
        case 'w': move(0, -1); break;
        case 's': move(0, 1); break;
        case 'a': move(-1, 0); break;
        case 'd': move(1, 0); break;
        case 'q':
            printf("Game Over! Your Score: %d\n", score);
            log_game_result("Quit");
            return 0;
        }
    }

    return 0;
}
