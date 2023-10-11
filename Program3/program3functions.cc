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

    cout << "Average: " << CountAboveAv(testArray, 4) << endl;
    cout << "Median in Column 0: " << MedianInCol(testArray, 4, 0) << endl;

    double testOut[2] = {-1,-1};

    cout << ModeInCol(testArray, 4, 2, testOut) << " mode(s): ";

    cout << testOut[0] << " and " << testOut[1] << endl;

    cout << "Sorted by column 6 in ascending order:" << endl;
    SortByCol(testArray, 4, 6, true);
    for (int i=0; i<4; i++)
    {
        for (int j=0; j<10; j++)
        {
            cout << testArray[i][j] << " ";
        }
        cout << endl;
    }

    cout << endl << "Sorted by row 1 in descending order: " << endl;

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

double MedianInCol(double array[][10], int num_rows, int col)
{
    // Slot values from specified column into a single-dimensional array.
    double data[num_rows];
    for (int i=0; i<num_rows; i++)
        data[i] = array[i][col];
    
    // Selection sort array in ascending order.
    for (int i=0; i<num_rows-1; i++)
    {
        int min = i;
        for (int j=i; j<num_rows; j++)
        {
            if (data[j] < data[min])
                min = j;
        }
        double temp = data[i];
        data[i] = data[min];
        data[min] = temp;
    }

    // If odd number of rows, return exact middle value in set.
    if (num_rows%2 != 0)
        return data[num_rows/2];

    // If not, return average of middle two values in set.
    return (data[num_rows/2] + data[(num_rows/2)-1]) / 2;
}

double ModeInCol(double array[][10], int num_rows, int col, double out[2])
{   
    double mode1, mode2;
    int mode1_count = 1, mode2_count = 1;
    int num_modes = 0;

    double current_number = array[0][col];
    int count = 1;
    for (int i=0; i<num_rows; i++)
    {
        // For each element, loop through array and find all occurrences AFTER.
        double current_number = array[i][col];
        int count = 1;
        for (int j=i; j<num_rows; j++)
        {
            if (array[j][col] == current_number)
                count++;
        }

        // Set new mode and count of it, if applicable.
        if (count > mode1_count)            // New single mode
        {
            mode1 = current_number;
            mode1_count = count;
            num_modes = 1;
        }
        else if (count == mode1_count)      // Tied with current mode.
        {
            if (mode1_count == mode2_count) // There were already 2 modes.
            {
                num_modes = 3;
            }
            else
            {
                mode2 = current_number;
                mode2_count = count;
                num_modes = 2;
            }
        }

    }

    // Return number of modes in a column and add each mode to array out.
    // If more than 2 distinct modes, return 0 and add nothing.
    switch(num_modes)
    {
        case 1:
            out[0] = mode1;
            break;
        case 2:
            out[0] = mode1;
            out[1] = mode2;
            break;
        case 3:
            return 0;
    }
    return num_modes;
}