#include <stdio.h> //remove if not using.

#include "util.h" //implementing

typedef struct
{ //example structure
    int example;
    int e_g;
} Example_Structure;

static void example_helper_function(int n)
{
    // Functions defined with the modifier 'static' are only visible
    // to other functions in this file. They cannot be called from
    // outside (for example, from main.c). Use them to organize your
    // code. Remember that in C, you cannot use a function until you
    // declare it, so declare all your utility functions above the
    // point where you use them.
    //
    // Maintain the mat_sq_trans_xt functions as lean as possible
    // because we are measuring their speed. Unless you are debugging,
    // do not print anything on them, that consumes precious time.
    //
    // You may delete this example helper function and structure, and
    // write your own useful ones.

    Example_Structure es1;
    es1.example = 13;
    es1.e_g = 7;
    printf("n = %d\n", es1.example + es1.e_g + n);
    return;
}

void primes_st(unsigned int max, unsigned int verb)
{
    //Put your code here.
    //Remember to swap m and n in the data structure
    //after performing the transposition.
    //example_helper_function(1000);
    return;
}

void primes_mt(unsigned int max, unsigned int threads,
               unsigned int verb)
{
    //Put your code here.
    //Remember to swap m and n in the data structure
    //after performing the transposition.
    //example_helper_function(2000);
    return;
}
