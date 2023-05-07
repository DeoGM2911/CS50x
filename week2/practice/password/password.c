// Check that a password has at least one lowercase letter, uppercase letter, number and symbol
// Practice iterating through a string
// Practice using the ctype library

#include <cs50.h>
#include <stdio.h>
#include <ctype.h>

bool valid(string password);

int main(void)
{
    // Get the user's password
    string password = get_string("Enter your password: ");
    if (valid(password))
    {
        // Print a report that the password is valid
        printf("Your password is valid!\n");
    }
    else
    {
        // Print a report that the password is invalid
        printf("Your password needs at least one uppercase letter, lowercase letter, number and symbol\n");
    }
}

bool valid(string password)
{
    // Initialize some counters
    int i = 0;
    // From left to right is the counter for lower, upper, number, and symbol char
    int count[4] = {0, 0, 0, 0};
    while (password[i] != '\0')
    {
        if (islower(password[i]))
        {
            count[0]++;
        }
        if (isupper(password[i]))
        {
            count[1]++;
        }
        if (isdigit(password[i]))
        {
            count[2]++;
        }
        // Check if the char is a symbol
        if (ispunct(password[i]) || isspace(password[i]))
        {
            count[3]++;
        }
        i++;
    }
    for (int j = 0; j < 4; j++)
    {
        if (count[j] < 1)
        {
            return false;
        }
    }
    return true;
}