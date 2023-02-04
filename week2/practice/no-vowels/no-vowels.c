#include <cs50.h>
#include <stdio.h>

string replace(string word);

int main(int argc, string argv[])
{
    // Check if there are 2 command-line arguments
    if (argc != 2)
    {
        printf("Usage: ./no-vowels word\n");
        return 1;
    }
    // Print out the "no-vowels" version
    printf("%s\n", replace(argv[1]));
    return 0;
}

string replace(string word)
{
    int i = 0;
    // Loop through until the end of the string ('\0')
    while (word[i] != '\0')
    {
        // Check each case for each char
        // The prompt doesn't say anything about uppercased vowels so I skipped them
        switch (word[i])
        {
            case 'a':
                word[i] = '6';
                break;
            case 'e':
                word[i] = '3';
                break;
            case 'i':
                word[i] = '1';
                break;
            case 'o':
                word[i] = '0';
                break;
            // The default also cover the letter 'u'
            default:
                break;
        }
        i++;
    }
    return word;
}