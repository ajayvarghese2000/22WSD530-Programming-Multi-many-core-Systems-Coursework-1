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
//#include "../includes/Matrix_Values.h"

using namespace std;


/* ################### [Defines] ################### */



//#define PRINT_MODE  // comment out to disable printing


/* ################### [Global Variables] ################### */

// Double Pointer to each matrix that will be allocated memory in main
int **pMatrix_A;
int **pMatrix_B;
int **pMatrix_C;

// The number of columns and rows in each matrix and the number of threads to use
int A_Columns, A_Rows, B_Columns, B_Rows, Num_T = 0;    // These will be set in main

int Row_Num = 0;    // This will be used to keep track of the row number

// Mutex for the row number
pthread_mutex_t Row_Num_Mutex = PTHREAD_MUTEX_INITIALIZER;



/**
 * @brief   This is the function that each thread will run. It will multiply the
 *          assigned parts of the matrices and store the result in matrix c.
 * 
 */
void *Matrix_Multiply(void *a)
{
    while(1)
    {
        // Lock the mutex
        pthread_mutex_lock(&Row_Num_Mutex); 

        // Get which row to start on
        int Row = Row_Num++;

        // Unlock the mutex
        pthread_mutex_unlock(&Row_Num_Mutex);
        
        // Check if the row is out bigger than the number of rows resultant matrix has
        if (Row >= A_Rows)
        {
            // Exit the thread
            pthread_exit(NULL);
        }

        // Preform the multiplication on current row
        for (int i = 0; i < B_Columns; i++)
        {
            for (int j = 0; j < A_Columns; j++)
            {
                pMatrix_C[Row][i] += pMatrix_A[Row][j] * pMatrix_B[j][i];
            }
        }
    }

    return NULL;
}


/* ################### [Main] ################### */

int main(int argc, char *argv[])
{
    int A = 0;
    int B = 0;
    int T = 0;

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

    // Initializing the C matrix to 0
    for (int i = 0; i < A_Rows; i++)
    {
        for (int j = 0; j < B_Columns; j++)
        {
            Matrix_C[i][j] = 0;
        }
    }

    // Coping Matrix_4x4_A and Matrix_4x4_B to the matrices A and B
    // for (int i = 0; i < 4; i++)
    // {
    //     for (int j = 0; j < 4; j++)
    //     {
    //         Matrix_A[i][j] = Matrix_4x4_A[i][j];
    //         Matrix_B[i][j] = Matrix_4x4_B[i][j];
    //     }
    // }

    // Setting up the global pointers to the matrices
    pMatrix_A = Matrix_A;
    pMatrix_B = Matrix_B;
    pMatrix_C = Matrix_C;

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

    // Check if there are more threads than rows in the result matrix
    if (Num_T > A_Rows)
    {   
        // Set the number of threads to the number of rows in the result matrix
        Num_T = A_Rows;
    }

    // Creating the threads
    pthread_t threads[Num_T];
    
    // Creating the threads and setting the thread inputs
    for (int i = 0; i < Num_T; i++)
    {
        // Creating the thread
        pthread_create(&threads[i], NULL, Matrix_Multiply, NULL);

    }

    // Waiting for the threads to finish
    for (int i = 0; i < Num_T; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Stopping the timer
    auto stop = chrono::high_resolution_clock::now();

    // Calculating the time taken
    auto duration = chrono::duration_cast<chrono::nanoseconds>(stop - start);

    // Printing the time taken
    cout << duration.count() << " nS" << endl;

    // Printing the output if the PRINT_MODE is enabled
    #ifdef PRINT_MODE

        // Printing the matrices to the console
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

    // // Freeing the memory
    // for (int i = 0; i < A_Rows; i++)
    // {
    //     delete[] Matrix_A[i];
    // }

    // for (int i = 0; i < B_Rows; i++)
    // {
    //     delete[] Matrix_B[i];
    // }

    // for (int i = 0; i < A_Rows; i++)
    // {
    //     delete[] Matrix_C[i];
    // }

    // delete[] Matrix_A;
    // delete[] Matrix_B;
    // delete[] Matrix_C;

    return 0;
}