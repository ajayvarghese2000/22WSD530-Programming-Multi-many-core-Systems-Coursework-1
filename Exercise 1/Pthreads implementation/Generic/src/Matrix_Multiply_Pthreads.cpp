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
 *          store the result.
 * 
 *          This version of the program can use any size of matrix that can be multiplied
 *          together as long as the number of columns in matrix A is equal to the number
 *          of rows in matrix B. (Conventional matrix multiplication rules)
*/



/* ######################## [Includes] ######################## */

#include <iostream>     // for cout, endl
#include <chrono>       // for timing
#include <pthread.h>    // for pthreads

using namespace std;

/* ######################## [Defines] ######################## */

//#define PRINT_MODE  // comment out to disable printing

/* ################### [Global Variables] #################### */

// Double Pointer to each matrix that will be set in main
int **pMatrix_A;
int **pMatrix_B;
int **pMatrix_C;

// The number of columns and rows in each matrix and the number of threads to use
int A_Columns, A_Rows, B_Columns, B_Rows, Num_T = 0;    // These will be set in main

// The Threads inputs, telling each thread what part of the matrix to multiply
struct Pthreads_Input
{   
    // The start and end row of the matrix that the thread will multiply
    int Start_Row;
    int End_Row;
};

/**
 * @brief   This is the function that each thread will run. It will multiply the
 *          assigned parts of the matrices and store the result in matrix c using
 *          the global pointer pMatrix_C.
 * @param   input   A pointer to a Pthreads_Input struct that contains the start
 *          and end row of the matrix that the thread will multiply.
 * 
 * @return  NULL
 */
void *Matrix_Multiply(void *input)
{
    // Cast the input to a Pthreads_Input struct
    Pthreads_Input *pInput = (Pthreads_Input*)input;

    /* Preform the multiplication using the same method as the serial version
       but only for the assigned rows */
    for (int i = pInput->Start_Row; i < pInput->End_Row; i++)
    {
        for (int j = 0; j < B_Columns; j++)
        {
            for (int k = 0; k < A_Columns; k++)
            {
                pMatrix_C[i][j] += pMatrix_A[i][k] * pMatrix_B[k][j];
            }
        }
    }

    return NULL;
}

/* ################### [Main] ################### */

/**
 * @brief   Main function, creates the matrices figures out how to split them
 *          between the threads and then runs the threads. It takes in arguments
 *          to specify the size of the matrices and the number of threads to use.
 *          The arguments are "-A" and "-B" and "-T".
 * 
 *              e.g ./Matrix_Multiply_Pthreads -A 100x100 -B 100x100 -T 8
 * 
 *          The -T argument specifies the number of threads to use.
 *          -A and -B specify the size of the matrices.
 * 
 * @param   argc The number of arguments passed to the program
 * @param   argv The arguments passed to the program
 * @return  int  0 if the program ran successfully
 */
int main(int argc, char *argv[])
{

    /* ################### [Driving Code] ################### */

    /* This section of code is not timed and used to set up the matrices */

    // Variables to store the index of the argument values
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

    /* ######################### [TIME CRITICAL ZONE START] ######################### */

    // Starting the timer
    auto start = chrono::high_resolution_clock::now();

    // Calculating the number of rows each thread will be responsible for
    int Rows_Per_Thread = A_Rows / Num_T;

    // Check if the rows per thread is 0 (Happens when the number of threads is greater than the number of rows)
    if (Rows_Per_Thread == 0)
    {
        // If the rows per thread is 0 then set it to 1
        Rows_Per_Thread = 1;

        // Set the number of threads to the number of rows
        Num_T = A_Rows;
    }

    // Creating the threads
    pthread_t threads[Num_T];

    // Creating the thread inputs array, one for each thread
    Pthreads_Input *thread_input = new Pthreads_Input[Num_T];

    // Creating the threads and setting the thread inputs
    for (int i = 0; i < Num_T; i++)
    {
        // Setting the start row for the matrix
        thread_input[i].Start_Row = (i * Rows_Per_Thread);

        
        // Setting the end row for the matrix
        thread_input[i].End_Row = (i + 1) * Rows_Per_Thread;

        // Check if this is the last thread
        if (i == Num_T - 1)
        {
            // If this is the last thread then set the end row to the number of rows in the first matrix
            thread_input[i].End_Row = A_Rows;
        }

        // Creating the thread and checking for errors
        if (pthread_create(&threads[i], NULL, Matrix_Multiply, (void *)&thread_input[i]) != 0)
        {
            // Printing the error message and terminating the program if a thread could not be created
            cerr << "Error: Could not create thread" << endl;
            return 0;
        }

    }

    // Waiting for the threads to finish
    for (int i = 0; i < Num_T; i++)
    {
        pthread_join(threads[i], NULL);
    }

    // Stopping the timer
    auto stop = chrono::high_resolution_clock::now();

    /* ######################### [TIME CRITICAL ZONE END] ######################### */

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

    // Freeing the memory allocated for the matrices
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