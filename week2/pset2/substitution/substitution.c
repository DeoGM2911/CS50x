#include <cs50.h>
#include <stdio.h>
#include <ctype.h>
#include <string.h>

char analyze(char c, string key);

int main(int argc, string argv[]) 
{
    // Check if there is exactly 1 command-line arg
    if (argc != 2)
    {
        printf("Usage: ./substitution key\n");
        return 1;
    }
    // Check if there are 26 characters in the key
    if (strlen(argv[1]) != 26)
    {
        printf("The key must contain 26 alphabetic letters!\n");
        return 1;
    }
    // Check if all chars in the key are alphabetic. 
    // At this point, there are 26 non-repeated chars in the key
    for (int i = 0; i < 26; i++)
    {
        // Check for alphabetic char
        if (!(isalpha(argv[1][i])))
        {
            printf("The key must contain only alphabetic letters!\n");
            return 1;
        }
        // Check for non-repeated char
        for (int j = i + 1; j < 26; j++)
        {
            if (argv[1][i] == argv[1][j])
            {
                printf("The key must contain 26 distinct alphabetic letters!\n");
                return 1;
            }
        }
    }
    string ptext = get_string("plaintext: ");
    printf("ciphertext: ");
    for (int u = 0, l = strlen(ptext); u < l; u++)
    {
        printf("%c", analyze(ptext[u], argv[1]));
    }
    printf("\n");
    return 0;
}

char analyze(char c, string key);
{
    // Return the char immediately if it's not alphabetic
    if (!(isalpha(c)))
    {
        return c;
    }
    // Substitute the char
    if (islower(c))
    {
        c = tolower(key[c - 97]);
        return c;
    }
    else
    {
        c = toupper(key[c - 65]);
        return c;
    }
}