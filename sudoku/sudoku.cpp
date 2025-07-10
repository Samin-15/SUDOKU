#include "sudoku.h"
using namespace std;

#define FG_GREEN   "\033[32m"
#define FG_BLUE    "\033[34m"
#define FG_MAGENTA "\033[35m"
#define FG_CYAN    "\033[36m"
#define BOLD       "\033[1m"
#define RESET      "\033[0m"

/**
 * SudokuGame constructor - Initializes a new Sudoku game
 * Clears all board data, resets game state, and generates a new puzzle
 */
SudokuGame::SudokuGame() 
{
    memset(data.board, 0, sizeof(data.board));       // Clear player board
    memset(data.fixed, false, sizeof(data.fixed));   // Reset fixed cell flags
    memset(data.solution, 0, sizeof(data.solution)); // Clear solution board
    data.hintsUsed = 0;                             // Reset hint counter
    data.wrongAttempts = 0;                         // Reset wrong attempts
    generateSudoku();                               // Generate new puzzle
}

/**
 * Finds the next empty cell in the solution board
 * @param row [out] Reference to store row index of empty cell
 * @param col [out] Reference to store column index of empty cell
 * @return True if empty cell found, false if board is complete
 */
bool SudokuGame::findEmptyCell(int& row, int& col) 
{
    for (row = 0; row < 9; row++) 
    {
        for (col = 0; col < 9; col++) 
        {
            if (data.solution[row][col] == 0) 
            {
                return true;
            }
        }
    }
    return false;
}

/**
 * Checks if a number can be placed at given position following Sudoku rules
 * @param row Row index (0-8)
 * @param col Column index (0-8)
 * @param num Number to check (1-9)
 * @return True if placement is valid, false otherwise
 */
bool SudokuGame::isValidPlacement(int row, int col, int num) 
{
    // Check row and column for duplicates
    for (int i = 0; i < 9; i++) 
    {
        if (data.solution[row][i] == num || data.solution[i][col] == num) 
        {
            return false;
        }
    }

    // Check 3x3 box for duplicates
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (data.solution[boxRow + i][boxCol + j] == num) 
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Recursive backtracking solver for Sudoku puzzle
 * @return True if solution found, false if unsolvable
 */
bool SudokuGame::solveSudoku() 
{
    int row, col;
    if (!findEmptyCell(row, col)) return true;  // Base case: board complete

    // Try numbers 1-9 in current cell
    for (int num = 1; num <= 9; num++) 
    {
        if (isValidPlacement(row, col, num)) 
        {
            data.solution[row][col] = num;      // Tentative placement
            if (solveSudoku()) return true;     // Recurse with this placement
            data.solution[row][col] = 0;        // Backtrack if solution fails
        }
    }
    return false;  // Trigger backtracking
}

/**
 * Generates a complete Sudoku solution
 */
void SudokuGame::generateSolution() 
{
    solveSudoku();
}

/**
 * Generates a playable Sudoku puzzle by removing numbers from solution
 * Removes 40-54 random numbers to create puzzle of varying difficulty
 */
void SudokuGame::generateSudoku() 
{
    generateSolution();  // First create complete solution

    // Initialize board with complete solution
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            data.board[i][j] = data.solution[i][j];
            data.fixed[i][j] = true;  // Mark all as fixed initially
        }
    }
    
    // Remove random cells to create puzzle
    srand(time(0));
    int cellsToRemove = 40 + rand() % 15;  // Remove 40-54 numbers
    while (cellsToRemove > 0) 
    {
        int row = rand() % 9;
        int col = rand() % 9;
        if (data.board[row][col] != 0) 
        {
            data.board[row][col] = 0;      // Clear cell
            data.fixed[row][col] = false;  // Mark as editable
            cellsToRemove--;
        }
    }
}

/**
 * Prints the current state of the Sudoku board with formatting
 * Shows row/column headers and 3x3 box boundaries
 */
void SudokuGame::printBoard() const 
{
    cout << FG_CYAN << BOLD << "    1 2 3   4 5 6   7 8 9\n" << RESET;
    cout << FG_GREEN << "  +-------+-------+-------+\n" << RESET;
    
    for (int i = 0; i < 9; i++) 
    {
        if (i % 3 == 0 && i != 0) 
        {
            cout << FG_GREEN << "  +-------+-------+-------+\n" << RESET;
        }
        cout << FG_CYAN << BOLD << i+1 << RESET;
        cout << FG_GREEN << " | " << RESET;
        
        for (int j = 0; j < 9; j++) 
        {
            if (j % 3 == 0 && j != 0) 
            {
                cout << FG_GREEN << "| " << RESET;
            }
            cout << (data.board[i][j] == 0 ? "." : to_string(data.board[i][j])) << " ";
        }
        cout << FG_GREEN << "|\n" << RESET;
    }
    cout << FG_GREEN << "  +-------+-------+-------+\n" << RESET;
}

/**
 * Checks if a move is valid according to Sudoku rules
 * @param row Row index (0-8)
 * @param col Column index (0-8)
 * @param num Number to check (1-9)
 * @return True if move is valid, false otherwise
 */
bool SudokuGame::isValid(int row, int col, int num) const 
{
    // Check row and column for duplicates
    for (int i = 0; i < 9; i++) 
    {
        if (data.board[row][i] == num || data.board[i][col] == num) 
        {
            return false;
        }
    }

    // Check 3x3 box for duplicates
    int boxRow = row - row % 3;
    int boxCol = col - col % 3;
    for (int i = 0; i < 3; i++) 
    {
        for (int j = 0; j < 3; j++) 
        {
            if (data.board[boxRow + i][boxCol + j] == num) 
            {
                return false;
            }
        }
    }
    return true;
}

/**
 * Provides a hint by revealing one correct number
 * Finds first empty cell and fills it with solution value
 * Limits to 2 hints per game
 */
void SudokuGame::provideHint() 
{
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++)  
        {
            if (data.board[i][j] == 0 && !data.fixed[i][j])  
            {
                data.board[i][j] = data.solution[i][j];
                data.fixed[i][j] = true;
                data.hintsUsed++;
                cout << FG_CYAN << "\n\n                                                        Hint placed at (" << i+1 << "," << j+1 << ")\n\n" << RESET;
                return;
            }
        }
    }
}

/**
 * Checks if puzzle is completely filled
 * @return True if all cells contain numbers, false otherwise
 */
bool SudokuGame::isComplete() const 
{
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            if (data.board[i][j] == 0) return false;
        }
    }
    return true;
}

/**
 * Updates the game board with player's move
 * Marks cell as fixed if number matches solution
 * @param row Row index (0-8)
 * @param col Column index (0-8)
 * @param num Number to place (1-9)
 */
void SudokuGame::updateBoard(int row, int col, int num) 
{
    data.board[row][col] = num;
    data.fixed[row][col] = true;
}

/**
 * Increments wrong attempt counter
 */
void SudokuGame::incrementWrongAttempts() 
{
    data.wrongAttempts++;
}

/**
 * @return Number of hints used so far
 */
int SudokuGame::getHintsUsed() const 
{
    return data.hintsUsed;
}

/**
 * @return Number of wrong attempts made
 */
int SudokuGame::getWrongAttempts() const 
{
    return data.wrongAttempts;
}

/**
 * Checks if cell is fixed (part of original puzzle or correct guess)
 * @param row Row index (0-8)
 * @param col Column index (0-8)
 * @return True if cell is fixed, false if editable
 */
bool SudokuGame::isFixed(int row, int col) const 
{
    return data.fixed[row][col];
}

/**
 * Gets solution value for specified cell
 * @param row Row index (0-8)
 * @param col Column index (0-8)
 * @return Number in solution at given position
 */
int SudokuGame::getSolutionAt(int row, int col) const 
{
    return data.solution[row][col];
}

/**
 * Displays the complete solution with formatting
 */
void SudokuGame::revealSolution() 
{
    cout << "\nSolution:\n";
    for (int i = 0; i < 9; i++) 
    {
        for (int j = 0; j < 9; j++) 
        {
            cout << data.solution[i][j] << " ";
            if (j % 3 == 2) cout << " ";
        }
        cout << "\n";
        if (i % 3 == 2) cout << "\n";
    }
}

/**
 * Saves current game state to file
 * @param filename Name of file to save to
 */
void SudokuGame::saveGame(const std::string& filename) const 
{
    ofstream file(filename);
    if (file.is_open()) 
    {
        // Save board state
        for (int i = 0; i < 9; i++) 
        {
            for (int j = 0; j < 9; j++) 
            {
                file << data.board[i][j] << " ";
            }
            file << "\n";
        }

        // Save game metadata
        file << data.hintsUsed << "\n";
        file << data.wrongAttempts << "\n";
        file.close();
        std::cout << "Game saved to " << filename << "\n";
    } 
    else 
    {
        std::cout << "Error: Could not save game!\n";
    }
}

/**
 * Loads game state from file
 * @param filename Name of file to load from
 */
void SudokuGame::loadGame(const std::string& filename) 
{
    ifstream file(filename);
    if (file.is_open()) 
    {
        // Load board state
        for (int i = 0; i < 9; i++) 
        {
            for (int j = 0; j < 9; j++) 
            {
                file >> data.board[i][j];
                data.fixed[i][j] = (data.board[i][j] != 0);
            }
        }

        // Load game metadata
        file >> data.hintsUsed;
        file >> data.wrongAttempts;
        file.close();
        std::cout << "Game loaded from " << filename << "\n";
        
        // Regenerate solution for loaded puzzle
        generateSolution();
    } 
    else 
    {
        std::cout << "Error: Could not load game!\n";
        // Reset to new game if load fails
        std::memset(data.board, 0, sizeof(data.board));
        std::memset(data.fixed, false, sizeof(data.fixed));
        data.hintsUsed = 0;
        data.wrongAttempts = 0;
        generateSudoku();
    }
}
