#include <cs50.h>
#include <stdio.h>

int max(int len, int array[]);

int main(void)
{
    // Get the number of elements
    int length;
    do
    {
        length = get_int("Number of elements: ");
    }
    while (length <= 0);
    // Initialize the array
    int array[length];
    for (int i = 0; i < length; i++)
    {
        array[i] = get_int("Element %i: ", i);
    }
    // Print the max value
    printf("The max value is %i\n", max(length, array));
    return 0;
}

int max(int len, int array[])
{
    int largest;
    for (int j = 0; j < len; j++)
    {
        // Set largest to be the first element in the array
        if (j == 0)
        {
            largest = array[j];
            continue;
        }
        if (largest < array[j])
        {
            largest = array[j];
        }
    }
    return largest;
}
