#include <cs50.h>
#include <ctype.h>
#include <math.h>
#include <stdio.h>
#include <string.h>

int convert(int len, string input);

int main(void)
{
    // Prompt the user
    string input = get_string("Enter a positive integer: ");
    // Check whether is it a positive int
    int n = strlen(input);
    for (int i = 0; i < n; i++)
    {
        if (!isdigit(input[i]))
        {
            printf("Invalid Input!\n");
            return 1;
        }
    }
    // Convert string to int
    printf("%i\n", convert(n, input));
}

int convert(int len, string input)
{
    // Base case
    if (len == 1)
    {
        int i = input[len - 1] - '0';
        return i;
    }
    // Convert the last char
    int i = input[len - 1] - '0';
    return (convert(len - 1, input) * 10 + i);
}