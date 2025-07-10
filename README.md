# SUDOKU
Sudoku Game

Overview

This is a command-line Sudoku game implemented in C++. The game allows players to solve a randomly generated 9x9 Sudoku puzzle with the following features:





Interactive gameplay with input validation.



Two hints per game to assist players.



Three wrong attempts before game over.



Option to save and load game progress.



Color-coded terminal output for better user experience.



Ability to quit and restart the game.

Files





sudoku.h: Header file defining the SudokuGame class and GameData structure.



sudoku.cpp: Implementation of the Sudoku game logic, including puzzle generation and solving.



main.cpp: Main game loop, user input handling, and console output formatting.



Makefile: Build script to compile the project.

Requirements





A C++ compiler (e.g., g++).



Linux/Unix-based system or a terminal emulator supporting ANSI color codes (e.g., Windows Subsystem for Linux, Cygwin).



Standard C++ libraries.

Compilation





Ensure all source files (sudoku.h, sudoku.cpp, main.cpp, Makefile) are in the same directory.



Open a terminal in the project directory.



Compile the game using:

make

This generates an executable named sudoku.



To clean up object files and the executable:

make clean

Running the Game





After compilation, run the game with:

./sudoku



Follow the on-screen instructions:





Enter row, column, and number (1-9) in the format row col num (e.g., 1 2 3).



Enter 0 0 0 to request a hint (up to 2 hints available).



Enter q or Q to access the quit menu (restart or exit).



(Hidden feature) Enter -1 -1 -1 to save the game to sudoku_save.txt.



(Hidden feature) Enter -2 -2 -2 to load a game from sudoku_save.txt.

Gameplay





The game starts with a randomly generated Sudoku puzzle.



Players input moves to fill the 9x9 grid.



Correct moves become permanent; incorrect moves count toward a limit of three wrong attempts.



The game ends when the puzzle is solved or three wrong attempts are made.



After completion, players can choose to play again or exit.

Notes





The puzzle is generated with 40–54 empty cells for varying difficulty.



The game uses ANSI color codes for a visually appealing interface (green for board, cyan for headers, red for errors, etc.).



Saved games are stored in sudoku_save.txt in the project directory.

Troubleshooting





"make: *** No targets. Stop.": Ensure the Makefile exists and is correctly formatted. Recreate it if needed (see previous instructions).



Missing files: Verify that sudoku.h, sudoku.cpp, and main.cpp are present in the directory.



Compiler errors: Ensure g++ is installed (sudo apt install g++ on Ubuntu/Debian).

License

This project is for educational purposes and is not licensed for commercial use.
