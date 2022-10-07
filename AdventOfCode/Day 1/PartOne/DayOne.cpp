#include <iostream>
#include <fstream>
using namespace std;

int main () {
  
  ifstream myfile;
  myfile.open ("input.txt");

    int sum = 0; 
    int deepest = 0;
    int DepthCompare;

  if (myfile.is_open()) {
    while ( myfile >> DepthCompare ) {
        if (deepest == 0){ //checks if this is the first input 
            deepest = DepthCompare;        
        }
        else if( DepthCompare > deepest ){ // comparing, if DepthCompare > deepest, increment sum
            deepest = DepthCompare;
            sum++;
            cout << "Increasing: " << DepthCompare << endl;
        }
        else if( DepthCompare < deepest ){
            deepest = DepthCompare;
            cout << "Decreasing: " << DepthCompare << " sum: " << sum  << endl;
        }
    }
        cout << sum << endl;
        myfile.close();
    }
    return 0;
}