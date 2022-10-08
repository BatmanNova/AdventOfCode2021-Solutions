#include <iostream>
#include <fstream>
#include <string>
#include <vector>
#include <cmath>
#include <chrono>

using namespace std;
using namespace std::chrono;

/*
        Need to find the gamma rate and epsilon rate of the binary numbers

        gamma rate is the MOST common bit in the corresponding position of ALL NUMBERS
        in our diagnostic report (input file).

        example:
        00100
        11110
        10110
        10111

        Our gamma rate would be per position:
        1  0  1  1  0   which is binary for 22.

        Epsilon rate is the LEAST common bit in the corresponding position for ALL NUMBERS
        in our diagnostic report.

        Example:
        00100
        11110
        10110
        10111

        Our gamma rate would be
        0 1 0 0 1 which is binary for 9.

        our input.txt has binary strings of 12 characters
    */
int binaryToDecimal(double n) {
    int decimalValue = 0;
    int base = 1;
    double temp = n;

    while (temp) {
        int lastDigit = fmod(temp, 10);
        temp = temp / 10;

        decimalValue += lastDigit * base;
        base = base * 2;
    }

    return decimalValue;

}
int main()
{

    fstream myFile("Text.txt");
    const int BINARYSIZE = 12;
    const int ROWS = 1000;

    int matrix[ROWS][BINARYSIZE] = { 0 };
    int matrixRow = 0;

    double gammaRate = 0;
    double epsilonRate = 0;

    auto start = high_resolution_clock::now();
    while (myFile.is_open()) {
        string line = "";
        double binaryString = 0;
        int binaryLength = 0;
        double temp = 0;

        /*
        We are going to populate a matrix with the corresponding binaryStrings

        example:
        BinaryString: 000100011010

        Goal:
            matrix: { 0, 0, 0, 1, 0, 0, 0, 1, 1, 0, 1, 0}

        How:
            int X = 100011010
            we need to find the length by dividing the int by 10 until there is no remainder.
            X / 10 (until no remainder) = 9 times

            That means our int is 9 integers in length, so the first 3 integers of the brinary string
            are 0's. So we now know that the first 3 elements of our matrix are 0, and where to start.
            We will then use modulus division in the same style to populate the elements.

            example:
            start: {0, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, ?}

            100011010 % 10 = 0
            {0, 0, 0, ?, ?, ?, ?, ?, ?, ?, ?, 0}

            10001101 % 10 = 1
            {0, 0, 0, ?, ?, ?, ?, ?, ?, ?, 1, 0}

            1000110 % 10 = 0
            {0, 0, 0, ?, ?, ?, ?, ?, ?, 0, 1, 0}
        */
        while (getline(myFile, line)) {
            binaryString = stod(line);
            binaryLength = 0;
            temp = binaryString;

            //Finding the length of the binaryString after conversion to double
            while (temp > 1) {
                temp = temp / 10;
                binaryLength++;
            }
            //resetting temp to be equal back to the binary string to not manipulate our control
            temp = binaryString;


            //populating the rest matrix row backwards
            double remainder = 0;
            for (int i = BINARYSIZE - 1; i >= (BINARYSIZE - binaryLength); --i) {
                remainder = fmod(temp, 10);
                temp = temp / 10;

                matrix[matrixRow][i] = (int)remainder;
            }

            //increasing our row position
            ++matrixRow;
            cout << "row: " << matrixRow << endl;
        }

        myFile.close();
    }

    /*
        Now that the matrix has been populated with the necessary binary strings
        we can find the gamma rate and epsilon rate by adding every element in a column and do a comparison
        to see if one is more common than zero.

        matrix sizes are 1000 rows, 12 columns

        reiterate:
        gamma rate is the MOST common bit in the corresponding position of ALL NUMBERS
        in our diagnostic report.

        Epsilon rate is the LEAST common bit in the corresponding position for ALL NUMBERS
        in our diagnostic report.

        if gamma rate is 1, epsilon is 0.
    */

    int commonValue = 0;
    int columnSum = 0;

    for (int i = 0; i < BINARYSIZE; ++i) {
        columnSum = 0;

        for (int j = 0; j < ROWS; ++j) {
            columnSum += matrix[j][i];
            cout << "columnSum: " << columnSum << "row: " << j << endl;
        }

        if (columnSum >= 500) {                                   //if the sum of the digits is more than 500, meaning its most common, add that digit to the corresponding position
            gammaRate += (1 * pow(10, ((BINARYSIZE - 1) - i)));
        }
        else {
            epsilonRate += (1 * pow(10, ((BINARYSIZE - 1) - i)));
        }
    }
    auto stop = high_resolution_clock::now();

    cout << "GammaRate: " << gammaRate << " EpsilonRate: " << epsilonRate << endl;
    int gammaDecimal = binaryToDecimal(gammaRate);
    int epsilonDecimal = binaryToDecimal(epsilonRate);

    cout << "gammaRate to decimal: " << gammaDecimal << " epsilonRate to decimal: " << epsilonDecimal << endl << "Power consumption: " << (gammaDecimal * epsilonDecimal) << endl;

    auto duration = duration_cast<milliseconds>(stop - start);
    cout << "First solution duration(milliseconds): " << duration.count() << endl;


    /*
    A potential idea for a solution was brought up to me discussion mine where we can add 1 if the binary is 1, -1 if it is 0 per binaryposition
    then in the end, if the value is > 0 it is a 1, if it is < 0 it is 0.
    */

    int newMatrix[12] = { 0 };
    fstream myFile2("Text.txt");

    start = high_resolution_clock::now();
    while (myFile2.is_open()) {
        string line = "";
        double binaryString = 0;
        int binaryLength = 0;
        double temp = 0;

        while (getline(myFile2, line)) {
            binaryString = stod(line);
            binaryLength = 0;
            temp = binaryString;

            int remainder = 0;
            for (int i = BINARYSIZE - 1; i >= 0; --i) {
                remainder = fmod(temp, 10);
                temp = temp / 10;

                if (remainder > 0) {
                    newMatrix[i] += 1;
                }
                else {
                    newMatrix[i] -= 1;
                }

            }

        }

        myFile2.close();
    }

    gammaRate = 0;
    epsilonRate = 0;

    for (int i = 0; i < 12; ++i) {
        if (newMatrix[i] > 0) {
            gammaRate += (1 * pow(10, ((BINARYSIZE - 1) - i)));
        }
        else {
            epsilonRate += (1 * pow(10, ((BINARYSIZE - 1) - i)));
        }
    }
    
    stop = high_resolution_clock::now();

    duration = duration_cast<milliseconds>(stop - start);


    cout << "GammaRate: " << gammaRate << " EpsilonRate: " << epsilonRate << endl;
    gammaDecimal = binaryToDecimal(gammaRate);
    epsilonDecimal = binaryToDecimal(epsilonRate);

    cout << "gammaRate to decimal: " << gammaDecimal << " epsilonRate to decimal: " << epsilonDecimal << endl << "Power consumption: " << (gammaDecimal * epsilonDecimal) << endl;

    cout << "Second solution duration(milliseconds): " << duration.count() << endl;


    /*
    This was a lot faster, but I do not retain the information needed for part two, so although it was creative I won't be using it further.
    */



    return 0;
}