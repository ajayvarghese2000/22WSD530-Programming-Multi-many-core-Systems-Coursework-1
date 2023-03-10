<!-- This README is in markdown format, It is must viewed using a markdown viewer -->

<!-- This is because there will be pictures and links embedded in the file -->

<!-- If you do not have a markdown viewer please visit the flowing github link to view properly and in the intended format -->

<!-- https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/blob/main/Exercise%202/README.md -->

<!-- This GitHub will be private until after the submission deadline -->

# Exercise 2 Producer Consumer Problem

<p align="center">
	<a href="https://github.com/ajayvarghese2000/22WSD530-Programming-Multi-many-core-Systems-Coursework-1/tree/main/Exercise%202"><img src="https://i.imgur.com/wdlhfhC.png"></a>
    <p align="center"> Example being run with 4 Producers, 1 Consumer, 15 items generated by each producer and a buffer size of 10 </p>
</p>

----

## Table of Contents
- [Introduction](#Introduction)
- [Implementation](#Implementation)
- [Build Guide](#Build-Guide)
    - [Prerequisites](#Prerequisites)
    - [Running the program](#Running-the-program)
    
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

Due to the simplicity of the program it is better to interact with the executable directly and only use the makefile to build the program not run it.

To interact with the executable directly, you must pass in the arguments as described earlier.

```
    ./producer_consumer.exe -P 4 -C 1 -I 4 -B 10
```

This will create 4 producers, 1 consumer, each producer will generate 4 items and the buffer size will be 10.

Once compiled, you can find the executable in the `./bin` directory.