// Jonathan Gregory
// CS 461 Project 2
// Fall 2017
// Prof. Brian Hare

#include <iostream> //in and output
#include <string> 
#include <locale> 
#include <vector>
#include <locale> //Using locale file
#include <algorithm>    // std::min
#include <stdlib.h>     //rand 
#include <time.h>       //time 
using namespace std;

struct Queen
{
	int currentRow; // the current row
	int bestRow = -1;// the row with the fewest attackable spots - hopfully 0
	int attackable = 0; // the number of queens that it can attack at that time
	int lessAttackable = 0; // the number of attackable that are gone after moving to the bestRow from the 
};

void main();

void setup(vector<Queen>& chessBoard);

void solveAnother();

bool validateSolution(vector<Queen>& chessBoard, vector<vector<int>>& attackableValues, int n);

void printBoard(vector<Queen>& chessBoard, int n);

void hillClimbSearch(vector<Queen>& chessBoard);

void placeQueens(vector<Queen>& chessBoard, int n);

void moveQueen(vector<Queen>& chessBoard, vector<vector<int>>& attackableValues, int n, int col);

int validMove(vector<Queen>& chessBoard, vector<vector<int>>& attackableValues, int n, int & plateauCounter);
// This is the main that just ask if the user wants to find more solutions.
// This is done.
void main()
{
	char more;
	cout << "Welcome to the n-Queens problem solution finder!" << endl;

	do { // keeps giving more solutions till the user unters in n or N
		solveAnother();
		cout << endl << "Do you want to get another one [y/n]: ";
		cin >> more;

	} while (!cin.fail() && more != 'n' && more != 'N');
	system("pause");
}

// This function sets up the new chessboard with the new parameters
// This is done.
void setup(vector<Queen> &chessBoard)
{
	int n;
	cout << endl << "Given an N x N chessboard, place N queens such that no queen is attacking any other. Enter desired value for N: ";

	while (!(cin >> n)) { // validates that the user has entered in a number
		cin.clear();
		cin.ignore(numeric_limits<streamsize>::max(), '\n');
		cout << "Invalid number.  Try again: ";
	}
	cout << "You enterd: " << n << endl;
	placeQueens(chessBoard, n);
}

// This function just initiates the new n-Queens problem
// This is done.
void solveAnother()
{
	vector<Queen> chessBoard;
	setup(chessBoard);
	hillClimbSearch(chessBoard);
}

// This function verifies that the chessboard solution is valid given the constrains in the directions
// This is done.
bool validateSolution(vector<Queen>& chessBoard, vector<vector<int>> &attackableValues, int n)
{
	if (!chessBoard.empty())
	{
		for (int col = 0; col < n; col++)
		{
			int row = chessBoard[col].currentRow;
			if (attackableValues[row][col] != 0)//no problems with where the queen is located
				return false;
		}
		cout << "Here is your solution:" << endl;
	}	
	return true; // all values of hte attack array are 0 where the queens are located
}

// This function prints out a chessboard with the locations of all the queens
// This is done.
void printBoard(vector<Queen> &chessBoard, int n)
{
	if (!chessBoard.empty())
	{
		// creates the board that just shows the queens locations
		vector<vector<char>> printBoard(n, vector<char>(n, '.'));
		for (int col = 0; col < n; col++)//for every file
		{
			int rank = chessBoard[col].currentRow;
			printBoard[rank][col] = 'Q';
		}

		// prints out the vector we just made
		cout << " ";
		for (int i = 0; i < n * 2 + 1; i++)
		{
			cout << "=";
		}
		cout << endl;

		for (int i = 0; i < n; i++)
		{
			cout << "| ";
			for (int j = 0; j < n; j++)
				cout << printBoard[i][j] << " ";
			cout << "|" << endl;
		}

		cout << " ";
		for (int i = 0; i < n*2+1; i++)
		{
			cout << "=";
		}
		cout << endl << endl;
	}
}

// This function initiates the hill climbing search
// This is done.
void hillClimbSearch(vector<Queen> &chessBoard)
{
	int n = chessBoard.size();
	vector<vector<int>> attackableValues(n, vector<int>(n, 0));
	int plateauCounter = 0;
	bool success = false;

	// this finds the attack value for each cell
	for (int col = 0; col < n; col++) {   // removes the file
		int row = chessBoard[col].currentRow;
		int temp = attackableValues[row][col];

		for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {//ul attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i >= 0 && j < n; --i, ++j) {//up attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; j >= 0; --j) {//l attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; j < n; ++j) {//r attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i < n && j >= 0; ++i, --j) {//dl attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i < n && j < n; ++i, ++j) {//dr attack
			attackableValues[i][j] ++;
		}
		attackableValues[row][col] = temp;
	}

	while (!success) {
		moveQueen(chessBoard, attackableValues, n, validMove(chessBoard, attackableValues, n, plateauCounter));// detracts from the attack matrix
		success = validateSolution(chessBoard, attackableValues, n);
	}
	if (!chessBoard.empty())
	{
		printBoard(chessBoard, n);
	}
	cout << endl;
}

// This function generates the initial location of each queen on the chessboard
// This is done.
void placeQueens(vector<Queen> &chessBoard, int n)
{
	srand(time(NULL));
	// randomly places a queen in every col 
	for (int col = 0; col < n; col++) {
		Queen tempQueen; /// creats a new queen then addes it to the board array
		tempQueen.currentRow = (rand() % n);
		chessBoard.push_back(tempQueen);
	}
}

// This function moves the queen to the best row possible
// This is done.
void moveQueen(vector<Queen> &chessBoard, vector<vector<int>> &attackableValues, int n, int col)
{
	if (!chessBoard.empty()) {
		/// removes the col
		int row = chessBoard[col].currentRow;
		int temp = attackableValues[row][col]; // saves the queen spot

		for (int i = row, j = col; j < n; ++j) {//l attack
			attackableValues[i][j] --;
		}
		for (int i = row, j = col; i < n && j >= 0; ++i, --j) {//dl attack
			attackableValues[i][j] --;
		}
		for (int i = row, j = col; i < n && j < n; ++i, ++j) {//dr attack
			attackableValues[i][j] --;
		}
		for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {//ul attack
			attackableValues[i][j] --;
		}
		for (int i = row, j = col; i >= 0 && j < n; --i, ++j) {//ur attack
			attackableValues[i][j] --;
		}
		for (int i = row, j = col; j >= 0; --j) {//r attack
			attackableValues[i][j] --;
		}
		attackableValues[row][col] = temp; // adds the queen back

								  /// adds the new file
		row = chessBoard[col].bestRow;
		temp = attackableValues[row][col]; // saves the queens spot

		for (int i = row, j = col; j < n; ++j) {//r attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i < n && j >= 0; ++i, --j) {//dl attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i < n && j < n; ++i, ++j) {//dr attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i >= 0 && j >= 0; --i, --j) {//ul attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; i >= 0 && j < n; --i, ++j) {//ur attack
			attackableValues[i][j] ++;
		}
		for (int i = row, j = col; j >= 0; --j) {//r attack
			attackableValues[i][j] ++;
		}
		attackableValues[row][col] = temp; // adds the queen back

		chessBoard[col].currentRow = (chessBoard[col].bestRow); // updates the rank of the queen
	}
}

// This function finds the correct col to move and makes it
// This is done.
int validMove(vector<Queen> &chessBoard, vector<vector<int>> &attackableValues, int n, int &plateauCounter)
{
	int removedConflicts = 0; // how many conflics are removed from moving from current to best rank
	srand(time(NULL)); // generates the dseed for the random file 
	vector<int> minRows;

	//goes through every single col finding the best row, possition to move to in that file, all while updating the best row
	for (int col = 0; col < n; col++){
		// this is the queen location
		int rank = chessBoard[col].currentRow;
		vector<int> minRank; //lowest rank with the lowest attackable score
		chessBoard[col].attackable = (attackableValues[rank][col]);//set queen position attackable value
		int minAtk = chessBoard[col].attackable;

		//updates the bestRow with the lowest attackable score
		for (int rowBest = 0; rowBest < n; rowBest++){
			if (rowBest != chessBoard[col].currentRow){
				if (attackableValues[rowBest][col] < minAtk){
					minRank.clear();
					minRank.push_back(rowBest);
					minAtk = attackableValues[rowBest][col];
				}
				else if (attackableValues[rowBest][col] == minAtk){
					minRank.push_back(rowBest);
				}
			}
		}
		if (minRank.empty()) // if it is already in the best possition
			chessBoard[col].bestRow = (chessBoard[col].currentRow);
		else { // moves queen to another random location
			int bestRank = minRank[rand() % minRank.size()];
			chessBoard[col].bestRow = (bestRank);
		}

		// the best move is the one with the lessAttackable value
		chessBoard[col].lessAttackable = (chessBoard[col].attackable - minAtk);

		// ignore if it is attack value 0 and if the current rank is the best rank
		if (chessBoard[col].bestRow != chessBoard[col].currentRow && chessBoard[col].attackable != 0)
		{
			// if it is removing conflics then 
			if (chessBoard[col].lessAttackable > removedConflicts) {
				minRows.clear();
				minRows.push_back(col);
				removedConflicts = chessBoard[col].lessAttackable;
			}
			else if (chessBoard[col].lessAttackable == removedConflicts)
				minRows.push_back(col);
		}
	}

	//resets after taking care of the local min
	if (minRows.empty()) {
		cout << "NOTE: local minimum" << endl;
		// This resets the chess board. It clears out the attack array and the board.
		attackableValues.clear();
		chessBoard.clear();
		placeQueens(chessBoard, n);
		hillClimbSearch(chessBoard);
		return 1;
	}

	// plateau handling
	int bestR = (minRows[rand() % minRows.size()]);//moves to a random col in our range

 // detects of the problem has come to a plateau by checking of current conflicts are the same as the new ones
	if (chessBoard[bestR].attackable == attackableValues[chessBoard[bestR].bestRow][bestR]) {
		plateauCounter++; // adds one to the platu counter 

		// If 200 rounds are carried out and the number of attacks has not decreased, declare a failed search, alert the user, and start the search over
		if (plateauCounter > 200)
		{
			cout << endl << "NOTE: plateau" << endl;
			// This resets the chess board. It clears out the attack array and the board.
			attackableValues.clear();
			chessBoard.clear();
			placeQueens(chessBoard, n);
			hillClimbSearch(chessBoard);
			return 1;
		}
		return bestR;
	}
	else {//returns the best placment to move
		plateauCounter = 0;
		return bestR;
	}
}