#include <iostream>
using namespace std;

int SIZE = 3;  // 3x3 puzzle

// CustomStack class to manage moves using a stack
class CustomStack {
private:
    int data[SIZE * SIZE];  // Array to store stack elements
    int top;  

public:
    CustomStack(){
       top=-1 ;
}  
    
    bool isEmpty() {
        return top == -1;
    }

    void push(int value) {
        if (top < SIZE * SIZE - 1) {
            data[++top] = value;
        }
    }

    
    int pop() {
        if (!isEmpty()) {
            return data[top--];
        }
        return -1;  // Return -1 if stack is empty
    }

    
    int peek() {
        if (!isEmpty()) {
            return data[top];
        }
        return -1;
    }

    // Display the contents of the stack
    void display() {
        for (int i = top; i >= 0; --i) {
            cout << "Tile " << data[i] << " ";
        }
        cout << endl;
    }
};

// SlidingPuzzle class to manage the puzzle and moves
class SlidingPuzzle {
private:
    int board[SIZE][SIZE];  // 2D array for the puzzle board
    CustomStack moveStack;  // Stack to store moves
    int emptyX, emptyY;     // Coordinates of the empty tile (0)

public:
    SlidingPuzzle() {
        initializeBoard();  // Set up the initial board
        shuffleBoard();     // Shuffle the board for gameplay
    }

    // Initialize the board with numbers 1 to 8 and an empty tile (0)
    void initializeBoard() {
        int num = 1;
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (i == SIZE - 1 && j == SIZE - 1) {
                    board[i][j] = 0;  // Set last cell as empty
                    emptyX = i;
                    emptyY = j;
                } else {
                    board[i][j] = num++;
                }
            }
        }
    }

    // Shuffle the board by moving the empty tile
    void shuffleBoard() {
        int shuffleMoves[] = {0, 1, 2, 3};  // Possible directions (0: up, 1: down, 2: left, 3: right)
        for (int i = 0; i < 50; ++i) {
            int dir = shuffleMoves[i % 4];
            switch (dir) {
                case 0: moveTile(emptyX - 1, emptyY); break;  // Up
                case 1: moveTile(emptyX + 1, emptyY); break;  // Down
                case 2: moveTile(emptyX, emptyY - 1); break;  // Left
                case 3: moveTile(emptyX, emptyY + 1); break;  // Right
            }
        }
        cout << "Puzzle shuffled. Try to solve it!" << endl;
        displayBoard();
    }

    // Display the current state of the board
    void displayBoard() {
        cout << "\nPuzzle Board:\n";
        for (int i = 0; i < SIZE; ++i) {
            for (int j = 0; j < SIZE; ++j) {
                if (board[i][j] == 0)
                    cout << "   ";  // Empty space for tile 0
                else
                    cout << board[i][j] << "  ";
            }
            cout << endl;
        }
    }

    // Check if the tile at (x, y) is adjacent to the empty tile
    bool isAdjacent(int x, int y) {
        return (abs(emptyX - x) + abs(emptyY - y) == 1);
    }

    // Move a tile if it is adjacent to the empty space
    void moveTile(int x, int y) {
        if (x < 0 || x >= SIZE || y < 0 || y >= SIZE) {
            cout << "Invalid position!" << endl;
            return;
        }
        if (board[x][y] == 0) {
            cout << "Cannot move an empty tile!" << endl;
            return;
        }
        if (isAdjacent(x, y)) {
            int tileValue = board[x][y];

            // Swap the tile with the empty space
            board[emptyX][emptyY] = tileValue;
            board[x][y] = 0;
            emptyX = x;
            emptyY = y;

            // Track the move in the stack (push or pop for undo)
            if (!moveStack.isEmpty() && moveStack.peek() == tileValue) {
                moveStack.pop();
                cout << "Tile " << tileValue << " popped (undo move)" << endl;
            } else {
                moveStack.push(tileValue);
                cout << "Tile " << tileValue << " moved" << endl;
            }
            displayBoard();
            displayStack();
        } else {
            cout << "Tile " << board[x][y] << " cannot be moved" << endl;
        }
    }

    // Display the move stack
    void displayStack() {
        cout << "Move Stack (most recent on top): ";
        if (moveStack.isEmpty()) {
            cout << "(empty)" << endl;
            return;
        }
        moveStack.display();
    }
};

int main() {
    SlidingPuzzle puzzle;
    int row, col;

    cout << "Welcome to the Sliding Puzzle Solver!" << endl;
    cout << "Enter row and column of the tile to move (0-based index):" << endl;

    // Game loop to keep moving tiles until the user exits
    while (true) {
        cout << "\nMove tile at position (row col): ";
        cin >> row >> col;

        // Exit condition if user enters -1 -1
        if (row == -1 || col == -1) {
            cout << "Exiting the game." << endl;
            break;
        }

        // Move the specified tile
        puzzle.moveTile(row, col);
    }

    return 0;
}
