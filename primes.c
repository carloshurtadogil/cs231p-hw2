#include <stdio.h> //remove if not using.
#include <pthread.h>
#include <stdbool.h>
#include <stdint.h>

#include "util.h" //implementing

#define BATCH_SIZE 50
unsigned int current_candidate_prime;
pthread_mutex_t lock;
unsigned int max_num;
bool verbosity;

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

void primes_st(unsigned int max, unsigned int verb)
{
    for (int i = 0; i < max; ++i)
        if (is_prime(i))
            if (verb != 0)
                printf("%d\n", i);
    return;
}

size_t get_test_set(unsigned int vals[], size_t size)
{
    pthread_mutex_lock(&lock);
    size_t i;
    for (i = 0; i < size; ++i)
    {
        if (current_candidate_prime > max_num)
        {
            pthread_mutex_unlock(&lock);
            return i;
        }
        vals[i] = current_candidate_prime++;
    }
    pthread_mutex_unlock(&lock);
    return i;
}

void *consume_and_test(void *_)
{
    unsigned int set[BATCH_SIZE];
    size_t actual_size = 0;
    while ((actual_size = get_test_set(set, BATCH_SIZE)) > 0)
    {
        for (int i = 0; i < actual_size; ++i)
        {
            unsigned int cur_num = set[i];
            if (is_prime(cur_num))
                if (verbosity != 0)
                    printf("%d\n", cur_num);
        }
    }
    return NULL;
}

void primes_mt(unsigned int max, unsigned int threads,
               unsigned int verb)
{
    max_num = max;
    verbosity = verb;
    if (pthread_mutex_init(&lock, NULL) != 0)
        pthread_exit((void *)1);
    pthread_t thread_arr[threads];
    for (int i = 0; i < threads; ++i)
    {
        if (pthread_create(&thread_arr[i], NULL, &consume_and_test, NULL) != 0)
            pthread_exit((void *)1);
    }

    for (int i = 0; i < threads; ++i)
        pthread_join(thread_arr[i], NULL);
    pthread_mutex_destroy(&lock);
}
