#include <cs50.h>
#include <ctype.h>
#include <stdio.h>

float calc_hours(int hours[], int weeks, char output);

int main(void)
{
    int weeks = get_int("Number of weeks taking CS50: ");
    int hours[weeks];
    // Take the user's inputs 
    for (int i = 0; i < weeks; i++)
    {
        hours[i] = get_int("Week %i HW Hours: ", i);
    }
    // Prompt the user for the choice
    char output;
    do
    {
        output = toupper(get_char("Enter T for total hours, A for average hours per week: "));
    }
    while (output != 'T' && output != 'A');
    // Print out the desired output
    printf("%.1f hours\n", calc_hours(hours, weeks, output));
}

float calc_hours(int hours[], int weeks, char output)
{
    float total = 0;
    float average;
    // Calculate the total
    for (int i = 0; i < weeks; i++)
    {
        total += hours[i];
    }
    // Calculate the average
    average = total / (float) weeks;
    if (output == 'T')
    {
        return total;
    }
    else
    {
        return average;
    }
}