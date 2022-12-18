/**
 * @name    Matrix_Multiply_Pthreads.cpp
 * @author  B929164
 * @date    18/12/2022
 * @brief   This program will multiply two matrices using pthreads. It can print the 
 *          result if PRINT_MODE is defined. The matrices are generated randomly and 
 *          are an integer type. The size can be specified using arguments, -A and -B.
 *          
 *              e.g ./Matrix_Multiply_Pthreads -A 100x100 -B 100x100 -T 8
 *          
 *          The -T argument specifies the number of threads to use.
 * 
 *          It achieves this by dividing the matrix into equal parts and assigning 
 *          each part to a thread. The threads then multiply the assigned parts and
 *          store the result in a global array.
*/




/* ################### [Includes] ################### */



#include <iostream>     // for cout, endl
#include <chrono>       // for timing
#include <pthread.h>    // for pthreads

using namespace std;


/* ################### [Defines] ################### */



//#define PRINT_MODE  // comment out to disable printing


/* ################### [Main] ################### */

int main(int argc, char *argv[])
{
    int A = 0;
    int B = 0;
    int T = 0;
    int A_Columns, A_Rows, B_Columns, B_Rows, Num_T = 0;

    // Loop over argv to see if "-A" and "-B" and "-T" are present then extract the value
    for (int i = 0; i < argc; i++)
    {
        if (string(argv[i]) == "-A")
        {
            A = i+1;
        }

        if (string(argv[i]) == "-B")
        {
            B = i+1;
        }

        if (string(argv[i]) == "-T")
        {
            T = i+1;
        }
    }

    // If "-A" and "-B" and "-T" are not present then exit
    if (A == 0 || B == 0 || T == 0)
    {
        cerr << "Please enter the size of the matrices and the number of threads to use" << endl;
        cerr << "e.g ./Matrix_Multiply_Pthreads -A 100x100 -B 100x100 -T 8" << endl;
        return 0;
    }

    // Tries to extract the values from the arguments into separate variables
    try
    {
        // Spliting on the "x" character and converting the values to integers
        A_Columns = stoi(string(argv[A]).substr(0, string(argv[A]).find("x")));
        A_Rows = stoi(string(argv[A]).substr(string(argv[A]).find("x")+1, string(argv[A]).length()));

        // Spliting on the "x" character and converting the values to integers
        B_Columns = stoi(string(argv[B]).substr(0, string(argv[B]).find("x")));
        B_Rows = stoi(string(argv[B]).substr(string(argv[B]).find("x")+1, string(argv[B]).length()));

    }
    catch(exception& e)
    {
        cerr << "Error: Please enter the dimensions of the matrices in the correct format (Columns x Rows)" << endl;
        cerr << "Example: ./Matrix_Multiply_Serial -A 3x3 -B 3x3" << endl;
        return 0;
    }

    // Tries to extract the number of threads to use from the arguments into a variable
    try
    {
        Num_T = stoi(argv[T]);
    }
    catch(exception& e)
    {
        cerr << "Error: Please enter the correct number of threads to use" << endl;
        cerr << "Example: ./Matrix_Multiply_Serial -A 3x3 -B 3x3 -T 8" << endl;
        return 0;
    }

    // If the number of columns in the first matrix is not equal to the number of rows in the second matrix then print error message and exit
    if (A_Columns != B_Rows)
    {
        cerr << "Error: The number of columns in the first matrix must be equal to the number of rows in the second matrix" << endl;
        cerr << "Can not multiply" << endl;
        return 0;
    }

}