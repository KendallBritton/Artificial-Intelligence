//***********************************************************************
// ITCS 3153 - 8 Queens Problem	(C++ Verision)							*
//																		*
// This program will utilize the Hill-Climbing algorithm to place eight *
// queens on a chessboard so that they all do not interact with each	*
// other. 																*
//																		*															
// Kendall Britton, 2022-02-01											* 
//***********************************************************************

// List of libraries that are required to run the program
#include <iostream>
#include <stdlib.h>
#include <time.h>
#include <vector>
using namespace std;

#define SIZE_OF_BOARD 8		// Defines the board size using a macro

// Instantiates all the global variables and array to be used
int board[SIZE_OF_BOARD][SIZE_OF_BOARD];
int tempBoard[SIZE_OF_BOARD][SIZE_OF_BOARD];
int goalState;
int neighborHeuristic = -1;
int stateCount = 0;
bool startState = true;

// Declares all the functions used
void setupBoard();
void displayBoard();
void populateBoard();
int calculateHeuristic();
int calculateTempHeuristic();
int sumDiagInteraction(int n);
void calculateNeighborHeuristic();
void updateQueenLocation();

// Runs the main program
int main() {

	int restartCount = 0;		// Keeps track of the total amount of restarts

	srand(time(NULL));		// Allows for randomization of the board

	setupBoard();		// Sets up an initial blank chessboard
	populateBoard();	// Populates a queen in a random row of every column 

	while (goalState != 0) {		// Checks to make sure that the heuristic isn't zero

		if (neighborHeuristic == 0) {				// Restarts the algorithm if there are no other options (local minima)
			cout << endl << "RESTART" << endl;
			setupBoard();
			populateBoard();
			restartCount++;							// Increments restart count
		}

		displayBoard();
		updateQueenLocation();
	}

	cout << "Solution Found!" << endl;					// Outputs solution found when there are no interactions on the board
	cout << "State Changes: " << stateCount << endl;	// Outputs total state changes
	cout << "Restarts: " << restartCount << endl;		// Outputs total restart count


}

// Function that sets up the initial blank chessboard
void setupBoard() {

	if (startState == true) {				// Checks if it is the first board or not
		cout << "Initial State" << endl;
		startState = false;
	}

	cout << endl << "Current h: 0" << endl;		// Displays the current heuristic
	cout << "Current State" << endl;
	
	for (int row = 0; row < SIZE_OF_BOARD; row++) {			// Sets up an 8 by 8 board filled with zeros or no queens
		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			board[row][col] = 0;

			if (col == 7) {
				cout << board[row][col] << " ";
			}
			else {

				cout << board[row][col] << ", ";

			}

		}

		cout << endl;

	}

	cout << "Neighbors found with lower h: 0" << endl;		// Displays the current amount of neighbor states  
	cout << "Setting new current state" << endl;

	goalState = -1;
}

// Function that populates the board with a queen in a random row in every column
void populateBoard() {

	for (int col = 0; col < SIZE_OF_BOARD; col++) {
		board[rand() % SIZE_OF_BOARD][col] = 1;
	}

}

// Function that displays the current state of the chessboard
void displayBoard() {

	cout << endl << "Current h: " << calculateHeuristic() << endl;		// Displays the current heuristic
	cout << "Current State" << endl;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {			// Displays the current state with the updated location of all the queens
		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			if (col == 7) {
				cout << board[row][col] << " ";
			}
			else {

				cout << board[row][col] << ", ";

			}

		}

		cout << endl;

	}

	calculateNeighborHeuristic();

	cout << "Neighbors found with lower h: " << neighborHeuristic << endl;		// Displays the current amount of neighbor states

	if (calculateHeuristic() != 0) {					// If neighbor state is not zero, proceed to next state
		cout << "Setting new current state" << endl;
	}


}

// Function that calculates the current heuristic of the chessboard
int calculateHeuristic() {
	int heuristic = 0;
	int diagCount = 0;

	// Horizontal Interactions
	for (int col = 0; col < SIZE_OF_BOARD; col++) {
		for (int row = 0; row < SIZE_OF_BOARD; row++) {
			for (int rowCheck = row + 1; rowCheck < SIZE_OF_BOARD; rowCheck++) {
				if ((board[col][row] == 1) && (board[col][rowCheck] == 1)) {
					heuristic++;
				}
			}

		}

	}

	// Top-left to bottom right (Lower Half) Interactions	
	for (int rowStart = 0; rowStart < SIZE_OF_BOARD - 1; rowStart++) {
		diagCount = 0;
		int row, col;
		for (row = rowStart, col = 0; row < SIZE_OF_BOARD && col < SIZE_OF_BOARD; row++, col++) {
			if (board[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-left to bottom right (Upper Half) Interactions
	for (int colStart = 1; colStart < SIZE_OF_BOARD - 1; colStart++) {
		diagCount = 0;
		int row, col;
		for (row = 0, col = colStart; row < SIZE_OF_BOARD && col < SIZE_OF_BOARD; row++, col++) {
			if (board[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-right to bottom left (Lower Half) Interactions	
	for (int rowStart = 0; rowStart < SIZE_OF_BOARD - 1; rowStart++) {
		diagCount = 0;
		int row, col;
		for (row = rowStart, col = SIZE_OF_BOARD - 1; row < SIZE_OF_BOARD && col > 0; row++, col--) {
			if (board[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-right to bottom left (Upper Half) Interactions
	for (int colStart = SIZE_OF_BOARD - 1; colStart > 0; colStart--) {
		diagCount = 0;
		int row, col;
		for (row = 0, col = colStart; row < SIZE_OF_BOARD && col > 0; row++, col--) {
			if (board[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	goalState = heuristic;

	return heuristic;
}

// Function that calculates the current heuristic of the temporary chessboard
int calculateTempHeuristic() {
	int heuristic = 0;
	int diagCount = 0;

	// Horizontal Interactions
	for (int col = 0; col < SIZE_OF_BOARD; col++) {
		for (int row = 0; row < SIZE_OF_BOARD; row++) {
			for (int rowCheck = row + 1; rowCheck < SIZE_OF_BOARD; rowCheck++) {
				if ((tempBoard[col][row] == 1) && (tempBoard[col][rowCheck] == 1)) {
					heuristic++;
				}
			}

		}

	}

	// Top-left to bottom right (Lower Half) Interactions	
	for (int rowStart = 0; rowStart < SIZE_OF_BOARD - 1; rowStart++) {
		diagCount = 0;
		int row, col;
		for (row = rowStart, col = 0; row < SIZE_OF_BOARD && col < SIZE_OF_BOARD; row++, col++) {
			if (tempBoard[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-left to bottom right (Upper Half) Interactions
	for (int colStart = 1; colStart < SIZE_OF_BOARD - 1; colStart++) {
		diagCount = 0;
		int row, col;
		for (row = 0, col = colStart; row < SIZE_OF_BOARD && col < SIZE_OF_BOARD; row++, col++) {
			if (tempBoard[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-right to bottom left (Lower Half) Interactions	
	for (int rowStart = 0; rowStart < SIZE_OF_BOARD - 1; rowStart++) {
		diagCount = 0;
		int row, col;
		for (row = rowStart, col = SIZE_OF_BOARD - 1; row < SIZE_OF_BOARD && col > 0; row++, col--) {
			if (tempBoard[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	// Top-right to bottom left (Upper Half) Interactions
	for (int colStart = SIZE_OF_BOARD - 1; colStart > 0; colStart--) {
		diagCount = 0;
		int row, col;
		for (row = 0, col = colStart; row < SIZE_OF_BOARD && col > 0; row++, col--) {
			if (tempBoard[col][row] == 1) {
				diagCount++;
			}

		}
		heuristic += sumDiagInteraction(diagCount);
	}

	return heuristic;
}

// Function that adds up the total diagonal interactions on the chessboard
int sumDiagInteraction(int n) {

	int totalInteractions = 0;

	for (int i = n; i > 1; i--) {
		totalInteractions += i - 1;
	}

	return totalInteractions;
}

// Function that calculates the total number of possible neighbor states
void calculateNeighborHeuristic() {

	int originalRow;
	int originalCol;
	int originalHueristic = calculateHeuristic();
	int count;

	neighborHeuristic = 0;

	vector<int> colHeuristic;

	for (int col = 0; col < SIZE_OF_BOARD; col++) {

		count = 0;

		for (int row = 0; row < SIZE_OF_BOARD; row++) {		// Detects the orginal location of a queen for each column

			if (board[row][col] == 1) {
				originalRow = row;
				originalCol = col;
				board[row][col] = 0;
			}

		}

		for (int row = 0; row < SIZE_OF_BOARD; row++) {		// Calculates the location of each index of each row

			board[row][col] = 1;
			colHeuristic.push_back(calculateHeuristic());
			board[row][col] = 0;



		}

		for (int row = 0; row < SIZE_OF_BOARD; row++) {			// Calcuates the total amount of lower heuristics states in each column
			if (originalHueristic > colHeuristic.at(row)) {
				count++;
			}
		}

		if (count > 0) {			// Calculates the total number of neighbor states across the whole board
			neighborHeuristic++;
		}

		board[originalRow][originalCol] = 1;		// Restores the original queen location

		colHeuristic.clear();

	}

}

// Functions that updates the new queen location on the chessboard
void updateQueenLocation() {

	// Variables used throughout function
	int orginalHeuristic = calculateHeuristic();
	int oldTempHeuristic, newTempHeuristic;
	oldTempHeuristic = 1000;
	int originalRow = 0;
	int originalCol = 0;

	for (int col = 0; col < SIZE_OF_BOARD; col++) {		// Copies original board to temporary board
		for (int row = 0; row < SIZE_OF_BOARD; row++) {
			if (board[row][col] == 1) {
				tempBoard[row][col] = 1;
			}
			else {
				tempBoard[row][col] = 0;
			}
		}
	}

	for (int col = 0; col < SIZE_OF_BOARD; col++) {			// Detects the location of queens within the temporary board
		for (int row = 0; row < SIZE_OF_BOARD; row++) {

			if (tempBoard[row][col] == 1) {
				originalRow = row;
				originalCol = col;
				tempBoard[row][col] = 0;
			}

		}

		for (int row = 0; row < SIZE_OF_BOARD; row++) {		// Calculate the current heuristic of the new state for temporary board
			tempBoard[row][col] = 1;
			newTempHeuristic = calculateTempHeuristic();


			if ((newTempHeuristic < orginalHeuristic) && (oldTempHeuristic > newTempHeuristic)) {		// Ensures that heuristic is better than previous heuristic

				oldTempHeuristic = newTempHeuristic;		// Updates temporary heuristic
				stateCount++;								// Updates the total state count

				for (int col2 = 0; col2 < SIZE_OF_BOARD; col2++) {		// Copies temporary board back to orginal board if heuristic is better
					for (int row2 = 0; row2 < SIZE_OF_BOARD; row2++) {
						if (tempBoard[row2][col2] == 1) {
							board[row2][col2] = 1;
						}
						else {
							board[row2][col2] = 0;
						}
					}
				}

			}

			tempBoard[row][col] = 0;		// Sets index back to zero
		}

		tempBoard[originalRow][originalCol] = 1;		// Restores the original queen location
	}
}