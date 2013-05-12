

//  sudoku.cpp
//  Assignment 6
//
//  Created by Aaron Lee on 2013-03-24.
//  Copyright (c) 2013 Aaron Lee. All rights reserved.

#include "sudoku.h"
using namespace std;

//Default Constructor
Sudoku::Sudoku() {
    for (int row =0; row < 9; row++) {
        for (int col =0; col < 9; col++) {
            puzzle[row][col] = new Cell ();
        }
    }
}

//Copy constructor
Sudoku::Sudoku (const Sudoku& other) {
    
    //Copy the puzzle
    for (int row = 0; row < 9; row ++) {
        for (int col = 0; col < 9; col ++) {
            
            Cell *newCell = new Cell();
            Cell *otherCell = other.puzzle[row][col];
            
            for (int num = 1; num < 10; num++) {
                if (!otherCell->at(num)) {
                    newCell->removeCandidate(num);
                }
            }
            
            this->puzzle[row][col] = newCell;
        }
    }
}

//Destructor
Sudoku::~Sudoku () {
    
    //Empty the final stack
    while (!finalStack.empty()) {
        SolvedCell *cellToDelete = finalStack.top();
        delete cellToDelete;
        finalStack.pop();
    }
    
    //Nullify each Cell in the 2D array
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            Cell *cellToDelete = puzzle[row][col];
            delete cellToDelete;
        }
    }
}

//Initialize a Sudoku with translated string
Sudoku::Sudoku (std::string input){
    
    int row = 0;
    int col = 0;
    
    //For each char in the input, check if it's '.' or an integer
    for (int num = 0; num < input.size(); num++) {
        char ch = input[num];
        
        if (col > 8) {
            col = 0;
            row++;
        }
        
        if ( ch == '.') {
            Cell *newCell = new Cell();
            puzzle[row][col] = newCell;
            col++;
        }
        
        else {
            int integer = ch - 48;
            SolvedCell* newSolvedCell = new SolvedCell(row, col, integer);
            Cell *newCell = new Cell(integer);
            puzzle[row][col] = newCell;
            finalStack.push(newSolvedCell);
            col++;
        }
    }
}

//Removes val as a possible candidate from all cells in the same
//row, column, and box. Any cell that is left with only one possible
//candidate gets added to finalStack
bool Sudoku::eliminate (int row, int col, int val) {
    
    //Go through each cell horizontally at the cell's position and remove candidate
    for (int c = 0; c < 9; c++) {
        Cell *newCell = puzzle[row][c];
        
        //Check if to make sure it's not the same cell
        if (c != col) {
            
            if (newCell->possibilities() == 1) {
                //Two cells with the same value = false
                if (newCell->first() == val) {
                    return false;
                }
            }
            
            else {
                
                if (!newCell->removeCandidate(val)) {
                    return false;
                }
                if (newCell->possibilities() == 1) {
                    
                    if (newCell->first() == val) {
                        return false;
                    }
                    
                    SolvedCell *solvedCell = new SolvedCell (row, c, newCell->first());
                    finalStack.push(solvedCell);
                }
            }
        }
        
    }
    
    //Go through each cell vertically at the cell's position and remove candidate
    for (int r = 0; r < 9; r++) {
        Cell *newCell = puzzle[r][col];
        
        if (r != row) {
            
            if (newCell->possibilities() == 1) {
                //Two cells with the same value = false
                if (newCell->first() == val) {
                    return false;
                }
            }
            
            else {
                
                if(!newCell->removeCandidate(val)) {
                    return false;
                }
                if (newCell->possibilities() == 1) {
                    
                    if (newCell->first() == val) {
                        return false;
                    }
                    
                    
                    SolvedCell *solvedCell = new SolvedCell (r, col, newCell->first());
                    finalStack.push(solvedCell);
                }
            }
        }
    }
    
    //Find which box the cell is in
    int boxRow;
    int boxCol;
    
    if (row < 3) {
        boxRow =0;
    }
    else if (row < 6) {
        boxRow = 3;
    }
    
    else {
        boxRow = 6;
    }
    
    if (col < 3) {
        boxCol =0;
    }
    else if (col < 6) {
        boxCol = 3;
    }
    else {
        boxCol = 6;
    }
    
    //Go through each cell in the box and remove candidate
    for (int r = boxRow; r < boxRow+3; r++) {
        for (int c = boxCol; c < boxCol+3; c++) {
            Cell *newCell = puzzle[r][c];
            
            if (r != row && c!=col) {
                
                if (newCell->possibilities() == 1) {
                    //Two cells with the same value = false
                    if (newCell->first() == val) {
                        return false;
                    }
                }
                
                else {
                    
                    if(!newCell->removeCandidate(val)) {
                        return false;
                    }
                    if (newCell->possibilities() == 1) {
                        
                        if (newCell->first() == val) {
                            return false;
                        }
                        
                        SolvedCell *solvedCell = new SolvedCell (r, c, newCell->first());
                        finalStack.push(solvedCell);
                    }
                }
            }
        }
    }
    return true;
}

//Removes a candidate from the cell with the least
//number of possible candidates. Returns a pointer
//to a Sudoku object for which the target cell is
//finalized to the removed candidate. If no guess
//can be made NULL is returned
Sudoku* Sudoku::guess () {
    
    bool foundCan = false;
    int lowestCandidate = 10;
    
    int savedRow;
    int savedCol;
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            
            Cell *cell = this->puzzle[row][col];
            
            //Find the lowest # of candidates and then save the coordinates of that cell
            if (cell->possibilities() < lowestCandidate && cell->possibilities() > 1){
                lowestCandidate = cell->possibilities();
                savedCol = col;
                savedRow = row;
                foundCan = true;
            }
        }
    }
    
    if (!foundCan) {
        return NULL;
    }
    
    else {
        
        Sudoku *newPuzzle = new Sudoku(*this);
        
        //Find that cell, remove all the other candidates so that it only has 1 candidate (the least)
        Cell *cell = this->puzzle[savedRow][savedCol];
        Cell *newCell = newPuzzle->puzzle[savedRow][savedCol];
        int leastCandidate = newCell->first();
        
        //Remove the candidate from the original puzzle
        cell->removeCandidate(leastCandidate);
        
        //If by removing the guess candidate from the original puzzle, it leaves with one possibility, then push it onto the stack
        if (cell->possibilities() == 1) {
            SolvedCell *solvedCell = new SolvedCell(savedRow,savedCol,cell->first());
            this->finalStack.push(solvedCell);
            //            this->finalize();
        }
        
        for (int num = 1; num < 10; num++) {
            if (num != leastCandidate) {
                newCell->removeCandidate(num);
            }
        }
        
        SolvedCell *solvedCell = new SolvedCell(savedRow,savedCol,leastCandidate);
        newPuzzle->finalStack.push(solvedCell);
        return newPuzzle;
    }
}

//Adds horizontally solvable cells to stack
//Looks for a cell with a candidate not found in the other cell in the same row
void Sudoku::horSolve () {
    
    //Check to see if a number is unique
    for (int num = 1 ; num < 10; num++) {
        
        //Go through each row
        for (int row = 0; row < 9; row++) {
            
            bool unique = false;
            bool within = false;
            int savedCol;
            
            //Go through each col
            for (int col = 0; col < 9; col++) {
                Cell *cell = puzzle[row][col];
                
                //Check that the cell has more than 1 candidate,
                //If there is a candidate, make sure it's unique
                if (cell->at(num)) {
                    if ( within == false) {
                        within = true;
                        unique = true;
                        savedCol = col;
                    }
                    else {
                        unique = false;
                        //                            break;
                    }
                }
            }
            
            if (unique) {
                Cell *cell = puzzle[row][savedCol];
                if (cell->possibilities() > 1) {
                    SolvedCell *newCell = new SolvedCell(row,savedCol,num);
                    finalStack.push(newCell);
                }
            }
        }
    }
}

//Adds vertically solvable cells to stack
void Sudoku::verSolve () {
    
    //Go through each col
    //Check to see if a number is unique
    for (int num = 1 ; num < 10; num++) {
        
        //Go through each row
        for (int col = 0; col < 9; col++) {
            
            bool unique = false;
            bool within = false;
            int savedRow;
            
            //Go through each col
            for (int row = 0; row < 9; row++) {
                Cell *cell = puzzle[row][col];
                
                //If there is a candidate, make sure it's unique
                if (cell->at(num)) {
                    
                    if ( within == false) {
                        within = true;
                        unique = true;
                        savedRow = row;
                    }
                    
                    else {
                        unique = false;
                    }
                }
            }
            
            if (unique) {
                Cell *cell = puzzle[savedRow][col];
                if (cell->possibilities() > 1) {
                    SolvedCell *newCell = new SolvedCell(savedRow, col,num);
                    finalStack.push(newCell);
                }
            }
        }
    }
    
}

//Adds solvable cells by box to stack
void Sudoku::boxSolve () {
    
    for (int box = 0; box < 9; box++) {
        int row;
        int col;
        
        //Set up the initial row and cols
        if (box == 0) {
            row = 0;
            col = 0;
        }
        
        else if (box == 1) {
            row = 0;
            col = 3;
        }
        
        else if (box == 2) {
            row = 0;
            col = 6;
        }
        
        else if (box == 3) {
            row = 3;
            col = 0;
        }
        
        else if (box == 4) {
            row = 3;
            col = 3;
        }
        
        else if (box == 5) {
            row = 3;
            col = 6;
        }
        
        else if (box == 6) {
            row = 6;
            col = 0;
        }
        
        else if (box == 7) {
            row = 6;
            col = 3;
        }
        
        else if (box == 8) {
            row = 6;
            col = 6;
        }
        
        for (int num = 1; num < 10; num++) {
            
            bool unique = false;
            bool within = false;
            int savedRow;
            int savedCol;
            
            //Go through each cell in the box and remove candidate
            for (int r = row; r < row+3; r++) {
                for (int c = col; c < col+3; c++) {
                    Cell *cell = puzzle[r][c];
                    
                    if (cell->at(num)) {
                        if (within == false) {
                            within = true;
                            unique = true;
                            savedRow = r;
                            savedCol = c;
                        }
                        
                        else {
                            unique = false;
                            //                                break;
                        }
                        
                    }
                }
            }
            
            if (unique) {
                Cell *cell = puzzle[savedRow][savedCol];
                if (cell->possibilities() > 1) {
                    SolvedCell *newCell = new SolvedCell(savedRow,savedCol,num);
                    finalStack.push(newCell);
                }
            }
            
        }
    }
}

//Finalizes cells in stack
int Sudoku::finalize () {
    
    if (finalStack.empty()) {
        return 0;
    }
    
    //Go through the whole final stack
    while (!finalStack.empty()) {
        
        SolvedCell* nextCell = finalStack.top();
        
        finalStack.pop();
        
        int cellRow = nextCell->row;
        int cellCol = nextCell->col;
        int cellVal = nextCell->val;
        
        Cell *cell = puzzle[cellRow][cellCol];
        
        //This means it was a cell that had a unique candidate in it's row/col/box, but needs to be removed of the other candidates
        if (cell->possibilities() > 1) {
            
            for (int num =1; num<10; num++) {
                if (cell->at(num) && num != cellVal) {
                    cell->removeCandidate(num);
                }
            }
        }
        
        //If eliminate errors, then after clearing memory out the finalStack, return -1
        if (!eliminate(cellRow, cellCol, cellVal)) {
            delete nextCell;
            
            while (!finalStack.empty()) {
                SolvedCell *nextCell = finalStack.top();
                delete nextCell;
                finalStack.pop();
            }
            
            return -1;
        }
        
        delete nextCell;
    }
    
    return 1;
}

//true if puzzle is solved, false otherwise
bool Sudoku::isSolved () {
    
    if (this == NULL) {
        return false;
    }
    
    //Check rows for any duplicates
    for (int row = 0; row < 9; row++) {
        bool numlist[9] = {0,0,0,0,0,0,0,0,0};
        
        //Go through each cell in the row to check if there are any duplicates, or if it has more than 1 candidate
        for (int col = 0; col < 9; col++) {
            
            Cell *cell = puzzle[row][col];
            
            // If there's more than one candidate, or no candidates, return false
            if (cell->possibilities() > 1 || cell->possibilities() == 0) {
                return false;
            }
            
            else {
                
                //There was already the value in the row before
                if (numlist[cell->first()-1] == true) {
                    return false;
                }
                
                else {
                    //Set the number as true
                    numlist[cell->first()-1] = true;
                }
            }
        }
        
        //Check through the numlist array to see if all the numbers are there
        for (int i =0 ; i < 9; i++) {
            
            if (numlist[i] == false) {
                return false;
            }
        }
    }
    
    //Check columns for any duplicates
    for (int col = 0; col < 9; col++) {
        
        //boolean array to check for all nums
        bool numlist[9] = {0,0,0,0,0,0,0,0,0};
        
        //Go through each cell in the column to check if there are any duplicates, or if it has more than 1 candidate
        for (int row = 0; row < 9; row++) {
            
            Cell *cell = puzzle[row][col];
            
            // If there's more than one candidate, or no candidates, return false
            if (cell->possibilities() > 1 || cell->possibilities() == 0) {
                return false;
            }
            else {
                //There was already the value in the row before
                if (numlist[cell->first()-1] == true) {
                    return false;
                }
                
                else {
                    //Set the number as true
                    numlist[cell->first()-1] = true;
                }
            }
        }
        //Check through the numlist array to see if all the numbers are there
        for (int i =0 ; i < 9; i++) {
            
            if (numlist[i] == false) {
                return false;
            }
        }
        
    }
    
    //Check each box for any duplicates
    for (int box = 0; box < 9; box++) {
        
        int row;
        int col;
        
        //Set up the initial row and cols
        if (box == 0) {
            row = 0;
            col = 0;
        }
        
        else if (box == 1) {
            row = 0;
            col = 3;
        }
        
        else if (box == 2) {
            row = 0;
            col = 6;
        }
        
        else if (box == 3) {
            row = 3;
            col = 0;
        }
        
        else if (box == 4) {
            row = 3;
            col = 3;
        }
        
        else if (box == 5) {
            row = 3;
            col = 6;
        }
        
        
        else if (box == 6) {
            row = 6;
            col = 0;
        }
        
        else if (box == 7) {
            row = 6;
            col = 3;
        }
        
        else if (box == 8) {
            row = 6;
            col = 6;
        }
        
        bool numlist[9];
        
        //First initialize an array to check for duplicates
        for (int i = 0; i < 9; i++) {
            numlist[i] = false;
        }
        
        for (int r = row; r <= row+2; r++) {
            for (int c = col; c <=col+2; c++) {
                
                Cell *cell = puzzle[r][c];
                
                // If there's more than one candidate, or no candidates, return false
                if (cell->possibilities() > 1 || cell->possibilities() == 0) {
                    
                    return false;
                }
                
                else {
                    
                    //There was already the value in the row before
                    if (numlist[cell->first()-1] == true) {
                        return false;
                    }
                    
                    else {
                        //Set the number as true
                        numlist[cell->first()-1] = true;
                    }
                }
            }
        }
        
        //Check through the numlist array to see if all the numbers are there
        for (int i =0 ; i < 9; i++) {
            
            if (numlist[i] == false) {
                return false;
            }
        }
    }
    
    return true;
}

std::ostream & operator<< (std::ostream & stream, const Sudoku & puzzleState){
    
    for (int row = 0; row < 9; row++) {
        for (int col = 0; col < 9; col++) {
            Cell *cell = puzzleState.puzzle[row][col];
            
            if (cell->possibilities() > 1) {
                stream << ". ";
            }
            else {
                stream << cell->first() << " ";
            }
            
            if (col == 8) {
                stream << std::endl;
            }
        }
    }
    
    return stream;
}