#include <cs50.h>
#include <stdio.h>

bool prime(int number);

int main(void)
{
    // Get the minimum value
    int min;
    do
    {
        min = get_int("Minimum: ");
    }
    while (min < 1);

    // Get the maximum value
    int max;
    do
    {
        max = get_int("Maximum: ");
    } 
    while (min >= max);
    
    // Get all the prime numbers in the given range
    for (int i = min; i <= max; i++)
    {
        if (prime(i))
        {
            printf("%i\n", i);
        }
    }
}

bool prime(int number)
{
    // Check whether prime or not
    // *Ending teh check with n - 1 as n % (n - 1) != 0
    for (int j = 2; j < number - 1; j++)
    {
        if (number % j == 0)
        {
            return false;
        } 
    }
    return true;
}