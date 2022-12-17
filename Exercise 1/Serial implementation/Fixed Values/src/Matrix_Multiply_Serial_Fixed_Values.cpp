/**
 * @name    Matrix_Multiply_Serial_Fixed_Values.cpp
 * @author  B929164
 * @date    17/12/2022
 * @brief   This program multiplies two matrices of fixed values that it reads from a file
 *          found in the include directory. The program can then prints the result to the 
 *          console if that option is selected. It use the simple iterative algorithm to 
 *          multiply the matrices. This program is a serial version of the program and
 *          therefore does not use any parallelism and is limited to one thread.
*/

// The size of the matrices 
#define MATRIX_SIZE 1000    // (this can only be 4 or 1000, will default to 1000 matrix if anything else is entered)
#define Timer               // (this will enable the timer, comment out to disable)

/* ################### [Includes] ###################*/
#include <iostream>                         // For cout, cin, endl
#include "../includes/Matrix_Values.h"      // For Matrix Values
#ifdef Timer
    #include <chrono>                       // For timer
#endif

using namespace std;


/* ################### [Globals] ###################*/
int matrix_Result[MATRIX_SIZE][MATRIX_SIZE];    // The matrix that will store the result of the multiplication


/* ################### [Functions] ###################*/

/**
 * @brief   This function multiplies two matrices together and stores the result in a third matrix
 * 
 * @param   matrix_A    The first matrix to be multiplied
 * @param   matrix_B    The second matrix to be multiplied
 * @param   matrix_C    The matrix that will store the result of the multiplication
 * 
 * @return  void
*/
void matrix_Multiply(int matrix_A[MATRIX_SIZE][MATRIX_SIZE], int matrix_B[MATRIX_SIZE][MATRIX_SIZE], int matrix_C[MATRIX_SIZE][MATRIX_SIZE])
{
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            for(int k = 0; k < MATRIX_SIZE; k++)
            {
                matrix_C[i][j] = matrix_C[i][j] + (matrix_A[i][k] * matrix_B[k][j]);
            }
        }
    }
}

/**
 * @brief   This function prints the matrix to the console
 * 
 * @param   matrix_A    The matrix to be printed
 * 
 * @return  void
*/
void print_Matrix(int matrix_A[MATRIX_SIZE][MATRIX_SIZE])
{
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            cout << matrix_A[i][j] << " ";
        }
        cout << endl;
    }
}

/**
 * @brief This function validates the matrix multiplication by comparing the 
 *        result to the expected result
 * 
 * @param matrix        The matrix to be validated
 * @param matrix_ANS    The expected result
 * 
 * @return true         If the matrix is correct
 * @return false        If the matrix is incorrect
 * 
 */
bool validate_Matrix(int matrix[MATRIX_SIZE][MATRIX_SIZE], int matrix_ANS[MATRIX_SIZE][MATRIX_SIZE])
{
    for(int i = 0; i < MATRIX_SIZE; i++)
    {
        for(int j = 0; j < MATRIX_SIZE; j++)
        {
            if(matrix[i][j] != matrix_ANS[i][j])
            {
                return false;
            }
        }
    }

    return true;
}


/* ################### [Main] ###################*/

int main()
{   
    
    // Start the timer
    #ifdef Timer
        auto start = chrono::high_resolution_clock::now();
    #endif

    // Multiply the matrices
    #if MATRIX_SIZE == 4
        matrix_Multiply(Matrix_4x4_A, Matrix_4x4_B, matrix_Result);
    #else
        matrix_Multiply(Matrix_1000x1000_A, Matrix_1000x1000_B, matrix_Result);
    #endif

    // Stop the timer
    #ifdef Timer
        auto stop = chrono::high_resolution_clock::now();
        auto duration = chrono::duration_cast<chrono::microseconds>(stop - start);
        cout << "Time taken : " << duration.count() << " uS" << endl;
        cout << endl;
    #endif

    // Only print the result if Timer is not defined
    #ifndef Timer

        // Print the result to the console only if the matrix size is 4
        #if MATRIX_SIZE == 4

            cout << "The result is:" << endl;
            print_Matrix(matrix_Result);
            cout << endl;

            cout << "Matrix A is:" << endl;
            print_Matrix(Matrix_4x4_A);
            cout << endl;

            cout << "Matrix B is:" << endl;
            print_Matrix(Matrix_4x4_B);
            cout << endl;

        #endif

    #endif

    // Only validate the result if Timer is not defined
    #ifndef Timer

        // Validate the result
        #if MATRIX_SIZE == 4

            if(validate_Matrix(matrix_Result, Matrix_4x4_ANS))
            {
                cout << "The result is correct" << endl;
            }
            else
            {
                cout << "The result is incorrect" << endl;
            }
        #else

            if(validate_Matrix(matrix_Result, Matrix_1000x1000_ANS))
            {
                cout << "The result is correct" << endl;
            }
            else
            {
                cout << "The result is incorrect" << endl;
            }

        #endif

    #endif
    
    
    return 0;
}