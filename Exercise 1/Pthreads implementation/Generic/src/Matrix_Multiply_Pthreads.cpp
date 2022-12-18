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



/* ################### [Matrix Multiply] ################### */

struct Matrix_Multiply_Pthreads_Input
{
    int **Matrix_A;
    int **Matrix_B;
    int **Matrix_C;
    int A_Rows;
    int A_Columns;
    int B_Rows;
    int B_Columns;
    int Start_Row;
    int End_Row;
};



/**
 * @brief   This is the function that each thread will run. It will multiply the
 *          assigned parts of the matrices and store the result in matrix c.
 * 
 */
void *Matrix_Multiply(void *input)
{
    // Extracting the input from the void pointer
    Matrix_Multiply_Pthreads_Input *Input = (Matrix_Multiply_Pthreads_Input*)input;

    // Looping over the rows of matrix a
    for (int i = Input->Start_Row; i < Input->End_Row; i++)
    {
        // Looping over the columns of matrix b
        for (int j = 0; j < Input->B_Columns; j++)
        {
            // Looping over the rows of matrix b
            for (int k = 0; k < Input->B_Rows; k++)
            {
                Input->Matrix_C[i][j] += Input->Matrix_A[i][k] * Input->Matrix_B[k][j];
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

    // Creating the threads
    pthread_t threads[Num_T];

    // Creating the thread input struct
    Matrix_Multiply_Pthreads_Input *thread_input = new Matrix_Multiply_Pthreads_Input[Num_T];

    // Loop over the number of threads
    for (int i = 0; i < Num_T; i++)
    {
        // Setting the pointers to the matrices
        thread_input[i].Matrix_A = Matrix_A;
        thread_input[i].Matrix_B = Matrix_B;
        thread_input[i].Matrix_C = Matrix_C;

        // Setting the dimensions of the matrices
        thread_input[i].A_Rows = A_Rows;
        thread_input[i].A_Columns = A_Columns;
        thread_input[i].B_Rows = B_Rows;
        thread_input[i].B_Columns = B_Columns;

        // Calculating the number of rows each thread will be responsible for
        int Rows_Per_Thread = A_Rows / Num_T;

        // Calculating the starting row for each thread
        thread_input[i].Start_Row = i * Rows_Per_Thread;

        // Calculating the ending row for each thread
        thread_input[i].End_Row = (i + 1) * Rows_Per_Thread;

        // If the thread is the last thread then it will be responsible for the remaining rows
        if (i == Num_T - 1)
        {
            thread_input[i].End_Row = A_Rows;
        }

        // Creating the thread
        pthread_create(&threads[i], NULL, Matrix_Multiply, (void *)&thread_input[i]);
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