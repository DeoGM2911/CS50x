#include <stdio.h>
#include <cs50.h>
#include <ctype.h>
#include <string.h>
#include <stdlib.h>

bool only_digits(string key);
char rotate(char c, int key);

int main(int argc, string argv[])
{
    // Check the number of command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./ceasar key\n");
        return 1;
    }
    // Check if the key argument is a number
    if (!(only_digits(argv[1])))
    {
        printf("Usage: ./ceasar key\n");
        return 1;
    }
    // Convert agrv[1] into an int
    int key = atoi(argv[1]);
    // Get the user's input
    string plain_text = get_string("plaintext: ");
    // Print out the cipher text
    printf("ciphertext: ");
    char c;
    for (int j = 0, n = strlen(plain_text); j < n; j++)
    {
        c = rotate(plain_text[j], key);
        printf("%c", c);
    }
    printf("\n");
    return 0;
}

bool only_digits(string key)
{
    for (int i = 0, n = strlen(key); i < n; i++)
    {
        if (!(isdigit(key[i])))
        {
            return false;
        }
    }
    return true;
}

char rotate(char c, int key)
{
    // Handle lowercased letter
    if (islower(c))
    {
        return (((c - 97 + key) % 26) + 97);
    }
    // Handle uppercased letter
    else if (isupper(c))
    {
        return (((c - 65 + key) % 26) + 65);
    }
    // Handle punctuation or UNICODE char outside of the 127 range
    else
    {
        return c;
    }
}
