
//
//  driver.cpp
//  Assignment 6
//
//  Created by Aaron Lee on 2013-03-24.
//  Copyright (c) 2013 Aaron Lee. All rights reserved.

#include <iostream>
#include <cstdlib>
#include "sudoku.h"

using namespace std;

Sudoku* solve (Sudoku* puzzle) {
    
    puzzle->horSolve();
    puzzle->verSolve();
    puzzle->boxSolve();
    
    int finalizeResult = puzzle->finalize();
    
    if (finalizeResult == -1) {
        delete puzzle;
        return NULL;
    }
    
    else if (puzzle->isSolved()) {
        return puzzle;
    }
    
    //no more solvable cells
    else if (finalizeResult == 0) {
        
        //Create a new puzzle with a guess on the least candidate
        Sudoku *newPuzzle = puzzle->guess();
        
        //Call solve on the new puzzle guess
        Sudoku *solveGuess = solve(newPuzzle);
        
        if (newPuzzle != solveGuess) {
            delete newPuzzle;
        }
        
        //Failed
        if (solveGuess == NULL) {
            delete solveGuess;
            return solve(puzzle);
        }
        
        
        return solveGuess;
    }
    else {
        return solve(puzzle);
    }
}



int main(int argc, const char * argv[])
{
    
    //INPUT
    char input = ' ';
    string strInput = "";
    int count = 0;
    while (count < 81) {
        cin >> input;
        strInput += input;
        count++;
    }
    
    Sudoku *sudokuObj = new Sudoku(strInput);
    
    cout << *sudokuObj << endl;
    
    Sudoku *finalSudoku = solve(sudokuObj);
    
    if (finalSudoku != NULL) {
        cout << *finalSudoku;
        delete finalSudoku;
    }
    
    else {
        cerr << "ERROR" <<endl;
        delete sudokuObj;
        delete finalSudoku;
        return 0;
        
    }
    return 0;
}

