//***********************************************************************
// ITCS 3153 - A Star Search Problem	(C++ Verision)					*
//																		*
// This program will utilize the A Star Searching algorithm to find the *
// shortest path between two nodes on a grid. There are untraversable 	*
// nodes as well. The discovered path will be displayed betweens the	*
// two nodes.															*
// 																		*															
// Kendall Britton, 2022-03-20											* 
//***********************************************************************


// List of libraries that are required to run the program
#include <iostream>
#include <string>
#include <vector>
#include <stdlib.h>
#include <time.h>
#include <cmath>
using namespace std;

#define SIZE_OF_BOARD 15	// Defines the board size using a macro

char gridBoard[SIZE_OF_BOARD][SIZE_OF_BOARD] = {};	// Instantiates the array to be used

// Declares all the functions used
void createBoard();
void placeGridBlockers();
void startPosition();
int calcHeuristic();
void AStarAlgorithm();
void generateNeighbors();
void searchClosedList(int surroundingNode);
void redoBoard();
void createPath();


// The data structure to hold the parent node coordinates
typedef struct parentNode {
	int previousX;
	int previousY;
};

// The data structure to hold the node properties
typedef struct Node{
	int positionX;
	int positionY;

	Node(int x, int y) {
		positionX = x;
		positionY = y;
	}

	Node() {

	}

	char value;			// Value at current node (traverseable or not)
	parentNode parent;		// Parent of current node
	double g;			// Cost to current node from start
	double h;			// Heuristic code of current node
	double f;			
};

// The various vectors needed to complete the program
vector<Node> openList;
vector<Node> tempUpdate;
vector<Node> currentNode;
vector<Node> closedList;
vector<Node> tempPathUpdate;

bool alreadyInList = false;		// Variable to check a node is in the closed list

// Nodes that are assigned to start and end position that the user selects
Node goalNode;
Node startNode;

// Runs the main program
int main() {

	int count = 0;		// Variable to ensure a proper location is picked
	int contRun = 0;	// Allows the program to run forever

	srand(time(NULL));	// Allows for randomization of the board

	createBoard();		// Function that creates the board layout
	startPosition();	// Function that asks the user where they want to start

	cout << endl << endl;

	while (contRun != 1) {

		while (count != 1) {
			cout << "Select a column that you want to move to? (1 - 15)" << endl;		// Asks the user what X position to move to
			cin >> goalNode.positionX;


			cout << endl << endl;

			cout << "Select a row in your desired column that want to move to? (1 - 15)" << endl;	// Asks the user what Y position to move to
			cin >> goalNode.positionY;

			if (gridBoard[goalNode.positionY - 1][goalNode.positionX - 1] != 'X') {		// Checks to see if move is possible

				gridBoard[goalNode.positionY - 1][goalNode.positionX - 1] = '?';	// Places ? to dictate this is where user is moving to
				count++;


			}
			else {
				cout << endl << "You can not move here!" << endl << endl;
			}

			cout << endl << endl;

		}

		count = 0;

		cout << "Calculating shortest route..." << endl;
;
		AStarAlgorithm();		// Runs the A Star Algorithm Search

		cout << endl << endl;

		cout << "Updating new board location..." << endl << endl;

		redoBoard();	// Outputs the updated user position once shortest path is found
	}

	return 0;
}

// Function that creates the initial layout of the board
void createBoard() {

	int boardCount = 1;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			gridBoard[row][col] = 'O';		// Makes all the nodes traversable at first

		}

	}

	placeGridBlockers();	// Randomly places untraversable nodes

	cout << "  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 " << endl;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		cout << "-------------------------------------------------------------" << endl;

		for (int col = 0; col < SIZE_OF_BOARD; col++) {		// Displays the board with untraversable nodes as "X"
			
			if (gridBoard[row][col] == 'X') {

				cout << "| " << gridBoard[row][col];
			}
			else {

				cout << "| " << " ";
			}
			

			if (col == 14) {
				cout << " |" << " " << boardCount << endl;
				boardCount++;
			}
			else {
				cout << " ";
			}


		}

	}

	cout << "-------------------------------------------------------------" << endl;

}

// Function that creates the updated board after position has moved
void redoBoard() {
	int boardCount = 1;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			if (gridBoard[row][col] == '.') {		// Resets the path visuals

				gridBoard[row][col] = 'O';

			}

			if (gridBoard[row][col] == '@') {		// Resets the old start location visuals

				gridBoard[row][col] = 'O';

			}

			if (gridBoard[row][col] == '?') {		// Updates the new start location visuals

				gridBoard[row][col] = '@';

			}

		}

	}

	cout << "  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 " << endl;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		cout << "-------------------------------------------------------------" << endl;

		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			if ((gridBoard[row][col] == 'X') || (gridBoard[row][col] == '@')) {		// Redraws the board with updated locations

				cout << "| " << gridBoard[row][col];
			}
			else {

				cout << "| " << " ";
			}


			if (col == 14) {
				cout << " |" << " " << boardCount << endl;
				boardCount++;
			}
			else {
				cout << " ";
			}


		}

	}

	cout << "-------------------------------------------------------------" << endl;

}

// Function that randomly places untraversable nodes
void placeGridBlockers() {

	int tenPercentBoard = 10;
	int temp;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		for (int col = 0; col < SIZE_OF_BOARD; col++) {

			temp = (rand() % 10) + 1;		// Each node has a 10% chance of being untraversable

			if (temp == 1) {
				gridBoard[row][col] = 'X';
			}
		}
	}

}

// Function that dictates/places the start position per user say
void startPosition() {
	int count = 0;
	int boardCount = 1;


	while (count != 1) {
		cout << endl << endl;

		cout << "Select a column that you want to start in? (1 - 15)" << endl;		// Asks the user what X position to start in 
		cin >> startNode.positionX;


		cout << endl << endl;

		cout << "Select a row in your desired column that you want to start in? (1 - 15)" << endl;		// Asks the user what Y position to start in 
		cin >> startNode.positionY;

		cout << endl;

		startNode.parent.previousX = -1;		// This will help detect start position
		startNode.parent.previousY = -1;

		if (gridBoard[startNode.positionY - 1][startNode.positionX - 1] != 'X') {		// Checks if start position is possible
			gridBoard[startNode.positionY - 1][startNode.positionX - 1] = '@';			// Marks start location with "@"
			count++;
		}
		else {
			cout << endl << "This cannot be the start location!" << endl << endl;
		}
	}

	cout << "  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 " << endl;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {

		cout << "-------------------------------------------------------------" << endl;

		for (int col = 0; col < SIZE_OF_BOARD; col++) {


			if ((gridBoard[row][col] == 'X') || (gridBoard[row][col] == '@')) {		// Displays updated board with start position marked

				cout << "| " << gridBoard[row][col];
			}
			else {

				cout << "| " << " ";
			}



			if (col == 14) {
				cout << " |" << " " << boardCount << endl;
				boardCount++;
			}
			else {
				cout << " ";
			}


		}

	}

	cout << "-------------------------------------------------------------" << endl;
}

// Function that calculates the current heuristic using the Manhattan Method
int calcHeuristic(Node currentNode) {
	int heuristic;

	int tempX = abs(goalNode.positionX - currentNode.positionX) * 10;

	int tempY = abs(goalNode.positionY - currentNode.positionY) * 10;

	heuristic = tempX + tempY;

	return heuristic;

}

// Function that traces/displays the shortest possible path
void createPath() {

	tempPathUpdate.push_back(currentNode.at(0));

	while ((currentNode.at(0).parent.previousX != -1) && (currentNode.at(0).parent.previousY != -1)) {		// Adds the parent nodes from goal node to start node into path update vector
		for (int i = 0; i < closedList.size(); i++) {
			if ((currentNode.at(0).parent.previousY == closedList.at(i).positionY) && (currentNode.at(0).parent.previousX == closedList.at(i).positionX)) {
				tempPathUpdate.push_back(closedList.at(i));
				currentNode.pop_back();
				currentNode.push_back(closedList.at(i));
			}
		}
	}

	for (int i = tempPathUpdate.size() - 2; i > 0; i--) {
		gridBoard[tempPathUpdate.at(i).positionY - 1][tempPathUpdate.at(i).positionX - 1] = '.';		// Displays path visuals in proper path nodes
	}

}

// Function that performs the A Star Search Algorithm
void AStarAlgorithm() {

	openList.push_back(Node(startNode.positionX, startNode.positionY));		// Adds start node to open list
	openList.at(0).g = 0;													// Sets g value
	openList.at(0).h = calcHeuristic(openList.at(0));						// Calculates the current heuristic
	openList.at(0).f = openList.at(0).g + openList.at(0).h;					// Calcualtes the current f
	openList.at(0).parent.previousX = -1;									// Sets initial parent node
	openList.at(0).parent.previousY = -1;

	currentNode.push_back(openList.at(0));		// Set start node to current node and taken out of open list
	openList.pop_back();

	do {
		generateNeighbors();		// Finds all possible neighboring traversable nodes

		closedList.push_back(currentNode.at(0));	// Puts current node in closed list and removes it from being current node
		currentNode.pop_back();

		for (int i = openList.size() - 1; i >= 0; i--) {		// Searches for the next best node within the open list

			Node temp;

			if (openList.back().f > openList.at(i).f) {

				temp = openList.back();
				openList.back() = openList.at(i);
				openList.at(i) = temp;

			}

		}

		currentNode.push_back(openList.back());		// Sets next best node as current node and removes it from open list
		openList.pop_back(); 

	} 
	while (currentNode.at(0).h != 0);		// Operates until goal node is reached

	if ((currentNode.at(0).positionX != goalNode.positionX) && (currentNode.at(0).positionY != goalNode.positionY)) {		// If goal node is never reached, path is not possible

		cout << "There is no path available!" << endl << endl;
	}
	else {

		createPath();		// Else it creates the discovered path

	}

	cout << endl << "  1   2   3   4   5   6   7   8   9   10  11  12  13  14  15 " << endl;

	int boardCount = 1;

	for (int row = 0; row < SIZE_OF_BOARD; row++) {


		cout << "-------------------------------------------------------------" << endl;

		for (int col = 0; col < SIZE_OF_BOARD; col++) {		// Displays the best path between start node and goal node that was discovered


			if ((gridBoard[row][col] == 'X') || (gridBoard[row][col] == '@') || (gridBoard[row][col] == '?') || (gridBoard[row][col] == '.')) {

				cout << "| " << gridBoard[row][col];
			}
			else {

				cout << "| " << " ";
			}



			if (col == 14) {
				cout << " |" << " " << boardCount << endl;
				boardCount++;
			}
			else {
				cout << " ";
			}


		}

	}

	cout << "-------------------------------------------------------------" << endl;

	openList.clear();			// Clears open list
	closedList.clear();			// Clears closed list
	currentNode.clear();		// Clears current node
	tempPathUpdate.clear();		// Clears path vector
	startNode = goalNode;		// Sets goal node to new start node
}

// Function that generates all the possible neighboring nodes
void generateNeighbors() {

	alreadyInList = false;

	if (currentNode.at(0).positionY - 1 > 0) {		// Check above current node
		if (gridBoard[currentNode.at(0).positionY - 2][currentNode.at(0).positionX - 1] != 'X') {

			searchClosedList(1);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX, currentNode.at(0).positionY - 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 10;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
			
			}

		}
	}

	alreadyInList = false;

	if (currentNode.at(0).positionY + 1 < 16) {		// Check below current node
		if (gridBoard[currentNode.at(0).positionY][currentNode.at(0).positionX - 1] != 'X') {

			searchClosedList(2);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX, currentNode.at(0).positionY + 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 10;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if (currentNode.at(0).positionX + 1 < 16) {		// Check right of current node
		if (gridBoard[currentNode.at(0).positionY - 1][currentNode.at(0).positionX] != 'X') {

			searchClosedList(3);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX + 1, currentNode.at(0).positionY));

				tempUpdate.at(0).g = currentNode.at(0).g + 10;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if (currentNode.at(0).positionX - 1 > 0) {		// Check left of current node
		if (gridBoard[currentNode.at(0).positionY - 1][currentNode.at(0).positionX - 2] != 'X') {

			searchClosedList(4);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX - 1, currentNode.at(0).positionY));

				tempUpdate.at(0).g = currentNode.at(0).g + 10;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if ((currentNode.at(0).positionX - 1 > 0) && (currentNode.at(0).positionY - 1 > 0)) {		// Check top-left of current node
		if (gridBoard[currentNode.at(0).positionY - 2][currentNode.at(0).positionX - 2] != 'X') {

			searchClosedList(5);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX - 1, currentNode.at(0).positionY - 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 14;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if ((currentNode.at(0).positionX - 1 > 0) && (currentNode.at(0).positionY + 1 < 16)) {		// Check bottom-left of current node
		if (gridBoard[currentNode.at(0).positionY][currentNode.at(0).positionX - 2] != 'X') {

			searchClosedList(6);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX - 1, currentNode.at(0).positionY + 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 14;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if ((currentNode.at(0).positionX + 1 < 16) && (currentNode.at(0).positionY - 1 > 0)) {		// Check top-right of current node
		if (gridBoard[currentNode.at(0).positionY - 2][currentNode.at(0).positionX] != 'X') {

			searchClosedList(7);	// Checks if not already in closed list
				
			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX + 1, currentNode.at(0).positionY - 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 14;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}

	alreadyInList = false;

	if ((currentNode.at(0).positionX + 1 < 16) && (currentNode.at(0).positionY + 1 < 16)) {		// Check bottom-right of current node
		if (gridBoard[currentNode.at(0).positionY][currentNode.at(0).positionX] != 'X') {

			searchClosedList(8);	// Checks if not already in closed list

			if (alreadyInList == false) {

				tempUpdate.push_back(Node(currentNode.at(0).positionX + 1, currentNode.at(0).positionY + 1));

				tempUpdate.at(0).g = currentNode.at(0).g + 14;
				tempUpdate.at(0).h = calcHeuristic(tempUpdate.at(0));
				tempUpdate.at(0).f = tempUpdate.at(0).g + tempUpdate.at(0).h;

				tempUpdate.at(0).parent.previousX = currentNode.at(0).positionX;
				tempUpdate.at(0).parent.previousY = currentNode.at(0).positionY;

				openList.push_back(tempUpdate.at(0));

				tempUpdate.pop_back();
				
			}

		}
	}


}

// Function that checks to see if a node is already in the closed list
void searchClosedList(int surroundingNode) {

	if (surroundingNode == 1) {								// Checks if above node is in closed list
		for (int i = 0; i < closedList.size(); i++) {
		
				if ((closedList.at(i).positionX == currentNode.at(0).positionX) && (closedList.at(i).positionY == currentNode.at(0).positionY - 1)) {		
					alreadyInList = true;
				}

		}
	}

	if (surroundingNode == 2) {								// Checks if below node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX) && (closedList.at(i).positionY == currentNode.at(0).positionY + 1)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 3) {								// Checks if right node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX + 1) && (closedList.at(i).positionY == currentNode.at(0).positionY)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 4) {								// Checks if left node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX - 1) && (closedList.at(i).positionY == currentNode.at(0).positionY)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 5) {								// Checks if top-left node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX - 1) && (closedList.at(i).positionY == currentNode.at(0).positionY - 1)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 6) {								// Checks if bottom-left node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX - 1) && (closedList.at(i).positionY == currentNode.at(0).positionY + 1)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 7) {								// Checks if top-right node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX + 1) && (closedList.at(i).positionY == currentNode.at(0).positionY - 1)) {
				alreadyInList = true;
			}

		}
	}

	if (surroundingNode == 8) {								// Checks if bottom-right node is in closed list
		for (int i = 0; i < closedList.size(); i++) {

			if ((closedList.at(i).positionX == currentNode.at(0).positionX + 1) && (closedList.at(i).positionY == currentNode.at(0).positionY + 1)) {
				alreadyInList = true;
			}

		}
	}
	

}