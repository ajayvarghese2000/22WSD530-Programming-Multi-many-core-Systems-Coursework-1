/**
 * @name    Matrix_Multiply_Serial.cpp
 * @author  B929164
 * @date    17/12/2022
 * @brief   This program multiplies two matrices. The program can then prints the
 *          result to the console if that option is selected. It use the simple 
 *          iterative algorithm to multiply the matrices. This program is a serial
 *          version of the program and therefore does not use any parallelism and 
 *          is limited to one thread. The values in the matrices are all intergers.
 *          
 *          First two matrices are created and then the values are assigned to
 *          the matrices. The matrices are then multiplied and the result is
 *          stored in a third matrix. The dimensions of the matrices are fixed and
 *          can be set in by passing in the required arguments.
*/



/* ################### [Includes] ###################*/



#include <iostream>     // For cout, cin, endl
#include <chrono>       // For timing the program

using namespace std;


/* ################### [Main] ###################*/



/**
 * @brief   This function will take in arguments and create two matrices with the
 *          dimensions specified by the arguments. The values in the matrices are
 *          then assigned. The matrices are then multiplied and the result is
 *          stored in a third matrix.
 * 
 */
int main(int argc, char *argv[])
{   

    int A = 0;
    int B = 0;
    int A_Columns, A_Rows, B_Columns, B_Rows = 0;

    // Loop over argv to see if "-A" and -"B" are present then extract the value
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
    }

    // If "-A" and "-B" are not present then print error message and exit
    if (A == 0 || B == 0)
    {
        cerr << "Error: Please include -A and -B arguments" << endl;
        cerr << "Example: ./Matrix_Multiply_Serial -A 3x3 -B 3x3" << endl;
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
    
    // If the number of columns in the first matrix is not equal to the number of rows in the second matrix then print error message and exit
    if (A_Columns != B_Rows)
    {
        cerr << "Error: The number of columns in the first matrix must be equal to the number of rows in the second matrix" << endl;
        cerr << "Can not multiply" << endl;
        return 0;
    }

    // Allocating memory for the matrices
    int **Matrix_A = new int*[A_Rows];
    int **Matrix_B = new int*[B_Rows];
    int **Matrix_C = new int*[A_Rows];

    for (int i = 0; i < A_Rows; i++)
    {
        Matrix_A[i] = new int[A_Columns];
    }

    for (int i = 0; i < B_Rows; i++)
    {
        Matrix_B[i] = new int[B_Columns];
    }

    for (int i = 0; i < A_Rows; i++)
    {
        Matrix_C[i] = new int[B_Columns];
    }

    // Filling up A and B with random values between 0 and 100
    for (int i = 0; i < A_Rows; i++)
    {
        for (int j = 0; j < A_Columns; j++)
        {
            Matrix_A[i][j] = rand() % 100;
        }
    }

    for (int i = 0; i < B_Rows; i++)
    {
        for (int j = 0; j < B_Columns; j++)
        {
            Matrix_B[i][j] = rand() % 100;
        }
    }


    // Printing the inputs if the PRINT_MODE is enabled
    #ifdef PRINT_MODE

        // Printing the matrices to the console
        cout << "Matrix A:" << endl;
        for (int i = 0; i < A_Rows; i++)
        {
            for (int j = 0; j < A_Columns; j++)
            {
                cout << Matrix_A[i][j] << " ";
            }
            cout << endl;
        }

        cout << endl;

        cout << "Matrix B:" << endl;
        for (int i = 0; i < B_Rows; i++)
        {
            for (int j = 0; j < B_Columns; j++)
            {
                cout << Matrix_B[i][j] << " ";
            }
            cout << endl;
        }

    #endif

    // Starting the timer
    auto start = chrono::high_resolution_clock::now();

    // Multiplying the matrices
    for (int i = 0; i < A_Rows; i++)
    {
        for (int j = 0; j < B_Columns; j++)
        {
            for (int k = 0; k < A_Columns; k++)
            {
                Matrix_C[i][j] == Matrix_C[i][j] + Matrix_A[i][k] * Matrix_B[k][j];
            }
        }
    }

    // Stopping the timer
    auto stop = chrono::high_resolution_clock::now();

    // Printing the result if the PRINT_MODE is enabled
    #ifdef PRINT_MODE

        // Printing the matrices to the console
        cout << endl;
        cout << "Matrix C:" << endl;
        for (int i = 0; i < A_Rows; i++)
        {
            for (int j = 0; j < B_Columns; j++)
            {
                cout << Matrix_C[i][j] << " ";
            }
            cout << endl;
        }
    
    #endif

    // Calculating the time taken
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    // Printing the time taken
    cout << duration.count() << " nS" << endl;

    // Freeing the memory
    for (int i = 0; i < A_Rows; i++)
    {
        delete[] Matrix_A[i];
    }

    for (int i = 0; i < B_Rows; i++)
    {
        delete[] Matrix_B[i];
    }

    for (int i = 0; i < A_Rows; i++)
    {
        delete[] Matrix_C[i];
    }

    delete[] Matrix_A;
    delete[] Matrix_B;
    delete[] Matrix_C;

    return 0;
}