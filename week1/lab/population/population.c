#include <cs50.h>
#include <stdio.h>

int threshold(int start, int end);

int main(void)
{
    // Prompt for start size
    int start;
    do
    {
        start = get_int("Start size: ");
    }
    while (start < 9);
    // Prompt for end size
    int end;
    do
    {
        end = get_int("End size: ");
    }
    while (end < start);
    // Calculate number of years until we reach threshold
    int year = threshold(start, end);
    // Print number of years
    printf("Years: %i\n", year);
}

int threshold(int start, int end)
{
    int time = 0;
    while (start < end)
    {
        start = (int) start + (int) (start / 3) - (int) (start / 4);
        time++;
    }
    return time;
}
