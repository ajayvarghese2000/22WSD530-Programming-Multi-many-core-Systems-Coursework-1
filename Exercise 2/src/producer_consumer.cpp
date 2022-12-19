/**
 * @file    producer_consumer.cpp
 * @author  B929164
 * @date    19/12/2022
 * @brief   This file contains the implementation of the producer consumer problem
 *          There are a set number of producers and consumers. The producers generate
 *          a random number using the poisson  distribution and adds it to the buffer.
 *          If the buffer is full, the producer waits until there is an empty slot.
 *          
 *          The consumers consume the number from the buffer and adds it to its own
 *          internal array where it is sorted using the quick sort algorithm. This
 *          sorted array is then printed to the console. The consumers will consume
 *          until all the producers have finished producing after which they will
 *          terminate.
 * 
 *          The producers and consumers are implemented using threads.
 *          The buffer is implemented using a mutex and semaphores as it is a shared
 *          resource and needs to be protected.
 * 
 *          The number of producers and consumers are defined by arguments passed
 *          to the program. As well as the number of elements each producer will
 *          produce and the size of the buffer.
 * 
 *              E.g ./producer_consumer -P 5 -C 5 -I 100 -B 10
 *          
 *          This will create 5 producers and 5 consumers. Each producer will produce
 *          100 elements and the buffer will have a size of 10.
 * 
 */



/* ######################## [Includes] ######################## */

#include <iostream>     // For cout, endl
#include <pthread.h>    // For threads and mutexs
#include <semaphore.h>  // For semaphores
#include <random>       // For random number generation using the poisson distribution

using namespace std;


/* ######################## [Defines] ######################## */

// This is the arguments that will be passed to each producer thread
struct producerInputArgs
{
    // A pointer to the buffer array
    int* buffer;
    
    // The size of the buffer
    int bufferSize;

    // Pointer to next empty slot in the buffer
    int* nextSlot;

    // The number of elements each producer will produce
    int numElements;

    // A pointer to the mutex that will be used to protect the buffer
    pthread_mutex_t* mutex;

    // A pointer to the semaphore that will be used to signal when the buffer is full
    sem_t* full;

    // A pointer to the semaphore that will be used to signal when the buffer is empty
    sem_t* empty;

};

// This is the arguments that will be passed to each consumer thread
struct consumerInputArgs
{
    // A pointer to the buffer array
    int* buffer;

    // The size of the buffer
    int bufferSize;

    // Pointer to next empty slot in the buffer
    int* nextSlot;

    // The number of elements each producer will produce
    int numElements;

    // The number of producers in the system
    int numProducers;

    // Pointer to the total number of elements consumed
    int* totalElementsConsumed;

    // A pointer to the mutex that will be used to protect the buffer
    pthread_mutex_t* mutex;

    // A pointer to the mutex that will be used to protect the total number of elements consumed
    pthread_mutex_t* totalElementsConsumedMutex;

    // A pointer to the semaphore that will be used to signal when the buffer is full
    sem_t* full;

    // A pointer to the semaphore that will be used to signal when the buffer is empty
    sem_t* empty;

};


/* ######################## [Functions ] ######################## */

/**
 * @brief   This function will sort the array using the quick sort algorithm
 * 
 * @param   array   A pointer to the array that will be sorted
 * @param   size    Next empty slot in the array
 * @param   element The element that will be inserted into the array
 * 
 * @return  NULL
 */
void quicksort(int* array, int size, int element)
{
    // If the array is empty, insert the element at the start
    if (size == 0)
    {
        array[0] = element;
    }
    else
    {
        // Find the correct position for the element
        for (int i = 0; i < size; i++)
        {
            // If the element is smaller than the current element, insert it before
            if (element < array[i])
            {
                // Shift all the elements after the current element up one
                for (int j = size; j > i; j--)
                {
                    array[j] = array[j - 1];
                }

                // Insert the element
                array[i] = element;

                // Exit the loop
                break;
            }
            // If the element is larger than the current element, insert it after
            else if (element > array[i])
            {
                // If the element is the last element in the array, insert it at the end
                if (i == size - 1)
                {
                    array[size] = element;
                }
            }
            // If the element is equal to the current element, insert it after
            else
            {
                // Shift all the elements after the current element up one
                for (int j = size; j > i; j--)
                {
                    array[j] = array[j - 1];
                }

                // Insert the element
                array[i + 1] = element;

                // Exit the loop
                break;
            }
        }
    }
}


/**
 * @brief   This function will be called by each producer thread. It will generate
 *          a random number using the poisson  distribution and adds it to the buffer.
 *          If the buffer is full, the producer waits until there is an empty slot.
 * 
 * @param   args    A pointer to the arguments that will be passed to the thread
 * 
 * @return  NULL
 */
void* producer(void* args)
{
    // Cast the arguments to the correct type
    producerInputArgs* inputArgs = (producerInputArgs*)args;

    // Create a random number generator using the poisson distribution
    random_device rd;
    mt19937 gen(rd());              // Standard mersenne_twister_engine seeded with rd()
    poisson_distribution<> d(5);    // The mean of the distribution is 5

    // Add the random number to the buffer
    for (int i = 0; i < inputArgs->numElements; i++)
    {   
        // Generate a random number using the poisson distribution
        int randomNum = d(gen);
        
        // Wait until there is an empty slot in the buffer
        sem_wait(inputArgs->empty);

        // Lock the buffer
        pthread_mutex_lock(inputArgs->mutex);

        // Add the random number to the buffer
        inputArgs->buffer[*inputArgs->nextSlot] = randomNum;

        // Increment the number of elements produced
        *inputArgs->nextSlot += 1;

        // Unlock the buffer
        pthread_mutex_unlock(inputArgs->mutex);

        // Print the number that was produced
        // cout << "Producer " << pthread_self() << " produced " << randomNum << endl;

        // Signal that there is a new element in the buffer
        sem_post(inputArgs->full);
    }

    // Print that the producer has finished producing
    // cout << "Producer " << pthread_self() << " finished producing" << endl;

    return NULL;
}


/**
 * @brief   This function will be called by each consumer thread. It will consume
 *          the number from the buffer and adds it to its own internal array where
 *          it is sorted using the quick sort algorithm. This sorted array is then
 *          printed to the console. The consumers will consume until all the producers
 *          have finished producing after which they will terminate.
 * 
 * @param   args    A pointer to the arguments that will be passed to the thread
 * 
 * @return  NULL
 */
void* consumer(void* args)
{
    // Cast the arguments to the correct type
    consumerInputArgs* inputArgs = (consumerInputArgs*)args;

    // Create an array to store the sorted numbers, size is the number of elements 
    // each producer will produce multiplied by the number of producers
    int sortedArray[inputArgs->numElements * inputArgs->numProducers];

    // Temp variable used for holding the consumed element
    int temp;

    // Consume the latest element from the buffer until empty
    for (int i = 0; i < inputArgs->numElements * inputArgs->numProducers; i++)
    {

        // Check if all the items that need to be consumed have been consumed
        if (*inputArgs->totalElementsConsumed == (inputArgs->numElements * inputArgs->numProducers))
        {
            // Print that the consumer has finished consuming
            cout << "Consumer " << pthread_self() << " finished consuming" << endl;

            // Exit the loop
            break;
        }

        // Wait until there is an element in the buffer
        sem_wait(inputArgs->full);

        // Lock the buffer
        pthread_mutex_lock(inputArgs->mutex);

        // Decrement the number of elements produced
        *inputArgs->nextSlot -= 1;

        // Copy the element from the buffer to the temp variable
        temp = inputArgs->buffer[*inputArgs->nextSlot];

        // Unlock the buffer
        pthread_mutex_unlock(inputArgs->mutex);

        // Signal that there is an empty slot in the buffer
        sem_post(inputArgs->empty);

        // lock the total number of elements consumed mutex
        pthread_mutex_lock(inputArgs->totalElementsConsumedMutex);

        // Increment the total number of elements consumed
        *inputArgs->totalElementsConsumed += 1;

        // unlock the total number of elements consumed mutex
        pthread_mutex_unlock(inputArgs->totalElementsConsumedMutex);

        // Print the consumed element to the console
        // cout << "Consumer " << pthread_self() << " Got: " << temp << endl;

        // // Preform a quick sort on the array to insert the new element
        quicksort(sortedArray,i , temp);

        // Printout which consumer is printing the sorted array
        cout << "Consumer " << pthread_self() << " Sorted Array: ";

        // Print the sorted array to the console
        for (int j = 0; j < i + 1; j++)
        {
            cout << sortedArray[j] << " ";
        }
        
        cout << endl;
    }

    return NULL;

}



/* ######################## [Main] ######################## */
int main(int argc, char *argv[])
{
    // Variables to index the location of the arguments values
    int P, C, I, B = 0;

    // Loop through the arguments to see if -P -C -I -B are present
    for (int i = 0; i < argc; i++)
    {
        // If -P is present, store the index of the next argument
        if (string(argv[i]) == "-P")
        {
            P = i + 1;
        }
        // If -C is present, store the index of the next argument
        else if (string(argv[i]) == "-C")
        {
            C = i + 1;
        }
        // If -I is present, store the index of the next argument
        else if (string(argv[i]) =="-I")
        {
            I = i + 1;
        }
        // If -B is present, store the index of the next argument
        else if (string(argv[i]) == "-B")
        {
            B = i + 1;
        }
    }

    // If any of the arguments are missing, print an error message and exit
    if (P == 0 || C == 0 || I == 0 || B == 0)
    {
        std::cout << "Incorrect arguments" << std::endl;
        // Give and example of the correct arguments
        std::cout << "Example: ./producer_consumer -P 2 -C 3 -I 5 -B 10" << std::endl;
        return 0;
    }

    // Get the arguments values and convert them to integers
    int numProducers = atoi(argv[P]);
    int numConsumers = atoi(argv[C]);
    int numElements = atoi(argv[I]);
    int bufferSize = atoi(argv[B]);

    // Create the buffer
    int buffer[bufferSize];

    // Create the mutexs
    pthread_mutex_t mutexBuffer = PTHREAD_MUTEX_INITIALIZER;
    pthread_mutex_t mutexTotalElementsConsumed = PTHREAD_MUTEX_INITIALIZER;

    // Create the buffer index
    int nextSlot = 0;

    // Create the total number of elements consumed
    int totalElements = 0;

    // Create the semaphores
    sem_t empty;
    sem_t full;

    // Initialize the semaphores
    sem_init(&empty, 0, bufferSize);
    sem_init(&full, 0, 0);

    // Create the producer and consumer threads
    pthread_t producerThreads[numProducers];
    pthread_t consumerThreads[numConsumers];

    // Create the producer and consumer input arguments
    producerInputArgs producerArgs[numProducers];
    consumerInputArgs consumerArgs[numConsumers];

    // Create the producer threads
    for (int i = 0; i < numProducers; i++)
    {
        // Set the producer arguments
        producerArgs[i].buffer = buffer;                // Address to the buffer
        producerArgs[i].bufferSize = bufferSize;        // Size of the buffer
        producerArgs[i].nextSlot = &nextSlot;           // Address to the next slot in the buffer
        producerArgs[i].numElements = numElements;      // Number of elements to produce
        producerArgs[i].mutex = &mutexBuffer;           // Address to the mutex
        producerArgs[i].empty = &empty;                 // Address to the empty semaphore
        producerArgs[i].full = &full;                   // Address to the full semaphore
    
        // Create the producer thread check for errors
        if (pthread_create(&producerThreads[i], NULL, producer, &producerArgs[i]) != 0)
        {
            std::cout << "Error creating producer thread" << std::endl;
            return 0;
        }

    }

    // Create the consumer threads
    for (int i = 0; i < numConsumers; i++)
    {
        // Set the consumer arguments
        consumerArgs[i].buffer = buffer;                                                    // Address to the buffer
        consumerArgs[i].bufferSize = bufferSize;                                            // Size of the buffer
        consumerArgs[i].nextSlot = &nextSlot;                                               // Address to the next slot in the buffer
        consumerArgs[i].numElements = numElements;                                          // Number of elements that each producer will produce
        consumerArgs[i].numProducers = numProducers;                                        // Number of producers
        consumerArgs[i].totalElementsConsumed = &totalElements;                             // Address to the total number of elements consumed
        consumerArgs[i].mutex = &mutexBuffer;                                               // Address to the mutex
        consumerArgs[i].totalElementsConsumedMutex = &mutexTotalElementsConsumed;           // Address to the mutex
        consumerArgs[i].empty = &empty;                                                     // Address to the empty semaphore
        consumerArgs[i].full = &full;                                                       // Address to the full semaphore

        // Create the consumer thread check for errors
        if (pthread_create(&consumerThreads[i], NULL, consumer, &consumerArgs[i]) != 0)
        {
            std::cout << "Error creating consumer thread" << std::endl;
            return 0;
        }
    }

    // Join the producer threads
    for (int i = 0; i < numProducers; i++)
    {
        pthread_join(producerThreads[i], NULL);
    }

    // Join the consumer threads
    for (int i = 0; i < numConsumers; i++)
    {
        pthread_join(consumerThreads[i], NULL);
    }

    // Destroy the semaphores
    sem_destroy(&empty);
    sem_destroy(&full);

    // Destroy the mutex
    pthread_mutex_destroy(&mutexBuffer);

    return 0;
    
}