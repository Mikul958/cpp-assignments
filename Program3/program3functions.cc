// Copyright 2023 mpikula

#include <iostream>
using std::cout;
using std::endl;

#include "program3functions.h"

// ====================== FOR TESTING, DELETE AFTER FINISHED =====================
int main()
{
    double testArray[][10] = {
        {1.2, 8.7, 4.1, 6.7, 7.1, 0.7, 0.3, 9.4, 6.4, 5.2},
        {2.9, 2.4, 2.3, 2.1, 1.9, 3.4, 0.6, 1.8, 1.7, 2.2},
        {5.7, 8.7, 2.3, 7.2, 3.3, 2.1, 1.6, 4.4, 5.5, 6.6},
        {0.5, 3.5, 4.1, 1.6, 2.5, 3.9, 0.5, 1.8, 5.6, 5.2},
        };

    cout << CountAboveAv(testArray, 4) << endl;

    SortByCol(testArray, 4, 6, true);
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<10; j++)
        {
            cout << testArray[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl;

    SortByRow(testArray, 4, 1, false);
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<10; j++)
        {
            cout << testArray[i][j] << " ";
        }
        cout << endl;
    }
}

int CountAboveAv(double array[][10], int rows)
{
    
    // Compute the average of all elements in the array.
    double total = 0;
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<10; j++)
            total += array[i][j];
    }
    double average = total / (rows * 10);

    // Count the number of elements in the array above the average.
    int countAbove = 0;
    for (int i=0; i<rows; i++)
    {
        for (int j=0; j<10; j++)
        {
            if (array[i][j] > average)
                countAbove++;
        }
    }
    return countAbove;
}

void SortByCol(double array[][10], int num_rows, int col, bool ascending)
{
    if (col >= 10)
        return;
    
    // Using selection sort, i is pivot row for each loop.
    for (int i=0; i<num_rows-1; i++)
    {
        // Row to be swapped with i; min if ascending, max if descending
        int swap = i;

        // Find min (or max) value in column after row i, store index to swap.
        for (int j=i+1; j<num_rows; j++)
        {
            // Makeshift XOR logic, may be a better way to write this.
            if ((array[j][col] < array[swap][col] && ascending) ||
                (array[j][col] > array[swap][col] && !ascending))
                swap = j;
        }
        // Swap all elements in row i and row "swap".
        for (int k=0; k<10; k++)
        {
            double temp = array[i][k];
            array[i][k] = array[swap][k];
            array[swap][k] = temp;
        }
    }
}

void SortByRow(double array[][10], int num_rows, int row, bool ascending)
{
    if (row >= num_rows)
        return;

    for (int i=0; i<9; i++)
    {
        // Column to be swapped with i.
        int swap = i;

        for (int j=i+1; j<10; j++)
        {
            if ((array[row][j] < array[row][swap] && ascending) ||
                (array[row][j] > array[row][swap] && !ascending))
                swap = j;
        }

        for (int k=0; k<num_rows; k++)
        {
            double temp = array[k][i];
            array[k][i] = array[k][swap];
            array[k][swap] = temp;
        }
    }
}

double MediumInCol(double array[][10], int num_rows, int col)
{
    // Create new array[] and slot in values from specified column, then sort them.
    // If odd number of rows, median is value at num_rows/2
    // If even number of rows, median is average of values at (num_rows/2) - 1 and num_rows/2
}

double ModeInCol(double array[][10], int num_rows, int col)
{
    // Create new array[] and slot in values from specified column, then sort them.
    // Run through numbers. Keep track of current number, add to "amount of current" for each of the same number in array.
    // After a new number shows up, compared "amount of current" with "amount of mode"
        // If higher, set number equal to new mode and "amount of mode" equal to "amount of current".
}