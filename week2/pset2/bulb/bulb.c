#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>

const int BITS_IN_BYTE = 8;

void print_bulb(int bit);

int main(void)
{
    // Get the user's input
    string text = get_string("Message: ");
    // Analyze the string
    int byte[BITS_IN_BYTE];
    int dividend;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Create the binary version of the text
        dividend = text[i];
        for (int j = 7; j >= 0; j--)
        {
            byte[j] = dividend % 2;
            dividend = dividend / 2;
        }
        // Print out the bulbs
        for (int k = 0; k < BITS_IN_BYTE; k++)
        {
            print_bulb(byte[k]);
        }
        // End with a newline before moving on to the next char
        printf("\n");
    }
}

void print_bulb(int bit)
{
    if (bit == 0)
    {
        // Dark emoji
        printf("\U000026AB");
    }
    else if (bit == 1)
    {
        // Light emoji
        printf("\U0001F7E1");
    }
}
