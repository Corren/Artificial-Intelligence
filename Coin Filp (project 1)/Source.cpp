// Jonathan Gregory
// CS 461
// Program 1
// September 4, 2017

// This program assumes that the user has the desired text files in the same directory as this program. 
// It assumes that the first number is the total number of ints in the file after it.
// It assumes that each int is seperated by a space.
// Its prints out the best possible senario to take the most money.
#include <algorithm>
#include <fstream> 
#include <iostream>
#include <iterator>
#include <locale>
#include <sstream>
#include <string>
#include <vector>

using namespace std;

const int MAX = 100;

void printVector(const vector<int>& vec);
void printScore(int user, int ai);
void getFileAndGame(ifstream & fin);
void aiMove(vector<int> &valueline, int & aiScore);


// this will take in a vecotr and print out the entire contents
void printVector(const vector<int>& vec)
{
	cout << endl << "----------------------------------------------------" << endl;
	cout << "Coins left: ";
	for (int i = 0; i < vec.size(); i++) {
		cout << vec[i] << " ";
	}
	cout << endl << "----------------------------------------------------" << endl << endl;
}

// this prints out the current score
void printScore(int user, int ai)
{
	cout << endl << "-----------------------Score------------------------" << endl;
	cout << "AI: " << ai << endl;
	cout << "User: " << user << endl;
	cout << "----------------------------------------------------" << endl;
}

// This function retrieves the info inside a text file and ask the user to choose the right or left item
// input: open stream from the file
// returns: vector of all the ints
void getFileAndGame(ifstream & fin)
{
	// master vector to hold all the contents and a tmp one to hold the first number
	vector<int> valueline;
	vector<int> tempVector;

	// score cards for both the user and the AI
	int aiScore = 0;
	int userScore = 0;

	// holds info form the file
	string line;
	bool firstPass = true;
	int firstNumber;

	// fills the vector with the numbers from the file
	while (getline(fin, line))
	{
		std::istringstream iss(line);
		int n;// temp varabile to hold the numbers
		while (iss >> n)
		{
			if (firstPass)
			{
				tempVector.push_back(n);
				firstNumber = tempVector[0];
				firstPass = false;
			}
			else
			{
				valueline.push_back(n);
			}
		}

		printScore(userScore, aiScore); // prints out the current score
		printVector(valueline); // prints out the vector

		while (valueline.size() != 0) // keeps looping till the vector has no more options for the players
		{
			string selection; // holds the users selection about taking the right or the left coin.
			locale locator; //Using locale file

			cout << "Do you want to take the right or left coin?" << endl;
			cout << "Answer with either r or l respectively." << endl;

			//get user input and take left or right coin, will not crash if non (l/r) value is entered
			cout << "Selection: ";
			cin >> selection;

			string::size_type i = 0; //first char of the string
			switch (tolower(selection[i], locator))
			{
			case 'L':
			case 'l': //remove left coin and add to user score
				cout << "You entered in [" << selection << "] and selected " << valueline.front() << "." << endl;
				userScore += valueline.front();
				valueline.erase(valueline.begin());
				// logic for AI choice
				aiMove(valueline, aiScore);
				printScore(userScore, aiScore); // prints out the current score
				printVector(valueline); // prints out the vector
				break;
			case 'R':
			case 'r': //remove right coin and add to user score
				cout << "You entered in [" << selection << "] and selected " << valueline.back() << "." << endl;
				userScore += valueline.back();
				valueline.pop_back();
				// logic for AI choice
				aiMove(valueline, aiScore);
				printScore(userScore, aiScore); // prints out the current score
				printVector(valueline); // prints out the vector
				break;
			default: //Anything other than L or R
				cout << "You entered in [" << selection << "]." << endl;
				cout << "Not valid. Please select either R or L: " << endl;
				break;
			}
		}
		cout << endl << endl << "----------------------------------------------------" << endl;
		cout << "----------------------------------------------------" << endl;
		cout << "----------------This round is over.-----------------" << endl;
		cout << "----------------------------------------------------" << endl;
		cout << "-----------------------Final------------------------";
		printScore(userScore, aiScore); // prints out the current score
		cout << "----------------------------------------------------" << endl;
		cout << "----------------------------------------------------" << endl << endl << endl;
	}
	fin.close(); // closes the input file.
}


// This function calculates out what the best move the the AI in order to get the most points
// This function utilizes the formula that was provided in the directions
// Vi,j = max( {Ci + min(Vi+1,j-1, Vi+2,j)}, {Cj + min(Vi, j-2, Vi+1,j-1) } )
// Inspiration for this function was found on http://articles.leetcode.com/
void aiMove(vector<int> &valueline, int & aiScore) {
	int valuesMatrix[MAX][MAX] = { 0 };
	int option2, option1, option3;
	for (int x = 0; x < valueline.size(); x++)
	{
		for (int i = 0, y = x; y < valueline.size(); i++, y++)
		{
			if (i + 1 <= valueline.size() - 1 && y - 1 >= 0)
				option1 = valuesMatrix[i + 1][y - 1];
			else
				option1 = 0;

			if (i + 2 <= valueline.size() - 1)
				option2 = valuesMatrix[i + 2][y];
			else
				option2 = 0;

			if (y - 2 >= 0)
				option3 = valuesMatrix[i][y - 2];
			else
				option3 = 0;

			valuesMatrix[i][y] = max(valueline[i] + min(option2, option1), valueline[y] + min(option1, option3));
		}
	}
	// This makes the selectiion for the AI based on the optimal outcome
	int i = 0;
	int j = valueline.size() - 1;
	if (valuesMatrix[i + 1][j] <= valuesMatrix[i][j - 1])
	{// ai choose the number on the right side
		aiScore += valueline.front(); // adds the score
		cout << "AI has selected " << valueline.front() << "." << endl;
		valueline.erase(valueline.begin()); // removes the element
	}
	else
	{// ai choose the number on the left side
		aiScore += valueline.back(); // adds the score
		cout << "AI has selected " << valueline.back() << "." << endl;
		valueline.pop_back(); // removes the element
	}
}

// This function takes the users input on what file to choose from
int main()
{
	ifstream fin;
	string infilename;
	string input;
	locale locator; //Using locale file

	cout << "This program reads in a text file of numbers and gives you a strategy." << endl;
	do // will keep asking the user for what they want to do until
	{
		cout << endl << endl;
		cout << "Enter the following for:" << endl;
		cout << "  1 for odd_coins.txt" << endl;
		cout << "  2 for simple_coins.txt" << endl;
		cout << "  3 for usa_coins.txt" << endl;
		cout << "  4 for a file of your choice." << endl;
		cout << "  5 to exit." << endl << endl;
		cout << "Selection: ";

		cin >> input;

		string::size_type i = 0; //first char of the string
		switch (tolower(input[i], locator))
		{
		case '1':
			infilename = "odd_coins.txt";
			fin.open("odd_coins.txt");
			cout << "Loaded " << infilename << endl;
			getFileAndGame(fin);
			fin.close();
			break;
		case '2':
			fin.open("simple_coins.txt");
			cout << "Loaded " << "simple_coins.txt" << endl;
			getFileAndGame(fin);
			fin.close();
			break;
		case '3':
			fin.open("usa_coins.txt");
			cout << "Loaded " << "usa_coins.txt" << endl;
			getFileAndGame(fin);
			fin.close();
			break;
		case '4':
			cout << "Please enter the main list (ie. test.txt): " << flush;
			while (true)
			{
				getline(cin, infilename);
				fin.open(infilename.c_str());
				if (fin)
				{
					cout << "Found " << infilename << endl;
					getFileAndGame(fin);
					break;
				}
				cout << "<<Invalid file. Please enter a valid input file name>> :" << flush;
			}
			fin.close();
			break;
		case '5':
			break;
		default:
			cout << "Error, not an option. Try again.\n";
			break;
		}
	} while (input != "5"); // waits for the user to enter 5 to leave the loop

	system("pause");
	return 0;
}