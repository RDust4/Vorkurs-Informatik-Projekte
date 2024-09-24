#include <stdio.h>
#include <stdlib.h>


typedef enum Result {
    Success,  // = No Win
    Invalid,
    WinForX,
    WinForO,
    Draw,
} Result;


// Function Prototypes
char* create_board();
void free_board(char* board);
void print_board(char* board);
void increment_player(char* player);
Result make_move(int y, int x, char* board, char player);
Result check_for_win(char* board);


int main() {
    char* board = create_board();
    char player = 'X';

    while (1) {
        print_board(board);

        int y, x;
        Result result;
        
        while (1) {
            printf("\nPlayer %c's turn: ", player);
            scanf("%d %d", &y, &x);
            result = make_move(y, x, board, player);

            if (result != Invalid) break;
            printf("Invalid position. Try again!\n");
        };

        if (result == WinForX || result == WinForO) {
            print_board(board);
            printf("Player %c won! Yayy!\n", player);
            break;
        }
        if (result == Draw) {
            print_board(board);
            printf("The game ended in a draw.");
            break;
        }

        increment_player(&player);
    }

    free_board(board);
    return 0;
}


char* create_board() {
    char* board = malloc(9 * sizeof(char));
    for (int i = 0; i < 9; i++) {
        board[i] = ' ';
    }
    return board;
}
void free_board(char* board) {
    free(board);
}


void print_board(char* board) {
    for (int y = 0; y < 3; y++) {
        printf("      |      |      \n");
        for (int x = 0; x < 3; x++) {
            printf("   %c", board[y*3 + x]);
            printf(x == 2 ? "\n" : "  |");
        }
        printf("      |      |      \n");
        if (y != 2) {
            printf("--------------------\n");
        }
    }
}


void increment_player(char* player) {
    // Swaps X and O
    *player = *player ^ ('X' ^ 'O');
}


Result make_move(int y, int x, char* board, char player) {
    if (x < 0 || x >= 3 || y < 0 || y >= 3 || board[y*3 + x] != ' ') {
        return Invalid;
    }
    board[y*3 + x] = player;
    return check_for_win(board);
}

Result check_for_win_line(char* board, int y, int x, int dy, int dx) {
    char player = board[y*3 + x];
    for (int i = 0; i < 2; i++) {
        y += dy;
        x += dx;
        if (player != board[y*3 + x]) {
            return Success;
        }
    }
    switch (player) {
        case 'X': return WinForX;
        case 'O': return WinForO;
    }
    return Success;
}

Result check_for_win(char* board) {
    // Check for draw
    int isOneFree = 0;
    for (int y = 0; y < 3 && !isOneFree; y++) {
        for (int x = 0; x < 3; x++) {
            if (board[y*3 + x] == ' ') {
                isOneFree = 1;
                break;
            }
        }
    }
    if (!isOneFree) {
        return Draw;
    }

    // check for 3 in a row
    int checks[][4] = {
        // vertically
        {0, 0, 1, 0},
        {0, 1, 1, 0},
        {0, 2, 1, 0},

        // horizontally
        {0, 0, 0, 1},
        {1, 0, 0, 1},
        {2, 0, 0, 1},

        // diagonally
        {0, 0, 1, 1},
        {0, 2, 1, -1}
    };
    for (int i = 0; i < 8; i++) {
        Result res = check_for_win_line(board, checks[i][0], checks[i][1], checks[i][2], checks[i][3]);
        if (res != Success) {
            return res;
        }
    }

    // No win
    return Success;
}
