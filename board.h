#include <iostream>
#include <string>
#include <algorithm>
#include <random>
#include <ctime>

using namespace std;

const int SIZE = 9;

struct Move {
    int row;
    int col;
    int number;
};

class SudokuBoard {
private:
    int cells[SIZE][SIZE];

public:
    SudokuBoard() {
        clear();
    }

    void clear() {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                cells[row][col] = 0;
            }
        }
    }

    int get(int row, int col) const {
        return cells[row][col];
    }

    void set(int row, int col, int value) {
        cells[row][col] = value;
    }

    bool isComplete() const {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (cells[row][col] == 0) {
                    return false;
                }
            }
        }

        return true;
    }

    bool isSafe(int row, int col, int num) const {
        for (int c = 0; c < SIZE; c++) {
            if (cells[row][c] == num) return false;
        }

        for (int r = 0; r < SIZE; r++) {
            if (cells[r][col] == num) return false;
        }

        int boxRowStart = row - row % 3;
        int boxColStart = col - col % 3;

        for (int r = boxRowStart; r < boxRowStart + 3; r++) {
            for (int c = boxColStart; c < boxColStart + 3; c++) {
                if (cells[r][c] == num) return false;
            }
        }

        return true;
    }

    void copyFrom(const SudokuBoard& other) {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                cells[row][col] = other.cells[row][col];
            }
        }
    }

    void print(const string& title = "") const {
        if (title != "") {
            cout << "\n" << title << "\n";
        }

        for (int row = 0; row < SIZE; row++) {
            if (row % 3 == 0) {
                cout << "+-------+-------+-------+\n";
            }

            for (int col = 0; col < SIZE; col++) {
                if (col % 3 == 0) cout << "| ";

                if (cells[row][col] == 0) {
                    cout << ". ";
                }
                else {
                    cout << cells[row][col] << ' ';
                }
            }

            cout << "|\n";
        }

        cout << "+-------+-------+-------+\n";
    }
};

class SudokuSolver {
private:
    mt19937 rng;

public:
    SudokuSolver() : rng(static_cast<unsigned int>(time(nullptr))) {}

    bool solve(SudokuBoard& board, bool randomize = false) {
        for (int row = 0; row < SIZE; row++) {
            for (int col = 0; col < SIZE; col++) {
                if (board.get(row, col) == 0) {
                    int nums[9] = { 1,2,3,4,5,6,7,8,9 };

                    if (randomize) {
                        shuffle(nums, nums + 9, rng);
                    }

                    for (int i = 0; i < 9; i++) {
                        int num = nums[i];

                        if (board.isSafe(row, col, num)) {
                            board.set(row, col, num);

                            if (solve(board, randomize)) {
                                return true;
                            }

                            board.set(row, col, 0);
                        }
                    }

                    return false;
                }
            }
        }

        return true;
    }
};

class SudokuGenerator {
private:
    SudokuSolver solver;
    mt19937 rng;

public:
    SudokuGenerator() : rng(static_cast<unsigned int>(time(nullptr))) {}

    SudokuBoard generate(int blanks) {
        SudokuBoard board;
        solver.solve(board, true);

        int cells[81];

        for (int i = 0; i < 81; i++) {
            cells[i] = i;
        }

        shuffle(cells, cells + 81, rng);

        if (blanks < 0) blanks = 0;
        if (blanks > 81) blanks = 81;

        for (int i = 0; i < blanks; i++) {
            int row = cells[i] / 9;
            int col = cells[i] % 9;

            board.set(row, col, 0);
        }

        return board;
    }
};

class SudokuGame {
private:
    SudokuBoard puzzle;
    SudokuBoard original;
    SudokuBoard solution;

public:
    SudokuGame(const SudokuBoard& startingPuzzle) {
        puzzle.copyFrom(startingPuzzle);
        original.copyFrom(startingPuzzle);
        solution.copyFrom(startingPuzzle);

        SudokuSolver solver;
        solver.solve(solution);
    }

    void play() {
        while (!puzzle.isComplete()) {
            puzzle.print("Current board:");

            Move move = getMove();

            if (move.row == 0 && move.col == 0 && move.number == 0) {
                cout << "\nGame ended.\n";
                return;
            }

            if (!isMoveInRange(move)) {
                cout << "Invalid move. Use numbers from 1 to 9.\n";
                continue;
            }

            int row = move.row - 1;
            int col = move.col - 1;

            if (original.get(row, col) != 0) {
                cout << "That cell is part of the original puzzle.\n";
                continue;
            }

            if (solution.get(row, col) != move.number) {
                cout << "That number is not correct.\n";
                continue;
            }

            puzzle.set(row, col, move.number);
        }

        puzzle.print("Completed board:");
        cout << "\nYou solved it!\n";
    }

private:
    Move getMove() const {
        Move move;

        cout << "\nEnter row column number, using 1-9.";
        cout << "\nExample: 3 4 7";
        cout << "\nEnter 0 0 0 to quit.";
        cout << "\nMove: ";

        cin >> move.row >> move.col >> move.number;

        return move;
    }

    bool isMoveInRange(const Move& move) const {
        return move.row >= 1 && move.row <= 9 &&
            move.col >= 1 && move.col <= 9 &&
            move.number >= 1 && move.number <= 9;
    }
};