<!-- This README is in markdown format, It is must viewed using a markdown viewer -->

<!-- This is because there will be pictures, gif and links embedded in the file to help explain the programs -->

<!-- If you do not have a markdown viewer please visit the flowing github link to  properly view the file in the intended format -->

<!-- https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%201 -->

<!-- This GitHub will be private until after the submission deadline -->

# Exercise 1 Matrix Multiplication using Pthreads

<p align="center">
	<a href="https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%201"><img src="https://i.imgur.com/lNPNcwZ.gif"></a>
    <p align="center"> Example 4x4 matrix being run with 4 threads</p>
</p>

----

## Table of Contents
- [Introduction](#introduction)
- [Folder Structure](#folder-structure)
- [Implementation](#implementation)
- [Build Guide](#build-guide)
    - [Pre-requisites](#pre-requisites)
    - [Running the programs](#running-the-program)
        - [Single Threaded Generic](#single-threaded)
        - [Single Threaded Fixed](#single-threaded-fixed)
        - [Multi Threaded Generic](#multi-threaded)
        - [Multi Threaded Fixed](#multi-threaded-fixed)
- [Experiment Setup](#experiment-setup)
- [Methodology](#methodology)
- [Results](#results)

----

## Introduction

This exercise aims to measure the performance of a matrix multiplication algorithm using pthreads. The algorithm is a simple matrix multiplication algorithm that uses a single thread to calculate each element of the resulting matrix. The algorithm is as follows:

```c++
for (int i = 0; i < N; i++) 
{
    for (int j = 0; j < N; j++) 
    {
        for (int k = 0; k < N; k++) 
        {
            C[i][j] += A[i][k] * B[k][j];
        }
    }
}
```

Where **C** is the resulting matrix, **A** and **B** are the two matrices being multiplied and N is the size of the matrix.

For small matrices the algorithm is fast enough to be run on a single thread, however for larger matrices the algorithm becomes slow and the time taken to calculate the matrix becomes significant. This is where the use of multiple threads comes in.

This task requires 4 programs to be made:
1. A single threaded program that calculates the matrix multiplication developed to support any size of matrix.
2. A multi-threaded program that calculates the matrix multiplication developed to support any size of matrix and any number of threads.
3. A single threaded program that calculates the matrix multiplication that uses fixed input values of at least 4x4
4. A multi-threaded program that calculates the matrix multiplication that uses fixed input values of at least 4x4 and any number of threads.

----

## Folder Structure

The folder structure is as follows:

```bash
├── Exercise 1
│   ├── README.md
│   ├── Pthread Implementation
│   │   ├── Fixed Values
│   │   └── Generic
│   ├── Serial Implementation
│   │   ├── Fixed Values
│   │   └── Generic
│   └── Results
│       ├── 5x5.xlsx
│       ├── 50x50.xlsx
│       ├── 500x500.xlsx
│       ├── 1000x1000.xlsx
│       └── 2000x2000.xlsx
└── 
```

Inside this folder there is the Pthread Implementation and Serial Implementation folders. Inside these folders are the Generic and Fixed Values folders. The Generic folders contain the program that can be run with any size of matrix and the Fixed Values folders contain the programs that can be run with a fixed size of 4x4 and 1000x1000. Which matrix size is used is determined by the user during compilation (Will be discussed in the build section). The Results folder contains the raw data from the experiments that were run. The results will be pictured and discussed in the results section.

----

## Implementation

The serial implementation of the matrix multiplication application is very simple.
It follows the algorithm described above and calculates each element of the resulting matrix in a single thread. First it generates two matrices of random values and then starts a timer. It then calculates the resulting matrix and stops the timer. The time taken to calculate the matrix is then printed to the console.

However, the multithreaded version of the application is a little more complex. There are many approaches to parallelising the matrix multiplication algorithm. One method could be to create a thread for each element of the resulting matrix. However, this would be very inefficient as:
1. There would be a lot of threads created and destroyed which will cause a performance hit.
2. The threads would be competing for the same resources (CPU cores) which will cause a performance hit.

An alternative method would be to create a pool of threads that has access to a task queue. The task queue would contain the elements of the resulting matrix that need to be calculated. The threads would then take a task from the queue and calculate it. This method would be more efficient than the one above as there wont be a lot of threads created and destroyed and the threads wont be competing for the same resources. However, this method would still be inefficient as:
1. There will be a performance hit as each thread will need to access the task queue, get a task and then calculate it. The calculation of the task will not be that time consuming but the overhead of accessing the task queue and pulling a task for each resulting matrix element will be significant. 
2. Furthermore, this queue will need to be protected by a mutex to prevent multiple threads from accessing it at the same time which will cause a performance hit.

Therefore, an optimum algorithm would:
1. Not create a lot of threads that fight for the same CPU cores. *(Although this could be overwritten by the user)*
2. Will have all the tasks that it needs to assign to the threads before the threads are created to avoid the overhead of accessing a task queue.
3. Will not have be accessing data that is shared to avoid the overhead of using mutexes.

The pthread implementation follows these rules. It works by taking the expected resultant matrix and splitting its rows between the number of threads that are to be created. Each thread is then assigned a set of rows to calculate, if the number of rows can not be divided evenly one thread ends up doing slightly more rows than the others. The threads then calculate the rows that they are assigned and inputs the answer into the resulting matrix. The threads then join and the time taken to calculate the matrix is printed to the console.

As the rows are assigned before the threads are created, the overhead of accessing a task queue is avoided. Furthermore, as the threads are not accessing data that is shared, as each thread only writes to the locations it is responsible for and no other thread will be assigned those location, the overhead of using mutexes is avoided.

Much like the serial implementation, first the two matrices are generated and then a timer is started. The threads are then created and the resulting matrix is calculated. The threads then join and the time taken to calculate the matrix is printed to the console.

Although only square matrices are used in this implementation, the program can be used to calculate the multiplication of any two matrices. The program will check if the two matrices can be multiplied and if they can it will calculate the resulting matrix. If the two matrices cannot be multiplied the program will print an error message to the console and exit.

----

## Build Guide

All the programs comes with a makefile. To build the program simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.
Furthermore, this make file will also run the program using some default values. What the default value are and how to change them will be explained in each programs individual section.

### Prerequisites

This is a c++ program and therefore requires a c++ compiler. The program was developed using the g++ compiler. The program also requires the pthread library to be installed. It also requires a version GNU Make to be installed.

The program was compiled using g++ 11.2.0 on Windows 10 (Version 10.0.19044 Build 19044)
Although the program was developed on Windows 10, it should work on any operating system that supports pthreads and has g++ installed and callable from the terminal.

If you are on windows you can obtain g++ by downloading the **MSYS2** project from [here](https://www.msys2.org/). Once installed, you can install g++ by running the following command in the MSYS2 terminal.

```
    pacman -S mingw-w64-x86_64-toolchain
```

This will install g++, gcc and a version of make. 

Once installed you must add the executable to your environment path.

After that you are ready to build the program.

Simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.
Furthermore, this make file will also run the program using some default values. What the default value are and how to change them will be explained in each programs individual section.

### Running the programs

#### Single Threaded Generic

First, navigate to the `./Exercise 1/Serial Implementation/Generic` directory.

To run the program with the default values, simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.

This will run the serial implementation of the matrix multiplication application with two matrices of size 2000x2000 each once. The time taken to calculate the matrix will be printed to the console.

To change matrix size, simply change the variables `a_size` and `b_size` either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each, run the following command in the terminal.

```
    make a_size=1000x1000 b_size=1000x1000
```

You can also change how many runs of the application it preforms by changing the `runs` variable either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each and run the application 5 times, run the following command in the terminal.

```
    make a_size=1000x1000 b_size=1000x1000 runs=5
```

Ultimately all this does is change the arguments passed into the binary program.

You can also interact with the raw executable file directly. To do this, navigate to the `./bin/` directory and run the following command in the terminal.

```
    ./Matrix_Multiply_Serial.exe -A 1000x1000 -B 1000x1000
```

Where `-A` is the size of the first matrix and `-B` is the size of the second matrix. The size of the matrices are in the format `widthxheight`. 


#### Single Threaded Fixed

First, navigate to the `./Exercise 1/Serial Implementation/Fixed Values` directory.

To run the program with the default values, simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.

By default it will use the 4x4 matrix and print the matrices and the resulting matrix to the console. It will also validate the answer against the correct answer that can be found in the `./Exercise 1/Serial Implementation/Fixed Values/includes/Matrix_Values.h` file.

The user has the option to either use a 4x4 matrix or a 1000x1000 matrix. To use the 1000x1000 matrix, simply change the `matrix` variable in the make file or on the command line. For example, to use the 1000x1000 matrix, run the following command in the terminal.

```
    make matrix=matrix_1000x1000
```

**This program can not be interacted with using arguments and must be interacted with using the make file.**


#### Multi Threaded Generic

First, navigate to the `./Exercise 2/Pthreads Implementation/Generic` directory.

To run the program with the default values, simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.

This will run the pthread implementation of the matrix multiplication application with two matrices of size 2000x2000 each once using 8 threads for multiplication. The time taken to calculate the matrix will be printed to the console.

**NOTE: When specifying the number of threads this is in addition to the thread running the main function. So if you only have 8 executable units you should only use 7 threads for multiplication**

This program can be change similar to the Single Threaded Generic program. 

To change the matrix size, simply change the variables `a_size` and `b_size` either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each, run the following command in the terminal.

```
    make a_size=1000x1000 b_size=1000x1000
``` 

To change the number of threads, simply change the `threads` variable either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each and use 4 threads for multiplication, run the following command in the terminal.

```
    make a_size=1000x1000 b_size=1000x1000 threads=4
```

You can also change how many runs of the application it preforms by changing the `runs` variable either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each and run the application 5 times, run the following command in the terminal.

```
    make a_size=1000x1000 b_size=1000x1000 runs=5
```

Ultimately all this does is change the arguments passed into the binary program.

You can also interact with the raw executable file directly. To do this, navigate to the `./bin/` directory and run the following command in the terminal.

```
    ./Matrix_Multiply_Pthreads.exe -A 1000x1000 -B 1000x1000 -T 4
```

Where `-A` is the size of the first matrix and `-B` is the size of the second matrix. The size of the matrices are in the format `widthxheight`. `-T` is the number of threads to use for multiplication.