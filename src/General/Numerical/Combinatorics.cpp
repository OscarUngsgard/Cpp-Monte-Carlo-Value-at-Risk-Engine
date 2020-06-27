#include "Combinatorics.h"
#include <iostream>
double binomCoefficient(unsigned long n, unsigned long k)
{
    //Source: https://stackoverflow.com/questions/9330915/number-of-combinations-n-choose-r-in-c
    if (k > n) return 0;
    if (k * 2 > n) k = n - k;
    if (k == 0) return 1;

    double result = n;
    for (int i = 2; i <= k; ++i) {
        result *= (n - i + 1);
        result /= i;
    }
    return result;
}

