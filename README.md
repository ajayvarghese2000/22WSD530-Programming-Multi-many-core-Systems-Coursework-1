<!-- This README is in markdown format, It is must viewed using a markdown viewer -->

<!-- This is because there will be pictures, gif and links embedded in the file to help explain the programs -->

<!-- If you do not have a markdown viewer please visit the flowing github link to  properly view the file in the intended format -->

<!-- https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%201 -->

<!-- This GitHub will be private until after the submission deadline -->
# 22WSD530 Programming Multi/many-core Systems Coursework 1

This repository contains the code for the first coursework for the module 22WSD530 Programming Multi/many-core Systems. The coursework is split into 2 exercises.

Each exercise has its own README.md file that explains what it is doing and how to run it.

The README.md files can be found in the Exercise 1 and Exercise 2 folders.

They are also summarised below.

## Table of Contents
- [Exercise 1 Matrix Multiplication using Pthreads](#Exercise-1-Matrix-Multiplication-using-Pthreads)
    - [Introduction](#introduction)
    - [Folder Structure](#folder-structure)
    - [Implementation](#implementation)
    - [Build Guide](#build-guide)
        - [Pre-requisites](#pre-requisites)
        - [Running the programs](#running-the-programs)
            - [Single Threaded Generic](#single-threaded-generic)
            - [Single Threaded Fixed](#single-threaded-fixed)
            - [Multi Threaded Generic](#multi-threaded-generic)
            - [Multi Threaded Fixed](#multi-threaded-fixed)
    - [Experiment Setup](#experiment-setup)
    - [Methodology](#methodology)
    - [Results](#results)
        - [5x5](#5x5)
        - [50x50](#50x50)
        - [500x500](#500x500)
        - [1000x1000](#1000x1000)
        - [2000x2000](#2000x2000)
- [Exercise 2 Producer Consumer Problem](#Exercise-2-Producer-Consumer-Problem)
    - [Introduction](#Introduction-1)
    - [Implementation](#Implementation-1)
    - [Build Guide](#Build-Guide-1)
        - [Prerequisites](#Prerequisites)
        - [Running the program](#Running-the-program)

---

# Exercise 1 Matrix Multiplication using Pthreads

<p align="center">
	<a href="https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%201"><img src="https://i.imgur.com/lNPNcwZ.gif"></a>
    <p align="center"> Example 4x4 matrix being run with 4 threads</p>
</p>

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

### Pre-requisites

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

#### Multi Threaded Fixed

First, navigate to the `./Exercise 2/Pthreads Implementation/Fixed Values` directory.

To run the program with the default values, simply run the following command in the terminal.

```
    make
```

This will build the program and place create an executable file in `./bin/` directory.

By default it will use the 4x4 matrix, with 8 threads, and print the matrices and the resulting matrix to the console. It will also validate the answer against the correct answer that can be found in the `./Exercise 2/Pthreads Implementation/Fixed Values/includes/Matrix_Values.h` file.

The user has the option to either use a 4x4 matrix or a 1000x1000 matrix. To use the 1000x1000 matrix, simply change the `matrix` variable in the make file or on the command line. For example, to use the 1000x1000 matrix, run the following command in the terminal.

```
    make matrix=matrix_1000x1000
```

Likewise to change the number of threads, simply change the `threads` variable either in the make file or on the command line. For example, to run the program with two matrices of size 1000x1000 each and use 4 threads for multiplication, run the following command in the terminal.

```
    make matrix=matrix_1000x1000 threads=4
```

**This program should not be interacted with using arguments and must be interacted with using the make file due to needing compile directives**

----

## Experiment Setup

The experiments were conducted on a Windows 10 Version 10.0.19044 Build 19044 machine with the following specifications.

**CPU: Intel Core i7-8700k @ 3.70GHz (12 CPUs), boost to 4.30GHz**

**RAM: 16GB DDR4 3000MHz**

**GPU: Nvidia RTX 3070 Ti**

The applications were compiled using g++ version 11.2.0.

There were no other applications running in teh foreground during the experiments other than the terminal.

----

## Methodology

First serial implementation of the matrix multiplication application was tested.
Matrix sizes of the following were tested:

1. 5x5
2. 50x50
3. 500x500
4. 1000x1000
5. 2000x2000

Each matrix size was tested 100 times and the average time taken was calculated.

The same was done for the pthread implementation of the matrix multiplication application however for each matrix size the threads between 2-13 were tested. Each run was tested 100 times and the average time taken was calculated.

The test PC only had 12 threads available to it so anything above 11 threads would be over subscribing the CPU.

**NOTE: When specifying the number of threads this is in addition to the thread running the main function. So if you only have 8 executable units you should only use 7 threads for multiplication**

The data was collected and compiled into excel spreadsheets and graphs were created comparing the actual speedup of the pthread implementation to the theoretical speedup using Amdahl's Law.

<p align="center">
	<img src="https://i.imgur.com/LrNJlXX.png">
    <p align="center"> Example Spreadsheet</p>
</p>

----

## Results

### 5x5
<p align="center">
	<img src="https://i.imgur.com/gd1k74z.png">
    <p align="center"> Speed Up for 5x5 </p>
</p>

Unfortunately, the 5x5 matrix test was inconclusive as the serial implementation was too fast to measure accurately as it was under nS resolution.

### 50x50
<p align="center">
	<img src="https://i.imgur.com/DUTDngx.png">
    <p align="center"> Speed Up for 50x50 </p>
</p>

The 50x50 matrix test was more interesting, here we can see that the serial implementation was faster than the pthread implementation. Which lead to a speedup on less that 1 for the pthread implementation. This is due to the overhead of creating and managing the threads.

When threads are created and destroyed there is a cost associated with this. This cost is not present in the serial implementation as there is only one thread running the application. Therefore, the serial implementation can be busy doing the matrix multiplication while the pthread implementation is busy creating and destroying threads.

From this we can conclude that the pthread implementation is not suitable for small matrix sizes.
### 500x500
<p align="center">
    <img src="https://i.imgur.com/XKcaXjl.png">
    <p align="center"> Speed Up for 500x500 </p>
</p>

However, for larger matrix sizes the pthread implementation is faster than the serial implementation and follows Amdahl's Law closely. Here the extra threads can be used to speed up the matrix multiplication.

It is worth noting that after 9 threads the speedup no longer follows Amdahl's Law. The system has 12 cores so the expected result is that the speedup should follow Amdahl's Law until 11 threads are used for the multiplication. (1 thread for the main function)

This could be due to the fact that the application was run on an OS and not bare metal. The OS could be using the extra threads for other tasks that will have priority over the application. This would mean that some of the threads will be context switched out and not be able to do any work for some time in order to allow the OS to do its work.

It is also worth noting that the speedup tanks after 11 threads. This is due to the fact that the application is over subscribed. The application is trying to use more threads than the system has available. This means that some threads will be context switched out and not be able to do any work for some time in order to allow the OS to do its work. The more threads you over subscribe the worse the performance will be.

The Amdahl's Law Parallel Efficiency for this was 0.92. This means that 92% of the code was parallelised and 8% was executed serially. This is a good result as it means that the majority of the code was parallelised and the application is suitable for parallelisation.

You will never get 100% parallel efficiency as there will always be some code that is executed serially. For example, the code that creates and destroys the threads.

### 1000x1000

<p align="center">
    <img src="https://i.imgur.com/GxPg2TD.png">
    <p align="center"> Speed Up for 1000x1000 </p>
</p>

This follows the same trend as the 500x500 matrix size. The speedup follows Amdahl's Law closely until 9 threads are used. After 9 threads the speedup no longer follows Amdahl's Law.

The Amdahl's Law Parallel Efficiency for this was 0.92 as well.
### 2000x2000

<p align="center">
    <img src="https://i.imgur.com/QKD2MiL.png">
    <p align="center"> Speed Up for 2000x2000 </p>
</p>

This follows the same trend as the 500x500 matrix size. The speedup follows Amdahl's Law closely until 9 threads are used. After 9 threads the speedup no longer follows Amdahl's Law. This could be for much the same reason as the 500x500 matrix size.

The Amdahl's Law Parallel Efficiency for this was 0.92 as well.

----

# Exercise 2 Producer Consumer Problem

<p align="center">
	<a href="https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%202"><img src="https://i.imgur.com/wdlhfhC.png"></a>
    <p align="center"> Example being run with 4 Producers, 1 Consumer, 15 items generated by each producer and a buffer size of 10 </p>
</p>

----
## Introduction

The producer consumer problem is a classic problem in computer science. It is a problem where there are multiple producers and consumers that share a common buffer. The producers produce items and place them in the buffer and the consumers consume the items from the buffer. 

The problem is that the buffer has a limited size and the producers and consumers must work together to ensure that the buffer does not overflow. This problem is a classic multi-threaded problem.

<p align="center">
	<img src="https://i.imgur.com/fII1guB.png">
</p>

However specific to this module we are asked to generate numbers using a given algorithm and then place them in the buffer where the consumer will then consume them and sort them using another algorithm. The sorted values are then output to the terminal.

---

## Implementation

The program is implemented using the pthread library. The program is run from the main file found in the `./src` directory. 

There are 8 key objectives for this task.

1. Producer threads must generate random numbers using a specific algorithm.
2. The consumer thread must sort the numbers using another specific algorithm.
3. The number of producers and consumers must be able to be varied.
4. The number of items generated by each producer must be able to be varied.
5. The size of the buffer must be able to be varied.
6. The program must protect the shared resources.
7. The program must be able to handle situations where the buffer is full without skipping data.
8. The program must be able to handle situations where the buffer is empty and not terminate until all the required data has been consumed.

**Objective 1** was achieved by creating a function that generates a random number using the poisson distribution. This is handled in the `producer()` function, it uses the `random` library to generate and seed a random number generator. The random number generator is then used to generate a random number using the poisson distribution. The `random` library has a poisson distribution class that we can pass the random number generator too, this will allow us to generate a random number using the poisson distribution.

**Objective 2** was achieved by creating a function that sorts the numbers using a modified quick sort algorithm. This is handled in the `quicksort()` function, it looks through the array and finds where the new consumed value should go moves the other elements across and then places the new value in the correct position.

**Objective 3** , **Objective 4** and, **Objective 5**  was achieved by allowing the program to take in arguments. Once compiled, to run the program you must pass in the number of producers, consumers, items generated by each producer and the buffer size as arguments.

E.g.
```
    ./producer_consumer.exe -P 5 -C 5 -I 100 -B 10
```

The `-P` flag is used to specify the number of producers, the `-C` flag is used to specify the number of consumers, the `-I` flag is used to specify the number of items generated by each producer and the `-B` flag is used to specify the buffer size.

**Objective 6** Is a key objective with any producer consumer problem. The program must protect the shared resources, otherwise as multiple threads are accessing the same resources at the same time, there is a possibility for a race condition to occur. To alleviate this issue, the program uses mutex locks. There are 3 mutex locks used in the program. 
1. One mutex lock is used to protect the buffer and the next free position in the buffer.
2. Another mutex lock is used to protect the total number of items consumed. (This is to help with objective 8) When there are more consumers than producers there is a possibility for the buffer to be empty which could cause the program to terminate before all the items have been consumed. This mutex lock is used to ensure that the program does not terminate until all the items have been consumed.
3. The last mutex lock is used to protect the `cout` function. This is to ensure that no two threads try to output to the terminal at the same time which could cause the output to be jumbled and overlapping.

**Objective 7** was achieved by using semaphores. The program uses 2 semaphores. One semaphore is used to keep track of the number of free spaces in the buffer. The other semaphore is used to keep track if the buffer is empty. The producer will wait until there is a free space in the buffer before inserting an item. The consumer will wait until there is an item in the buffer before consuming an item.

**Objective 8** was achieved by keeping track of how many items have been consumed by all consumers. The program uses a mutex lock to protect the total number of items consumed. Every time a consumer consumes an item it increments the total number of items consumed. The program will terminate once the total number of items consumed is equal to the total number of items generated by all producers.

For this implementation, the queue acts like a **stack** i.e first in last out. This should not mess up the order of the items as once the items are consumed they are sorted in ascending order.

----

## Build Guide

This program comes with a makefile. To build the program simply run the following command in the terminal.

```
    make
```

This will create an executable file called `producer_consumer.exe` in the `./bin` directory.

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

This will call g++ and create an executable file called `producer_consumer.exe` in the `./bin` directory. It will output any errors to the terminal.

### Running the program

The simplest way to run the program is to run the make target `test`. This will run the program with the default arguments.

```
    make test
```

The default arguments are as follows.

```
    ./producer_consumer.exe -P 4 -C 1 -I 4 -B 10
```

This will create 4 producers, 1 consumer, each producer will generate 4 items and the buffer size will be 10.

If you want to change the variables in the make file and then run test.

```
    make test producer_threads=1 consumer_threads=2 items=5 buffer_size=10
```

This will create 1 producer, 2 consumers, each producer will generate 5 items and the buffer size will be 10.

However, due to the simplicity of the program it is better to interact with the executable directly and only use the makefile to build the program.

To interact with the executable directly, you must pass in the arguments as described earlier.

```
    ./producer_consumer.exe -P 4 -C 1 -I 4 -B 10
```

This will create 4 producers, 1 consumer, each producer will generate 4 items and the buffer size will be 10.
