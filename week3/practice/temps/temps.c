// Practice working with structs
// Practice applying sorting algorithms

#include <cs50.h>
#include <stdio.h>

#define NUM_CITIES 10

typedef struct
{
    string city;
    int temp;
}
avg_temp;

avg_temp temps[NUM_CITIES];

void sort_cities(void);

int main(void)
{
    // Add data to avg_temp structure
    temps[0].city = "Austin";
    temps[0].temp = 97;

    temps[1].city = "Boston";
    temps[1].temp = 82;

    temps[2].city = "Chicago";
    temps[2].temp = 85;

    temps[3].city = "Denver";
    temps[3].temp = 90;

    temps[4].city = "Las Vegas";
    temps[4].temp = 105;

    temps[5].city = "Los Angeles";
    temps[5].temp = 82;

    temps[6].city = "Miami";
    temps[6].temp = 97;

    temps[7].city = "New York";
    temps[7].temp = 85;

    temps[8].city = "Phoenix";
    temps[8].temp = 107;

    temps[9].city = "San Francisco";
    temps[9].temp = 66;

    // Sort the temp
    sort_cities();
    // Print the headline
    printf("\nAverage July Temperatures by City\n\n");
    // Print the cities with the highest to lowest temperature
    for (int i = 0; i < NUM_CITIES; i++)
    {
        printf("%s: %i\n", temps[i].city, temps[i].temp);
    }
}

// Sort cities by temperature in descending order
void sort_cities(void)
{
    // Selection sort algorithm.
    // (Time complexit O(n^2), Space complexity O(NUM_CITIES))
    int smallest;
    for (int i = 0; i < NUM_CITIES; i++)
    {
        // Find the smallest elements in a sub-array
        for (int j = 0; j < NUM_CITIES - i; j++)
        {
            if (j == 0)
            {
                smallest = temps[i].temp;
                continue;
            }
            if (smallest > temps[j + i].temp)
            {
                smallest = temps[j + i].temp;
            }
        }
        temps[i].temp = smallest;
    }
    /* Bubble sort algorithm
     * (Time complexit O(n^2), Space complexity O(NUM_CITIES))
    int placeholder;
    * Only n - 1 iterations are needed (the smallest and second smallest number fall in place the same time)
    for (int i = 0; i < NUM_CITIES - 1; i++)
    {
        * Compare a maximum of n - 1 times
        for (int j = 0; j < NUM_CITIES - 1; j++)
        {
            if (temps[j].temp <= temps[j + 1].temp)
            {
                * Condition satisfied
                break;
            }
            if (temps[j].temp > temps[j + 1].temp)
            {
                * Swap the elements
                placeholder = temps[j].temp
                temps[j].temp = temps[j + 1].temp;
                temps[j + 1].temp = placeholder;
            }
        }
    }
    */
    /* Insertion sort algorithm
     * (Time complexit O(n^2), Space complexity O(NUM_CITIES))
    */
}
