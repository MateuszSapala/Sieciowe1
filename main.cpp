#include <stdio.h>
#include <stdlib.h>
#include <cstdlib>
#include <unistd.h>
#include <iostream>
#include <fstream>
#include <sys/wait.h>

using namespace std;

const int RANGE_MIN = 1;
const int RANGE_MAX = 10;

int getRandom() {
    return (rand() % RANGE_MAX)+RANGE_MIN;
}

void printBreak() {
    cout << "---------------------------------------\n";
}

void write(int value, int i, int j) {
    ofstream file;
    string fileName = to_string(i) + "-" + to_string(j) + ".txt";
    file.open(fileName);
    file << value;
    file.close();
}

int read(int i, int j) {
    string fileName = to_string(i) + "-" + to_string(j) + ".txt";
    ifstream file(fileName);
    string line;
    if (file.is_open()) {
        getline(file, line);
        file.close();
        remove(fileName.c_str());
        return atoi(line.c_str());
    }
    file.close();
    return 0;
}

void matrixMultiplication() {
    // Get the number of rows and columns of the first matrix from the user
    cout << "Number of rows of first matrix: ";
//    int aRows=3;
    int aRows;
    cin>>aRows;
    cout << "Number of columns of first matrix: ";
//    int aCols=2;
    int aCols;
    cin>>aCols;
//    int aMatrix[3][2] = {{1, 4}, {2, 5}, {3, 6}};
    int aMatrix[aRows][aCols];
    printBreak();

    // Print first matrix
    cout << "First:\n";
    for (int i = 0; i < aRows; i++) {
        for (int j = 0; j < aCols; j++) {
            aMatrix[i][j] = getRandom();
            cout << aMatrix[i][j] << "\t";
        }
        cout << "\n";
    }
    printBreak();

    // Get the number of columns of the second matrix from the user
    int bRows = aCols;
    cout << "Number of columns of second matrix: ";
//    int bCols=3;
    int bCols;
    cin>>bCols;
//    int bMatrix[2][3] = {{7, 8, 9}, {10, 11, 12}};
    int bMatrix[bRows][bCols];
    printBreak();

    // Print second matrix
    cout << "Second:\n";
    for (int i = 0; i < bRows; i++) {
        for (int j = 0; j < bCols; j++) {
            bMatrix[i][j] = getRandom();
            cout << bMatrix[i][j] << "\t";
        }
        cout << "\n";
    }
    printBreak();

    // Create children and compute in them calculations
    int resultRows = aRows;
    int resultCols = bCols;
    int result[resultRows][resultCols];
    pid_t pids[resultRows][resultCols]; // Stores children's PIDs
    for (int row = 0; row < resultRows; row++) {
        for (int col = 0; col < resultCols; col++) {
            pid_t pid = fork();
            pids[row][col] = pid;
            if (pid == 0) {
                // Child
                int value = 0;
                for (int inner = 0; inner < aCols; inner++) {
                    value += aMatrix[row][inner] * bMatrix[inner][col];
                }
//                sleep(row*col); // Simulate long calculations
                write(value, row, col);
                exit(0);
            }
        }
    }

    // Wait for the children and get the results
    for (int row = 0; row < resultRows; row++) {
        for (int col = 0; col < resultCols; col++) {
            cout << "Waiting for [" << row << "][" << col << "]-" << &pids[row][col] << " \n";
            int status;
            waitpid(pids[row][col], &status, 0);
            result[row][col] = read(row, col);
        }
    }
    printBreak();

    // Print the multiplied matrix
    for (int row = 0; row < resultRows; row++) {
        for (int col = 0; col < resultCols; col++) {
            printf("%d\t", result[row][col]);
        }
        printf("\n");
    }
}

int main(int argc, char** argv) {
    //    The program generates two matrices with any dimensions given by the user. 
    //    Then, for each of the calculated results in a given place in the matrix, 
    //    it generates a new process that returns the result of the calculation as the process completion status. 
    //    The program then waits for the results of the children's processes on the basis of which it completes the matrix. 
    //    Finally, the matrix is printed on the screen.

    //    If you want to simulate long computations, you can uncomment line 78 (with comment "Simulate long computations")
    matrixMultiplication();
    return (EXIT_SUCCESS);
}