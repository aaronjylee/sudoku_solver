#ifndef SUDOKU_H
#define SUDOKU_H
#include <iostream>
#include <stack>
#include "cell.h"

class Sudoku {
	//The SolvedCell class is used to store the index (row, colomn coordinate)
	//of a cell that can be solved and the value to finalize the cell to
	class SolvedCell {
	public:
		//Stores the row of the cell to be finalied
		int row;
		//Stores the colomn of the cell to be finalied
		int col;
		//Stores the determined candidates value for the cell to be finalied
        int val;
		//Constructor
		SolvedCell () {};
		//Overloaded Constructor
		//Used to initialie values
		SolvedCell (int row, int col, int val) {
			this->row = row;
			this->col = col;
			this->val = val;
		}
	};

	//Represents a Sudoku puzzle
	Cell* puzzle [9][9];

    //finalStack is used to store all the cells that need to finalized.
	//Elements are removed from finalStack (poped of the stack) by finalize()
	std::stack<SolvedCell*> finalStack;

	//Removes val as a possible candidate from all cells in the same
	//row, colomn, and box (excluding the cell at the given coordinates).
	//Any cell that is left with only one possible candidate gets added to finalStack.
	//Returns false if a cell's candidate was removed leaving the cell with 0 possibilities.
	//Returns true otherwise
    
	bool eliminate (int row, int col, int val);

public:
	//Constructor
	Sudoku ();

	//Initializes a Sudoku with the translated string
	Sudoku (std::string input);

	//Copy Constructor
	Sudoku (const Sudoku& other);

	//Destructor
	~Sudoku ();

	//Removes a candidate from a target cell in "this" Sudoku puzzle.
	//Returns a pointer to a Sudoku object for which the target cell
	//has been finalized to the removed candidate
	Sudoku* guess ();
    
	//A cell with a uniquie candidate, in a row, is solvable.
	//horSolve finds such cells and adds them to finalStack
	void horSolve ();
    
	//A cell with a uniquie candidate, in a colomn, is solvable.
	//verSolve finds such cells and adds them to finalStack
	void verSolve ();
    
	//A cell with a uniquie candidate, in a box, is solvable.
	//boxSolve finds such cells and adds them to finalStack
	void boxSolve ();

	//For every cell in finalStack the function finalize() sets the
	//determined candidate for the cell to true and all other candidates
	//to false. finalize then calls eliminate to removes the value from
	//all cells in the same row, colomn, and box
	//A return value of -1 means the puzzle is a failure
	//A return value of 0 means finalStack was empty
	//A return value of 1 means all cells in finalStack were finalized
	int finalize ();

	//true if puzzle is solved, false otherwise
	bool isSolved ();

	//Output
	friend std::ostream & operator<< (std::ostream & stream, const Sudoku & puzzleState);
};
#endif
