#include "board.h"

int main() {
    SudokuGenerator generator;

    cout << "1. Enter a Sudoku puzzle\n";
    cout << "2. Generate a Sudoku puzzle\n";
    cout << "Choose an option: ";

    int choice;
    cin >> choice;

    if (choice == 1) {
        SudokuBoard board;

        cout << "\nEnter the Sudoku puzzle row by row.\n";
        cout << "Use 0 for blanks.\n";
        cout << "Example row: 530070000\n\n";

        for (int row = 0; row < SIZE; row++) {
            string input;

            while (true) {
                cout << "Row " << row + 1 << ": ";
                cin >> input;

                if (input.length() != 9) {
                    cout << "Each row must contain exactly 9 digits.\n";
                    continue;
                }

                bool valid = true;

                for (int col = 0; col < SIZE; col++) {
                    if (input[col] < '0' || input[col] > '9') {
                        valid = false;
                        break;
                    }

                    board.set(row, col, input[col] - '0');
                }

                if (!valid) {
                    cout << "Only digits 0 through 9 are allowed.\n";
                    continue;
                }

                break;
            }
        }

        SudokuSolver solver;

        if (solver.solve(board)) {
            board.print("Solved Sudoku:");
        }
        else {
            cout << "\nNo solution exists for this puzzle.\n";
        }
    }
    else if (choice == 2) {
        int blanks;

        cout << "\nHow many blanks should the puzzle have? ";
        cin >> blanks;

        SudokuBoard puzzle = generator.generate(blanks);

        SudokuGame game(puzzle);
        game.play();
    }
    else {
        cout << "Invalid choice.\n";
    }

    return 0;
}