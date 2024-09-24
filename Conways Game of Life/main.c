#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>

#define ROWS 25
#define COLS 25


// Function Prototypes
void set_console_utf8();
int** create_empty_board(int rows, int cols);
void free_board(int** board, int rows);
void clear_screen();
void print_board(int** board);
void tick_board(int** src, int** dest);



int main() {
    set_console_utf8();

    int** board1 = create_empty_board(ROWS, COLS);
    int** board2 = create_empty_board(ROWS, COLS);

    while (1) {
        // Print board
        clear_screen();
        print_board(board1);

        // Handle user input
        while (1) {
            // Read line, break if empty
            char input[100];
            if (fgets(input, sizeof(input), stdin) == NULL || input[0] == '\n') {
                break;
            }

            // Toggle cell
            int row, col;
            sscanf(input, "%d %d", &row, &col);
            if (row >= 0 && row < ROWS && col >= 0 && col < COLS) {
                board1[row][col] = !board1[row][col];
            }

            // Print board
            clear_screen();
            print_board(board1);
        }

        // Update board
        tick_board(board1, board2);
        int** temp = board1;
        board1 = board2;
        board2 = temp;
    }

    free_board(board1, ROWS);
    free_board(board2, ROWS);

    return 0;
}


void set_console_utf8() {
    system("chcp 65001");
}


int** create_empty_board(int rows, int cols) {
    int** board = malloc(rows * sizeof(int*));
    for (int i = 0; i < rows; ++i) {
        board[i] = calloc(cols, sizeof(int));
    }
    return board;
}

void free_board(int** board, int rows) {
    for (int i = 0; i < rows; ++i) {
        free(board[i]);
    }
    free(board);
}


void clear_screen() {
    system("cls");
}


// Prints the board to the console with an ASCII border around it.
// "  " represents a dead cell and "██" represents a live cell.
void print_board(int** board) {
    // Print the top border
    printf("┌");
    for (int col = 0; col < COLS; ++col) {
        printf("──");
    }
    printf("┐\n");


    // Print the board and side borders
    for (int row = 0; row < ROWS; ++row) {
        printf("│");
        for (int col = 0; col < COLS; ++col) {
            if (board[row][col] == 0) {
                printf("  ");  // Dead cell
            } else {
                printf("██");  // Live cell
            }
        }
        printf("│\n");
    }


    // Print the bottom border
    printf("└");
    for (int col = 0; col < COLS; ++col) {
        printf("──");
    }
    printf("┘\n");
}


// Updates the board according to the rules of Conway's Game of Life.
// The updated board is stored in the 'dest' array.
void tick_board(int** src, int** dest) {
    for (int i = 0; i < ROWS; ++i) {
        for (int j = 0; j < COLS; ++j) {
            int neighbors = 0;
            for (int x = -1; x <= 1; ++x) {
                for (int y = -1; y <= 1; ++y) {
                    if (x == 0 && y == 0) {
                        continue;
                    }
                    int ni = i + x;
                    int nj = j + y;
                    if (ni < 0 || ni >= ROWS || nj < 0 || nj >= COLS) {
                        continue;
                    }
                    if (src[ni][nj] == 1) {
                        neighbors++;
                    }
                }
            }
            if (src[i][j] == 1) {
                if (neighbors < 2 || neighbors > 3) {
                    dest[i][j] = 0;
                } else {
                    dest[i][j] = 1;
                }
            } else {
                if (neighbors == 3) {
                    dest[i][j] = 1;
                } else {
                    dest[i][j] = 0;
                }
            }
        }
    }
}
