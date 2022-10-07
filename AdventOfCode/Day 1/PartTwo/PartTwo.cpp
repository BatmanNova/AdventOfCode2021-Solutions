#include <iostream>
#include <fstream>
#include <vector>
using namespace std;

int main () {
  
    ifstream myfile;
    myfile.open ("input.txt");
    
    // Variables for array of inputs, length obtained from input.txt
    int myArr[2001];
    int increased = 0; 
    int tempsum;
    int currentMeasurement;
    int currentPos;

    // positions for the 3 inputs that the windows have
    int windowPos1 = 1;
    int windowPos2 = 2;
    int windowPos3 = 3;
    
    //Variables used for calculating sum of increased window sizes
    int windowSum;
    int currentWindow;
    int windowToCompare;

    //vector that will contain windows measurements
    vector<int> windows;

    if (myfile.is_open()) { //reading in the file, storing in array

        while (myfile >> currentMeasurement) {
            if(currentMeasurement == 0){ 
            } else {
                myArr[currentPos] = currentMeasurement;
                currentPos++;
            }
        }

        myfile.close();
    }

    //Calculating the sum of the current windows measurements, and adding to vector
    for(int i = 0; i < 2000 - windowPos3; i++){
        windowSum = myArr[i + windowPos1] + myArr[i + windowPos2] + myArr[i + windowPos3]; 
        windows.push_back(windowSum);
    }

    //Looping through vector and incrementing sum when window size is increased.
    for(int i = 0; i < windows.size(); i++){
        if(windows[i] == 0){
            currentWindow = windows[i];
        }

        if(windows[i] > currentWindow){
            increased++;
        }
        currentWindow = windows[i];
    }
    cout << "increased: " << increased << endl;
    return 0;
}