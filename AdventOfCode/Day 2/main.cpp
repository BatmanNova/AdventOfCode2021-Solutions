#include <iostream>
#include <fstream>

using namespace std;

int main()
{
    int up = 0;
    int down = 0;
    int forward = 0;


    ifstream myfile ("input.txt");
    
    if (myfile.is_open()){
        string line;
        int stringLength, movement;
        
        /*Part one: 
            inputs possibe are:
            Up X
            Down X
            Forward X

            These have fixed lengths due so we are going to tokenize the strings
            based upon that.
            
            
            if the line is 4 characters, it is up (decreasing the depth)
            elif 6 characters, it is down (increasing the depth)
            elif 9 characters, it is forward

            now that we know the directions, we just need to increase their corresponding
            values accordingly. 

            With up, we will be cutting off the first 3 characters, so up(space) and
            leaving the lone digit which we will add to the direction counter.

            Finally the problem asks
        */
        while (getline(myfile, line)){
            stringLength = line.size();

            if (stringLength == 4){
                up += stoi(line.substr(3,1));

            } else if (stringLength == 6){
                down += stoi(line.substr(5,1));

            } else if (stringLength == 9){
                forward += stoi(line.substr(8,1));

            } else {
                cout << endl << "unexpected input." << endl;
            }
        }
    }

    cout << "Horizontal position: " << (forward * (down - up)) ;
    
    return 0;
}