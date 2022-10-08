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

    while (temp > 1) {
        int lastDigit = fmod(temp, 10);
        temp = temp / 10;

        decimalValue += lastDigit * base;
        base = base * 2;
    }

    return decimalValue;

}
int main()
{

    fstream myFile("input.txt");
    const int BINARYSIZE = 12;
    const int ROWS = 1000;

    int matrix[ROWS][BINARYSIZE] = { 0 };
    int matrixRow = 0;

    double gammaRate = 0;
    double epsilonRate = 0;

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
            //cout << "row: " << matrixRow << endl;
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

    /*
        For part two, we need to find the our life support rating which is oxygen generator rating * CO2 scrubber rating.
        The oxygen gen rating will be the final binaryString that consists of the most common binaryString elements.

        example:
        11110
        10110
        10111
        10101
        11100
        00010
        00100
        01111

        The first most common element 1, meaning we retain the entire list.
        the second most common element is 0, so we retain only the entries that start with 10xxx
        10110
        10111
        10101

        The next will be 1
        and 1 again, leaving only two remaining

        10110
        10111

        The oxygen rating will be the larger of the two, so 10111 is our solution.

        for C02 rating, we do the same thing, but instead of most common it is the least common.
        so our solution would be 01111 for the C02 rating.

        solution:

        1. we will be iterating through our matrix, adding together the first index of every array to determine
        the most common bit.
        2. Once we have found the most common bit, We will iterate through the matrix again extracting
        every ROW that contains that most common bit.
        3.Now we will do the same two things again for the remaining 10 digits until two rows remain, we are not including
        the final digit yet.
        4. select the final binary string for the oxygen rating
        5. do step 1-4 again but instead of the most common find the least common for the CO2 rating
    */
    vector<int> rowNumbersForCommon;
    vector<int> rowNumbersForLeast;
    int onesCounter = 0;
    int commonDigit = 0;
    double oxygenRating = 0;
    double co2Rating = 0;
    int positionInMatrix = 0;
    int rounding = 0;

    //1.
    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][0] == 1) {
            ++onesCounter;
        }
    }

    //2. 
    if (onesCounter >= ROWS / 2) {
        commonDigit = 1;
    }
    else {
        commonDigit = 0;
    }

    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][0] == commonDigit) {
            rowNumbersForCommon.push_back(i);
        }
        else {
            //do nothing
        }
    }

    //3.

    for (int i = 1; i <= 10; ++i) {
        //reset onesCounter
        onesCounter = 0;
        positionInMatrix = 0;

        for (int j = 0; j < rowNumbersForCommon.size(); ++j) {      //finding the common digit through ones
            positionInMatrix = rowNumbersForCommon.at(j);
            if (matrix[positionInMatrix][i] == 1) {
                ++onesCounter;
            }
            else {
                //do nothing
            }
        }
        
        if (rowNumbersForCommon.size() % 2 == 1) {
            rounding = rowNumbersForCommon.size() -1;
        }
        else {
            rounding = rowNumbersForCommon.size();
        }

        if (onesCounter > rounding / 2) {      //seeing if the common digit is 1 or 0
            commonDigit = 1;
        }
        else {
            commonDigit = 0;
        }

        cout << "Most commonDigit: " << commonDigit << " Current column: " << i << endl;
        for (int j = 0; j < rowNumbersForCommon.size(); ++j) {             //iteration through the recorded common values
            positionInMatrix = rowNumbersForCommon.at(j);

            if (matrix[positionInMatrix][i] == commonDigit) {     //if the value is common, keep it, if not pop out.

            }
            else {

                rowNumbersForCommon.erase(rowNumbersForCommon.begin() + j);
                --j;
            }
        }

        if (rowNumbersForCommon.size() == 2) {
            break;
        }
    }

    //print what remains from our searching and validate that it is two binary strings:
    cout << "rowNumbers contains:" << endl;
    for (unsigned i = 0; i < rowNumbersForCommon.size(); ++i) {

        for (int j = 0; j < 12; ++j) {
            positionInMatrix = rowNumbersForCommon.at(i);
            cout << matrix[positionInMatrix][j] << " ";
        }

        cout << endl;
    }
    
    //4.
    //convert the binary string into a double and compare
    double temp = 0;
    for (unsigned i = 0; i < rowNumbersForCommon.size(); ++i) {
        temp = 0;

        for (int j = 0; j < 12; ++j) {
            positionInMatrix = rowNumbersForCommon.at(i);
            int currentDigit = matrix[positionInMatrix][j];
            
            if (currentDigit == 1) {
                temp += (1 * pow(10, ((BINARYSIZE - 1) - j)));
            }
            else {
            }
        }

        if (temp > oxygenRating) {
            oxygenRating = temp;
        }
    }
    int oxygenRatingDecimal = binaryToDecimal(oxygenRating);
    cout << "OxygenRating: " << oxygenRatingDecimal << endl;

    //5
    int leastcommonDigit = 0;
    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][0] == 1) {
            ++onesCounter;
        }
    }

    if (onesCounter <= ROWS / 2) {
        leastcommonDigit = 1;
    }
    else {
        leastcommonDigit = 0;
    }

    for (int i = 0; i < ROWS; ++i) {
        if (matrix[i][0] == leastcommonDigit) {
            rowNumbersForLeast.push_back(i);
        }
        else {
            //do nothing
        }
    }

    //reiterating through the rest of the binarystrings
    for (int i = 1; i <= 10; ++i) {
        //reset onesCounter
        onesCounter = 0;
        positionInMatrix = 0;

        for (int j = 0; j < rowNumbersForLeast.size(); ++j) {      //finding the common digit through ones
            positionInMatrix = rowNumbersForLeast.at(j);

            if (matrix[positionInMatrix][i] == 1) {
                ++onesCounter;
            }
            else {
                //do nothing
            }
        }

        if (rowNumbersForLeast.size() % 2 == 1) {
            rounding = rowNumbersForLeast.size() + 1;
        }
        else {
            rounding = rowNumbersForLeast.size();
        }

        if (onesCounter < (rounding / 2)) {      //seeing if the common digit is 1 or 0
            leastcommonDigit = 1;
        }
        else {
            leastcommonDigit = 0;
        }

        cout << "least commonDigit: " << leastcommonDigit << " Current column: " << i << " onesCounter: " << onesCounter << " listSize:" << rowNumbersForLeast.size() << endl;

        for (int j = 0; j < rowNumbersForLeast.size(); ++j) {             //iteration through the recorded values
            positionInMatrix = rowNumbersForLeast.at(j);

            if (matrix[positionInMatrix][i] == leastcommonDigit) {     //if the value is the uncommon, keep it, if not pop out.

            }
            else {

                rowNumbersForLeast.erase(rowNumbersForLeast.begin() + j);
                --j;
            }
        }
        
        if (rowNumbersForLeast.size() <= 2) {
            break;
        }
    }

    //print what remains from our searching and validate the strings:
    cout << "rowNumbers contains:" << endl;
    for (unsigned i = 0; i < rowNumbersForLeast.size(); ++i) {

        for (int j = 0; j < 12; ++j) {
            positionInMatrix = rowNumbersForLeast.at(i);
            cout << matrix[positionInMatrix][j] << " ";
        }

        cout << endl;
    }

    //comparing the final two
    for (unsigned i = 0; i < rowNumbersForLeast.size(); ++i) {
        temp = 0;

        for (int j = 0; j < 12; ++j) {
            positionInMatrix = rowNumbersForLeast.at(i);
            int currentDigit = matrix[positionInMatrix][j];

            if (currentDigit == 1) {
                temp += (1 * pow(10, ((BINARYSIZE - 1) - j)));
            }
            else {
            }
        }

        if (temp < co2Rating || co2Rating == 0 ) {
            co2Rating = temp;
        }
    }

    int co2RatingDecimal = binaryToDecimal(co2Rating);
    cout << "co2Rating: " << co2RatingDecimal << endl;

    cout << "Life Support rating: " << (co2RatingDecimal * oxygenRatingDecimal) << endl;

    return 0;
}