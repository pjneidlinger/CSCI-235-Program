// Student Name: Phillip (PJ) Neidlinger
// Program Name: Battleship
// Creation Date: November 15, 2016
// Last Modified Date: November 28, 2016
// CSCI Course: CSCI 235
// Grade Received: 100?
// Comments Regarding Design: Dr. Hayes, the professor for my CSCI 235 class,
// created the enemyAI.h file for students to use as the opposing player in
// the Battleship game, we were required to create the Battleship.cpp file to
// make the game work.

// Include standard headers
#include <iostream>
#include <iomanip>
#include <thread>
#include <chrono>
#include <iomanip>
#include <cctype>
#include <iostream>
#include <cstdlib>
#include <ctime>
#include <climits>
using namespace std;

// Include custom headers
#include "gameSpecs.h"
#include "enemyAI.h"

/**
 * Displays random battleship coordinates in quick succession to make the
 * illusion that the computer/enemy is thinking about which coordinate to shoot
 * at.
 *
 * After the function returns the last random coordinate is still on the screen,
 * but the cursor is back up so that it can be overwritten with something new.
 */
void randomCoordinatesAnimation();

// Makes all locations in a passed array one tile, which is default water
void wipeBoard(Tile array[][BOARD_LENGTH], Tile input = WATER);

// Displays two passed arrays in the correct format
void displayBoards(const Tile playerBoard[][BOARD_LENGTH], const Tile
	enemyBoard[][BOARD_LENGTH], bool showAll = false);

// Places a specific ship horizontally on a passed array
bool placeShipHorizontally(Tile ship, int length, int xCoor, int yCoor,
	Tile board[][BOARD_LENGTH]);

// Places a specific ship vertically on a passed array
bool placeShipVertically(Tile ship, int length, int xCoor, int yCoor,
	Tile board[][BOARD_LENGTH]);

// Places all ships randomly on an array using other called functions
void placeShipsRandomly(Tile board[][BOARD_LENGTH]);

// Places all the user's ships until the player finds a board they want
void placePlayersShips(Tile playerBoard[][BOARD_LENGTH]);

// Prompts the user to pick a position to fire at and fires there
string playersTurn(Tile enemyBoard[][BOARD_LENGTH], int enemyShips[]);

// Takes a passed array and determines whether all the positions equal zero
bool isAllZeros(int enemyShips[]);

int main()
{
	// Two two dimensional Tile arrays that are the player's and enemy's boards
	Tile playerBoard[BOARD_LENGTH][BOARD_LENGTH];
	Tile enemyBoard[BOARD_LENGTH][BOARD_LENGTH];

	// Two arrays that keep track of the amount of hits left for each ship
	int enemyShips[6] = {0, 5, 4, 3, 3, 2};
	int playerShips[6] = {0, 5, 4, 3, 3, 2};

	// A boolean variable that tracks whether the game is over
	bool showAll;

	// A variable to seed the random number generator
	unsigned int seed = static_cast<unsigned int>(time(nullptr));

	// Seeds the random number generator
	srand(seed);

	// Wipes the enemy's board to start the game
	wipeBoard(enemyBoard);

	// Randomly places ships on the enemy's board
	placeShipsRandomly(enemyBoard);

	// Prompts the user to pick a randomly generated board
	placePlayersShips(playerBoard);

	// Loops through this until the game is finished
	do
	{
		// Displays both boards
		displayBoards(playerBoard, enemyBoard);

		// Prompts the user to make a shot and outputs it in the correct format
		cout << "Your shot: " << playersTurn(enemyBoard, enemyShips);

		// Checks to see whether all the enemy's ships have been sunk
		if (isAllZeros(enemyShips))
			break;

		// Displays both boards
		displayBoards(playerBoard, enemyBoard);

		// Outputs the enemy's shot in the correct format
		cout << "Enemy's Shot: " << EnemyAI::enemyTurn(playerBoard, playerShips);
	} while (!isAllZeros(playerShips)); // Loops while there are ships remaining

	// Checks to see who won
	if (isAllZeros(playerShips))
	{
		cout << "You Lost!\n";
	}
	else if (isAllZeros(enemyShips))
	{
		cout << "You Won!\n";
	}

	// Re-displays the boards showing the positions of all ships
	showAll = true;
	displayBoards(playerBoard, enemyBoard, showAll);

	return 0;
}

void randomCoordinatesAnimation()
{
	// Variables to hold random coordinates to print
	int colNumber;
	char rowLetter;

	cout << left; // left align columns

	// Repeatedly output a random coordinate, then wait, then backup to overwrite
	// The pauses increase in time exponentially until it is >= 160 msecs
	for (int msecs = 10; msecs < 160; msecs = static_cast<int>(msecs * 1.1))
	{
		// Get random numbers
		rowLetter = rand() % BOARD_LENGTH + 'A';
		colNumber = rand() % (BOARD_LENGTH) + 1;

		// Output numbers
		cout << rowLetter << setw(2) << colNumber << flush;

		// Wait the alloted time
		this_thread::sleep_for(chrono::milliseconds(msecs));

		// Backup for next output.
		cout << "\b\b\b";
	}

	cout << right; // reset alignment back to the default
}

void wipeBoard(Tile array[][BOARD_LENGTH], Tile input)
{
	// Loops through all values in the array and updates them to the passed tile
	for (int rowCounter = 0; rowCounter < BOARD_LENGTH; ++rowCounter)
	{
		for (int colCounter = 0; colCounter < BOARD_LENGTH; ++colCounter)
		{
			array[rowCounter][colCounter] = input;
		}
	}
}

void displayBoards(const Tile playerBoard[][BOARD_LENGTH], const Tile
	enemyBoard[][BOARD_LENGTH], bool showAll)
{
	// Sets up the row identifier variable
	char row = 'A';

	// Sets up the format for the boards
	cout << " Enemy's Fleet            Your Fleet" << endl;
	cout << "  1 2 3 4 5 6 7 8 910      1 2 3 4 5 6 7 8 910" << endl;

	// Loops through all values in the array
	for (int rowCounter = 0; rowCounter < BOARD_LENGTH; ++rowCounter)
	{
		// Outputs the row
		cout << row;

		// Loop for the enemy board
		for (int colCounter = 0; colCounter < BOARD_LENGTH; ++colCounter)
		{
			cout << setw(2);

			// If the game is over, output the enemies ships
			if(showAll)
			{
				cout << tileToSymbol(enemyBoard[rowCounter][colCounter]);
			}
			// Outputs a period if the location is water or an unhit ship
			else if(enemyBoard[rowCounter][colCounter] < MISS)
			{
				cout << '.';
			}
			// Ouputs an o if the location is already a missed hit location
			else if (enemyBoard[rowCounter][colCounter] == MISS)
			{
				cout << 'o';
			}
			// Outputs an x if the location is a ship that has been hit
			else if (enemyBoard[rowCounter][colCounter] > MISS)
			{
				cout << 'X';
			}
		}

		// Ouputs the space in between the boards and the same row identifier
		cout << "    " << row;

		// Ouputs the players board with all ships present
		for (int colCounter = 0; colCounter < BOARD_LENGTH; ++colCounter)
		{
			cout << setw(2) << tileToSymbol(playerBoard[rowCounter][colCounter]);
		}
		cout << endl;

		// Increments the row variable to move to the next row
		++row;
	}
}

bool placeShipHorizontally(Tile ship, int length, int xCoor, int yCoor,
	Tile board[][BOARD_LENGTH])
{
	// If the passed coordinate is not valid, return false
	if (xCoor < 0 || yCoor < 0 || (xCoor + length - 1) >= BOARD_LENGTH ||
		(yCoor + length - 1) >= BOARD_LENGTH)
	{
		return false;
	}

	// Makes sure a ship is not placed where there is another ship
	for (int index = 0; index < length; ++index)
	{
		if (board[yCoor][xCoor + index] > WATER)
			return false;
	}

	// If the first two loops pass, then the location is updated with the ship
	for (int index = 0; index < length; ++index)
	{
		board[yCoor][xCoor + index] = ship;
	}

	// True is returned if no loop returns false
	return true;
}

bool placeShipVertically(Tile ship, int length, int xCoor, int yCoor,
	Tile board[][BOARD_LENGTH])
{
	// If the passed coordinate is not valid, return false
	if (xCoor < 0 || yCoor < 0 || (xCoor + length - 1) >= BOARD_LENGTH ||
		(yCoor + length - 1) >= BOARD_LENGTH)
	{
		return false;
	}

	// Makes sure a ship is not placed where there is another ship
	for (int index = 0; index < length; ++index)
	{
		if (board[yCoor + index][xCoor] > WATER)
			return false;
	}

	// If the first two loops pass, then the location is updated with the ship
	for (int index = 0; index < length; ++index)
	{
		board[yCoor + index][xCoor] = ship;
	}

	// True is returned if no loop returns false
	return true;
}

void placeShipsRandomly(Tile board[][BOARD_LENGTH])
{
	// Variables to determine the starting location of a ship
	int randomXPos, randomYPos;

	// Variable for the ship to be placed
	Tile ship;

	// Variable to determine whether the ship was placed
	bool isShipPlaced;

	// Loops through all the ships
	for (int index = AIRCRAFT_CARRIER; index <= PATROL_BOAT; ++index)
	{
		// Loops while the ship is not placed
		do
		{
			// Gets a random x position
			randomXPos = rand() % BOARD_LENGTH;
			// Gets a random y position
			randomYPos = rand() % BOARD_LENGTH;
			// Gets a ship to be placed
			ship = static_cast<Tile>(index);

			// Uses random numbers to determine how a ship will be placed
			if (rand() % 2 == 0)
			{
				isShipPlaced = placeShipHorizontally(ship, SHIP_SIZE[ship],
					randomXPos, randomYPos, board);
			}
			else
			{
				isShipPlaced = placeShipVertically(ship, SHIP_SIZE[ship],
					randomXPos, randomYPos, board);
			}
		} while (!isShipPlaced);
	}
}

void placePlayersShips(Tile playerBoard[][BOARD_LENGTH])
{
	char input;

	// Loops until the user finds a board they will play with
	do
	{
		// Wipes the player's board
		wipeBoard(playerBoard, WATER);
		// Places random ships on the player's board
		placeShipsRandomly(playerBoard);
		// Displays a blank board and the player's board
		displayBoards(playerBoard, playerBoard);
		// Prompts the user to choose whether they will use the current board
		cout << "Would you like to play with this board? (y/n): ";
		cin >> input;
		input = tolower(input);

		while (input != 'n'  && input != 'y')
		{
			// If the input is not valid, ignore all remaining input and ask again
			cin.ignore(INT_MAX, '\n');
			cout << "Invalid entry, please enter y or n: ";
			cin >> input;
			input = tolower(input);
		}

		// Ignore anything after the first character
		cin.ignore(INT_MAX, '\n');
	} while (input != 'y');
}

string playersTurn(Tile enemyBoard[][BOARD_LENGTH], int enemyShips[])
{
	// A character variable to hold the input Row
	char inputRow;
	// One variable to hold the input column, and two to hold the actual indexes
	int inputColumn, indexColumn, indexRow;
	// A variable to determine whether the location is valid
	bool invalid;
	// A string to output whether the location was a hit or miss, default miss
	string hitOrMiss = " Miss!\n";
	// A string to be sent back at the end of this function
	string goesBack;
	// A Tile variable that will hold what ship is hit
	Tile shipHit;


	// Loops until a valid location has been chosen for the user to hit
	do
	{
		// Prompts the user to pick a coordinate to shoot at
		cout << "Enter the coordinates for a shot. (ex: B2): ";
		cin >> inputRow >> inputColumn;

		// Makes the row uppercase
		inputRow = toupper(inputRow);

		// Sets the invalid variable to false to begin the loop
		invalid = false;

		// Converts the input location to the correct array index locations
		indexRow = inputRow - 'A';
		indexColumn = inputColumn - 1;

		// If the input row is not valid, changes invalid to true
		if (indexRow < 0 || indexRow >= BOARD_LENGTH)
			invalid = true;
		// If the input column is not valid, changes invalid to true
		else if (!cin || inputColumn < 1 || inputColumn > BOARD_LENGTH)
			invalid = true;
		// If the position they would like to hit at is a miss or an already
		// hit ship, changes invalid to true
		else if(enemyBoard[indexRow][indexColumn] > PATROL_BOAT)
			invalid = true;

		// If the invalid variable has been changed to true
		if (invalid)
		{
			// Prompts the user for a valid coordinate to loop over again with
			cout << "Invalid row or column." << endl;
		}

		cin.clear();
		cin.ignore(INT_MAX, '\n');
	} while (invalid);

	// If the chosen location is water
	if (enemyBoard[indexRow][indexColumn] == WATER)
	{
		// The location is updated to be a miss
		enemyBoard[indexRow][indexColumn] = MISS;
		// Water in the enemyShips array is decremented to make it not zero
		enemyShips[WATER] -= 1;
		// The shiphit is updated to be water
		shipHit = WATER;
	}
	// If the chosen location is an aircraft carrier location
	else if (enemyBoard[indexRow][indexColumn] == AIRCRAFT_CARRIER)
	{
		// The location is updated to be a hit aircraft carrier
	 	enemyBoard[indexRow][indexColumn] = AIRCRAFT_CARRIER_HIT;
		// Aircraft Carrier in the enemyShips array is decremented
		enemyShips[AIRCRAFT_CARRIER] -= 1;
		// hitOrMiss is updated to say hit with a newline character
		hitOrMiss = " Hit!\n";
		// The shiphit is updated to be the aircraft carrier
		shipHit = AIRCRAFT_CARRIER;
	}
	else if (enemyBoard[indexRow][indexColumn] == BATTLESHIP)
	{
		// The location is updated to be a hit battleship
	 	enemyBoard[indexRow][indexColumn] = BATTLESHIP_HIT;
		// Battleship in the enemyShips array is decremented
		enemyShips[BATTLESHIP] -= 1;
		// hitOrMiss is updated to say hit with a newline character
		hitOrMiss = " Hit!\n";
		// The shiphit is updated to be the battleship
		shipHit = BATTLESHIP;
	}
	else if (enemyBoard[indexRow][indexColumn] == SUBMARINE)
	{
		// The location is updated to be a hit submarine
	 	enemyBoard[indexRow][indexColumn] = SUBMARINE_HIT;
		// Submarine in the enemyShips array is decremented
		enemyShips[SUBMARINE] -= 1;
		// hitOrMiss is updated to say hit with a newline character
		hitOrMiss = " Hit!\n";
		// The shiphit is updated to be the submarine
		shipHit = SUBMARINE;
	}
	else if (enemyBoard[indexRow][indexColumn] == DESTROYER)
	{
		// The location is updated to be a hit destroyer
		enemyBoard[indexRow][indexColumn] = DESTROYER_HIT;
		// Destroyer in the enemyShips array is decremented
		enemyShips[DESTROYER] -= 1;
		// hitOrMiss is updated to say hit with a newline character
		hitOrMiss = " Hit!\n";
		// The shiphit is updated to be the destroyer
		shipHit = DESTROYER;
	}
	else if (enemyBoard[indexRow][indexColumn] == PATROL_BOAT)
	{
		// The location is updated to be a hit patrol boat
		enemyBoard[indexRow][indexColumn] = PATROL_BOAT_HIT;
		// Patrol boat in the enemyShips array is decremented
		enemyShips[PATROL_BOAT] -= 1;
		// hitOrMiss is updated to say hit with a newline character
		hitOrMiss = " Hit!\n";
		// The shiphit is updated to be the patrol boat
		shipHit = PATROL_BOAT;
	}

	// The return string is updated with the row in character form
	goesBack = static_cast<char>(indexRow + 'A');
	// The index column is concatenated to the return string
	goesBack += to_string(indexColumn + 1);
	// Whether the shot was a hit or miss is concatenated to the return string
	goesBack += hitOrMiss;

	// If the shot was the final remaining hit on a ship
	if (enemyShips[shipHit] == 0)
	{
		// A line of prompt telling what ship was sunk is also returned
		goesBack += "\tYou sunk the enemy's ";
		goesBack += shipToString(shipHit);
		goesBack += "!\n";
	}

	// The return string is returned
	return goesBack;
}

bool isAllZeros(int arrayOfShips[])
{
	// If all the ships in the array have been sunk, return true
	if (arrayOfShips[AIRCRAFT_CARRIER] == 0 && arrayOfShips[BATTLESHIP] == 0 &&
		arrayOfShips[SUBMARINE] == 0 && arrayOfShips[DESTROYER] == 0 &&
		arrayOfShips[PATROL_BOAT] == 0)
		return true;
	// Otherwise, return false
	else
		return false;
}
