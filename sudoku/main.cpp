#include <iostream>
#include <limits>
#include "sudoku.h"
using namespace std;

#define FG_RED     "\033[31m"
#define FG_GREEN   "\033[32m"
#define FG_BLUE    "\033[34m"
#define FG_YELLOW  "\033[33m"
#define FG_CYAN    "\033[36m"
#define BOLD       "\033[1m"
#define RESET      "\033[0m"

/**
 * Clears the input buffer to handle invalid inputs and prevent infinite loops
 */
void clearInputBuffer() 
{
    cin.clear();  // Clear any error flags
    cin.ignore(numeric_limits<streamsize>::max(), '\n');  // Discard remaining characters
}

/**
 * Handles the quit menu when player requests to exit
 * @return true if player wants to restart, false if wants to quit
 */
bool handleQuitOption() 
{
    char choice;
    while (true) 
    {
        cout << FG_BLUE << BOLD << "\n\n                                                                          Quit Menu:\n" << RESET;
        cout << FG_YELLOW << BOLD << "                                                                      1. Restart Game\n" << RESET;
        cout << FG_YELLOW << BOLD << "                                                                      2. Quit Game\n" << RESET;
        cout << FG_CYAN << BOLD << "                                                                   Enter your choice (1-2): " << RESET;
        cin >> choice;
        clearInputBuffer();

        if (choice == '1') 
        {
            return true;  // Restart game
        } 
        else if (choice == '2') 
        {
            return false; // Quit game
        } 
        else 
        {
            cout << FG_RED << "\n\n                                                             Invalid choice! Please enter 1 or 2.\n" << RESET;
        }
    }
}

/**
 * Main game execution function that controls the Sudoku game flow
 * Handles game initialization, main loop, and post-game options
 */
int main() 
{
    bool playAgain = true;
    
    // Main game loop - continues until player chooses not to play again
    while (playAgain) 
    {
        SudokuGame game;  // Initialize new game instance
        
        // Display game introduction and rules
        cout << FG_YELLOW << BOLD << "\n                                   OH! So You Think You Good With Numbers AY!! Lets's See If You Live Upto Your Confidence" << RESET;
        cout << FG_GREEN << BOLD << "\n\n\n                                                                  Welcome to Sudoku!\n\n"
                  <<"                                                                       Rules:\n"
                  <<"                                                         1. Correct numbers become permanent\n"
                  <<"                                                         2. You get 2 hints\n"
                  <<"                                                         3. 3 wrong attempts end the game\n"
                  <<"                                                         4. Press 'q' or 'Q' to quit game\n\n" << RESET;
        
        bool shouldExit = false;  // Flag to control game exit
        while (!shouldExit) 
        {
            game.printBoard();  // Display current board state

            int row, col, num;
            cout << FG_YELLOW << BOLD << "Enter row (1-9), column (1-9), number (1-9) [e.g. Format: 1 2 3] / Press 0 0 0 for a hint / 'q' to quit: " << RESET;
            
            // Check for quit command first
            char firstInput;
            cin >> firstInput;
            
            if (firstInput == 'q' || firstInput == 'Q') 
            {
                clearInputBuffer();
                if (handleQuitOption()) 
                {
                    shouldExit = true;  // Restart game
                } 
                else 
                {
                    cout << FG_GREEN << "\n\n                                                                  Thanks for playing! Goodbye!\n\n" << RESET;
                    playAgain = false;
                    shouldExit = true;
                }
                continue;
            }
            
            // Put the character back if it wasn't a quit command
            cin.putback(firstInput);
            
            // Validate numeric input
            if (!(cin >> row >> col >> num)) 
            {
                cout << FG_RED << "\n\n                                                               Invalid input! Enter numbers only\n\n" << RESET;
                clearInputBuffer();
                continue;
            }

            // Handle hint request
            if (row == 0 && col == 0 && num == 0) 
            {
                if (game.getHintsUsed() < 2) 
                {
                    game.provideHint();
                } 
                else 
                {
                    cout << FG_RED << "\n\n                                                                    No hints left!\n\n" << RESET;
                }
                continue;
            }

            // Validate input range
            if (row < 1 || row > 9 || col < 1 || col > 9 || num < 1 || num > 9) 
            {
                cout << FG_RED << "\n\n                                                                      Numbers must be 1-9!\n\n" << RESET;
                continue; 
            }
            
            // Handle save/load commands (hidden features)
            if (row == -1 && col == -1 && num == -1) 
            {
                game.saveGame("sudoku_save.txt");
                continue;
            }
            else if (row == -2 && col == -2 && num == -2) 
            {
                game.loadGame("sudoku_save.txt");
                continue;
            }

            // Check if cell is already fixed (correct)
            if (game.isFixed(row-1, col-1)) 
            {
                cout << FG_GREEN << "\n\n                                                     Cell (" << row << "," << col << ") is already correct!\n\n" << RESET;
                continue;
            }

            // Validate move against Sudoku rules
            if (!game.isValid(row-1, col-1, num)) 
            {
                cout << FG_RED << "\n\n                                                           Invalid move! Number conflicts with existing numbers.\n\n" << RESET;
                continue;
            }

            // Check if number matches solution
            if (num != game.getSolutionAt(row-1, col-1)) 
            {
                game.incrementWrongAttempts();
                int attemptsLeft = 3 - game.getWrongAttempts();
                cout << FG_RED << BOLD << "\n\n                                                           Wrong! Attempts left: " << attemptsLeft << "\n\n" << RESET;
                
                // Game over condition
                if (game.getWrongAttempts() >= 3) 
                {
                    cout << FG_RED << "\n\n                                                                    Game Over! Good Luck Next Time\n\n" << RESET;
                    game.revealSolution();
                    break;       
                }
                continue;
            }

            // Valid move - update board
            game.updateBoard(row-1, col-1, num);
            cout << FG_GREEN << "\n\n                                                                         Correct!\n\n" << RESET;

            // Check for win condition
            if (game.isComplete()) 
            {
                cout << FG_GREEN << "\n\n                                                           Congratulations! You solved the Sudoku!\n\n" << RESET;
                break;
            }
        }

        if (!playAgain) break;

        // Post-game menu
        char playAgainChoice;
        while (true) 
        {
            cout << FG_BLUE <<  "\n\n                                                                       Play again? (y/n): " << RESET;
            cin >> playAgainChoice;
            clearInputBuffer();
            
            if (playAgainChoice == 'y' || playAgainChoice == 'Y') 
            {
                break;  // Continue outer loop for new game
            } 
            else if (playAgainChoice == 'n' || playAgainChoice == 'N') 
            {
                cout << FG_GREEN << "\n\n                                                                  Thanks for playing! Goodbye!\n\n" << RESET;
                playAgain = false;
                break;
            } 
            else 
            {
                cout << FG_RED << "\n\n                                                               Invalid input! Please enter 'y' or 'n'.\n" << RESET;
            }
        }
    }
}
