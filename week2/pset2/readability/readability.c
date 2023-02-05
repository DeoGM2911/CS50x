// Everu one space are there two words
// Every period/ exclaimation mark/ question mark is there 1 sentence
// Goal: calculate average word's length and sentence's length

#include <cs50.h>
#include <stdio.h>
#include <string.h>
#include <math.h>
#include <ctype.h>

int count_letters(string text);
int count_words(string text);
int count_sentences(string text);

int main(void)
{
    // Get the text
    string text = get_string("Text: ");
    int L = count_letters(text);
    int W = count_words(text);
    int sen = count_sentences(text);
    // Counting letters, words, and sentences
    // Calculate the average letter per word
    // Round the index by making it an int
    double index = round(0.0588 * (L / (float) W) * 100 - 0.296 * (sen / (float) W) * 100 - 15.8);
    if (index >= 16)
    {
        printf("Grade 16+\n");
    }
    else if (index < 1)
    {
        printf("Before Grade 1\n");
    }
    else
    {
        printf("Grade %.0f\n", index);
    }
}

int count_letters(string text)
{
    int avr_let = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Count the number of word by counting the number of whitespaces
        if (isupper(text[i]) || islower(text[i]))
        {
            avr_let++;
        }
    }
    return avr_let;
}

int count_words(string text)
{
    int avr_word = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Count the number of word by counting the number of whitespaces
        if (isspace(text[i]))
        {
            avr_word++;
        }
    }
    // Add 1 to the the counter of word for the LAST word
    avr_word++;
    return avr_word;
}

int count_sentences(string text)
{
    int tot_sen = 0;
    for (int i = 0, n = strlen(text); i < n; i++)
    {
        // Count the number of sentence by counting the number of '.' or '!' or '?'
        if (text[i] == 33 || text[i] == 63 || text[i] == 46)
        {
            tot_sen++;
        }
    }
    return tot_sen;
}
