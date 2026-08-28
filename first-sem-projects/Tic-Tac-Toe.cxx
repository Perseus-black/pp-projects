#include <iostream>
using namespace std;

const int SIZE = 3;

void displayBoard(char board[SIZE][SIZE]) {
    int indent = 10; // number of spaces to shift right
    string spacer(indent, ' '); // creates a string with 'indent' spaces

    cout << "\n";
    for (int i = 0; i < SIZE; i++) {
        cout << spacer;
        for (int j = 0; j < SIZE; j++) {
            cout << board[i][j];
            if (j < SIZE - 1) cout << " | ";
        }
        cout << "\n";
        if (i < SIZE - 1) {
            cout << spacer << "--+---+--\n";
        }
    }
    cout << "\n";
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
        cout << "Player " << player << ", enter row and column (1-3): ";
        cin >> row >> col;
        row--; col--; // Convert to 0-based index
        if (row >= 0 && row < SIZE && col >= 0 && col < SIZE && board[row][col] == ' ') {
            board[row][col] = player;
            break;
        } else {
            cout << "Invalid move! Try again.\n";
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
            cout << "Player " << currentPlayer << " wins!\n";
            gameOver = true;
        } else if (checkDraw(board)) {
            displayBoard(board);
            cout << "It's a draw!\n";
            gameOver = true;
        } else {
            // Switch player
            currentPlayer = (currentPlayer == 'X') ? 'O' : 'X';
        }
    }

    return 0;
}