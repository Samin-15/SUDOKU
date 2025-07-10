#ifndef SUDOKU_H
#define SUDOKU_H

#include <iostream>
#include <cstring>
#include <cstdlib>
#include <ctime>
#include <fstream>
#include <string>
using namespace std;

// Stores all game data including board, fixed cells, hints, and solution
struct GameData {
    int board[9][9];
    bool fixed[9][9];  
    int hintsUsed;
    int wrongAttempts;
    int solution[9][9];
};

class SudokuGame {
private:
    GameData data;
    bool solveSudoku();                          // Solves the puzzle using backtracking
    bool findEmptyCell(int& row, int& col);      // Finds next empty cell coordinates
    bool isValidPlacement(int row, int col, int num); // Checks if number fits in cell
    void generateSolution();                     // Generates a complete valid solution

public:
    SudokuGame();                                // Initializes new game
    void generateSudoku();                       // Creates new puzzle
    void printBoard() const;                     // Displays current board
    bool isValid(int row, int col, int num) const; // Validates move
    void provideHint();                          // Reveals one correct number
    bool isComplete() const;                     // Checks if board is solved
    void updateBoard(int row, int col, int num); // Updates cell with new number
    void incrementWrongAttempts();               // Increases wrong attempts count
    int getHintsUsed() const;                    // Returns hints used count
    int getWrongAttempts() const;                // Returns wrong attempts count
    void revealSolution();                       // Shows full solution
    bool isFixed(int row, int col) const;        // Checks if cell is editable
    int getSolutionAt(int row, int col) const;   // Gets solution value for cell
    void saveGame(const string& filename) const; // Saves game to file
    void loadGame(const string& filename);       // Loads game from file
};

#endif