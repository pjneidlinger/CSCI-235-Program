/*
 * This file holds global constants, data types, and their associated helper
 * functions for the game of battleship.
 */
#ifndef ___GAME_SPECS___
#define ___GAME_SPECS___

#include <iostream>
#include <string>
using namespace std;

// An enum that holds tiles of water, the ships, misses, and hit ships
enum Tile { WATER, AIRCRAFT_CARRIER, BATTLESHIP, SUBMARINE, DESTROYER,
	PATROL_BOAT, MISS, AIRCRAFT_CARRIER_HIT, BATTLESHIP_HIT, SUBMARINE_HIT,
	DESTROYER_HIT, PATROL_BOAT_HIT };

// A constant integer that is the length of the boards in rows and column
const int BOARD_LENGTH = 10;

// A constant integer array that holds the length of all ships
const int SHIP_SIZE[6] = {0, 5, 4, 3, 3, 2};

// A function that takes a tile and returns a character of what tile it is
char tileToSymbol(Tile input)
{
	// Checks the input tile to see what tile it is
	switch (input)
	{
		// Returns a period for water
		case WATER:
			return '.';
		// Returns a capitalized version of the first letter in an unhit ships name
		case AIRCRAFT_CARRIER:
			return 'A';
		case BATTLESHIP:
			return 'B';
		case SUBMARINE:
			return 'S';
		case DESTROYER:
			return 'D';
		case PATROL_BOAT:
			return 'P';
		// Returns a tilde for a miss
		case MISS:
			return '~';
		// Returns a lowercase version of the first letter in a hit ships name
		case AIRCRAFT_CARRIER_HIT:
			return 'a';
		case BATTLESHIP_HIT:
			return 'b';
		case SUBMARINE_HIT:
			return 's';
		case DESTROYER_HIT:
			return 'd';
		case PATROL_BOAT_HIT:
			return 'p';
		// If the tile is not any of these, outputs an error and returns a space
		default:
			cout << "Error.";
			return ' ';
	}
}

// A function that takes a tile and returns a string of what tile the input is
string shipToString(Tile input)
{
	// Checks the input tile to see what tile it is
	switch (input)
	{
		// Returns a string of the name of the tile
		case WATER:
			return "Water";
		case MISS:
			return "Miss";
		case AIRCRAFT_CARRIER:
		case AIRCRAFT_CARRIER_HIT:
			return "Aircraft Carrier";
		case BATTLESHIP:
		case BATTLESHIP_HIT:
			return "Battleship";
		case SUBMARINE:
		case SUBMARINE_HIT:
			return "Submarine";
		case DESTROYER:
		case DESTROYER_HIT:
			return "Destroyer";
		case PATROL_BOAT:
		case PATROL_BOAT_HIT:
			return "Patrol Boat";
		// If the tile is not any of these, error is returned
		default:
			return "Error";
	}
}

#endif
