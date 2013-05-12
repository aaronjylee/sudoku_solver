#ifndef CELL_H
#define CELL_H
#include <iostream>

class Cell {
	//Stores the number of remaining candidates
	int count;

	//Remaining candidates are true, otherwise false
	bool candidates [9];

public:
	//Constructor
	Cell ();

	//Initialize a solved cell to number
	//Only candidate[number - 1] will be true
	Cell (int number);

	//Returns the numerically smallest candidate
	int first () const;

	//Returns the number of remaining candidates
	int possibilities () const;

	//Returns true if number - 1 is still a candidate for the cell, otherwise false
	bool at (int number) const;

	//Removes number - 1 as a candidate and returns false if doing so leaves no more candidate, otherwise true
	bool removeCandidate (int number);
};

#endif
