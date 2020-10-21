#include <stdio.h>
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#include "util.h" //implementing

#define BATCH_SIZE 50 // max amount of candidate numbers a pthread can read

/* GLOBAL VARIABLES */
unsigned int current_candidate_prime; // number to be tested
pthread_mutex_t lock; // lock status used to properly assign candidate primes
unsigned int max_num; // maximum number of candidates to evaluate
bool verbosity; // flag to determine whether to print primalities

/* FUNCTIONS */

/**
 * Checks if the given number is prime using a naive approach.
 * @param[in] num The number of which to check the primality.
 * @return[out] Its primality in a form of a boolean.
 */
static bool is_prime(unsigned int num)
{
    if (num <= 3)
        return num > 1;
    else if (num % 2 == 0 || num % 3 == 0)
        return false;
    int i = 5;

    while (i * i <= num)
    {
        if (num % i == 0 || num % (i + 2) == 0)
            return false;
        i += 6;
    }
    return true;
}

/**
 * Gets consecutive numbers to test that have not been tested.
 * @param[in] vals Where the values will be put.
 * @param[in] size How many values to get. 
 */
size_t get_test_set(unsigned int vals[], size_t size)
{
    size_t i = 0;
    int start_val;
    pthread_mutex_lock(&lock);
    if (current_candidate_prime <= max_num) // Still numbers left to check
    {
        start_val = current_candidate_prime;   // Start with current
        current_candidate_prime += BATCH_SIZE; // Move past this batch
        pthread_mutex_unlock(&lock);
        // Get the requested batch size or as many as legal
        for (i = 0; i < size && start_val <= max_num; ++i)
            vals[i] = start_val++;
    }
    else
        pthread_mutex_unlock(&lock);
    return i;
}

/**
 * Consumes BATCH_SIZE consecutive integers and tests their primalities.
 * Takes no parameters and returns nothing. Prints the value if it is prime
 * and verbosity is not 0. 
 */
void *consume_and_test(void *_)
{
    unsigned int set[BATCH_SIZE];
    size_t actual_size = 0;

    // While there is still numbers left to process
    while ((actual_size = get_test_set(set, BATCH_SIZE)) > 0)
    {
        for (int i = 0; i < actual_size; ++i)
        {
            unsigned int cur_num = set[i];
            if (is_prime(cur_num) && verbosity != 0)
                printf("%d\n", cur_num);
        }
        // That was the last batch
        if (actual_size < BATCH_SIZE)
            break;
    }
    return NULL;
}

/**
 * Sequentially test primes using a single thread
 * @param[in] max The maximum amount of primes to check
 * @param[in] verb flag to determine whether to print primalities
 */
void primes_st(unsigned int max, unsigned int verb)
{
    for (int i = 0; i < max; ++i)
        if (is_prime(i) && verb != 0)
            printf("%d\n", i);
}

/**
 * Sequentially test primes across multiple threads
 * @param[in] max The maximum amount of primes to check
 * @param[in] threads The amount of threads allowed
 * @param[in] verb flag to determine whether to print primalities
 */
void primes_mt(unsigned int max, unsigned int threads,
               unsigned int verb)
{
    max_num = max;
    verbosity = verb;
    if (pthread_mutex_init(&lock, NULL) != 0)
        pthread_exit((void *)1);
    pthread_t thread_arr[threads];
    for (int i = 0; i < threads; ++i)
        if (pthread_create(&thread_arr[i], NULL, &consume_and_test, NULL) != 0)
            pthread_exit((void *)1);

    for (int i = 0; i < threads; ++i)
        pthread_join(thread_arr[i], NULL);
    pthread_mutex_destroy(&lock);
}
