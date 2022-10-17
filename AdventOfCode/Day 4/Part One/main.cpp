#include <iostream>
#include <vector>
#include <fstream>
#include <string>
#include <sstream>

using namespace std;

    int solveBoard(int currentBoard[][5], vector<int> drawnNumbers, int& boardSumOfUnmarked, int &drawnNumberCounter) {
    /*


    we will be populating a fake board with 1s for the drawn numbers
    1 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    
    1 0 0 0 0
    0 0 0 1 0
    0 0 0 0 0
    0 0 0 0 0
    0 0 0 0 0
    */
    drawnNumberCounter = 0;
    int board[5][5] = { 0 };
    boardSumOfUnmarked = 0;
    int tempRow = 0;
    int tempCol = 0;
    int winningRow = 0;
    int winningCol = 0;

    for (auto drawn : drawnNumbers) {
        ++drawnNumberCounter;
        for (int row = 0; row < 5; ++row) {
            for (int col = 0; col < 5; ++col) {
                if (currentBoard[row][col] == drawn) {
                    //adding 1 to the current position of the drawn number 
                    board[row][col] += 1;
                }
            }
        }

        // after the number has been drawn, we will check to see if a row or column consists of all 1s by adding the elements into two temp variables
        // If either of these do not equal the index of the loop, break the loop, if they equal 5, go to score because we have a row completed
        
        //testing if a row has won        
        for (int i = 0; i < 5; ++i) {
            tempRow = 0;

            if (board[i][0] == 0) {
                //do nothing as there is no drawn number yet
            }
            else {
                for (int j = 0; j < 5; ++j) {
                    if (board[i][j] == 1) {
                        ++tempRow;
                    }
                    else {
                        break;
                    }
                }
                if (tempRow == 5) {
                    winningRow = i;
                    goto score;
                }
            }
        }

        //testing if a column has won
        for (int j = 0; j < 5; ++j) {
            tempCol = 0;

            if (board[0][j] == 0) {
                //do nothing as there is no drawn number yet
            }
            else {
                for (int i = 0; i < 5; i++) {
                    if (board[i][j] == 1) {
                        ++tempCol;
                    }
                    else {
                        break;
                    }
                }
                if (tempCol == 5) {
                    winningCol = j;
                }
                else {
                    //do nothing
                }
            }
        }
    }

score:  for (int row = 0; row < 5; ++row) {
    for (int col = 0; col < 5; ++col) {
        if (board[row][col] != 1) {
            boardSumOfUnmarked += currentBoard[row][col];
        }
    }
}

for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
        cout << currentBoard[i][j] << " ";
    }
    cout << endl;
}

for (int i = 0; i < 5; ++i) {
    for (int j = 0; j < 5; ++j) {
        cout << board[i][j] << " ";
    }
    cout << endl;
}

for (int i = 0; i < drawnNumberCounter; ++i) {
    cout << drawnNumbers.at(i) << " ";
}

cout << endl;

return boardSumOfUnmarked * drawnNumbers.at(drawnNumberCounter-1);
}

int main() {

    vector<int> drawnNumbers;
    int currentBoard[5][5];
    int boardSumOfUnmarked = 0;
    double currentBoardFinalScore = 0;
    double bestBoardFinalScore = 0;
    int drawnNumberCounter = 0;
    int bestDrawnNumberCounter = 0;
    string line;

    /*
    Need to find the best board in a bingo game with the pre-defined drawn numbers
    and predefined boards from input.txt. Once we have found the best board(the board that wins the quickest)
    we need to calculate the score of that board, by finding the sum of all unmarked numbers, and multiply it by the
    number last called.

    first we need to grab all the drawn numbers and put them into vector.

    we know that the first row of a board is a maximum of 5 two digit integers, with 4 spaces inbetween
    meaning the length is at most 14 characters. we will use that as our check for bringing in the drawn numbers
    */

    int boardsTested = 0;

    fstream myFile("input.txt");
    while (myFile.is_open()) {
        string intermediate;
        int currentRow = 0;
        int currentColumn;

        while (getline(myFile, line)) {
            stringstream check1(line);

            if (line.length() <= 14 && !line.empty()) {
                //Now that we have the drawn numbers, we need to get our board.
                currentColumn = 0;
                while (getline(check1, intermediate, ' ')) {
                    if (!intermediate.empty()) {
                        currentBoard[currentRow][currentColumn] = stoi(intermediate);
                        currentColumn++;
                    }
                }

                //populated our current board, now we need to check when our current board is better than our best board
                if (currentRow == 4) {

                    currentBoardFinalScore = solveBoard(currentBoard, drawnNumbers, boardSumOfUnmarked, drawnNumberCounter);
                    ++boardsTested;



                    cout << "Current sum of unmarked: " << boardSumOfUnmarked << " winning Number: " << drawnNumbers.at(drawnNumberCounter-1) << " Turns needed to win: " << drawnNumberCounter << endl;
                    
                    if (bestDrawnNumberCounter < drawnNumberCounter && bestDrawnNumberCounter != 0) {
                        //do nothing because the amount of drawnNumbers is more than our quickest winning board
                    }
                    else {
                        bestDrawnNumberCounter = drawnNumberCounter;
                        bestBoardFinalScore = currentBoardFinalScore;
                    }
                    currentRow = 0;
                    cout << "Best drawnNumbers: " << bestDrawnNumberCounter << " Best board score: " << bestBoardFinalScore << endl;
                }
                else {
                    currentRow++;
                }
            }
            else {
                while (getline(check1, intermediate, ',')) {
                    drawnNumbers.push_back(stoi(intermediate));

                }
            }
        }
    myFile.close();
    }
    

    cout << "Best score: " << bestBoardFinalScore;
    return 0;
}