#include <iostream>

const int SIZE = 3;

void displayBoard(char board[SIZE][SIZE]) {
    int indent = 10;                          // number of spaces to shift right
    std::string spacer(indent, ' ');          // creates a string with 'indent' spaces

    std::cout << "\n";
    for (int i = 0; i < SIZE; i++) {
        std::cout << spacer;
        for (int j = 0; j < SIZE; j++) {
            std::cout << board[i][j];
            if (j < SIZE - 1) std::cout << " | ";
        }
        std::cout << "\n";
        if (i < SIZE - 1) {
            std::cout << spacer << "--+---+--\n";
        }
    }
    std::cout << "\n";
}

bool checkWin(char board[SIZE][SIZE], char player) {
    // Check rows and columns
    for (int i = 0; i < SIZE; i++) {
        if ((board[i][0] == player && board[i][1] == player && board[i][2] == player) ||
            (board[0][i] == player && board[1][i] == player && board[2][i] == player))
            return true;
    }
    // Check diagonals
    if ((board[0][0] == player && board[1][1] == player && board[2][2] == player) ||
        (board[0][2] == player && board[1][1] == player && board[2][0] == player))
        return true;

    return false;
}

bool checkDraw(char board[SIZE][SIZE]) {
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            if (board[i][j] == ' ')
                return false; // Empty space found, not a draw
    return true;
}

void makeMove(char board[SIZE][SIZE], char player) {
    int row, col;
    while (true) {
        std::cout << "Player " << player << ", enter row and column (1-3): ";
        std::cin >> row >> col;
        row--; col--; // Convert to 0-based index
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
            board[row][col] = player;
            break;
        } else {
            std::cout << "Invalid move! Try again.\n";
        }
    }
}

int main() {
    char board[SIZE][SIZE];
    for (int i = 0; i < SIZE; i++)
        for (int j = 0; j < SIZE; j++)
            board[i][j] = ' '; // Initialize board

    char currentPlayer = 'X';
    bool gameOver = false;

    while (!gameOver) {
        displayBoard(board);
        makeMove(board, currentPlayer);

        if (checkWin(board, currentPlayer)) {
            displayBoard(board);
            std::cout << "Player " << currentPlayer << " wins!\n";
            gameOver = true;
        } else if (checkDraw(board)) {
            displayBoard(board);
            std::cout << "It's a draw!\n";
            gameOver = true;
        } else {
            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    return 0;
}