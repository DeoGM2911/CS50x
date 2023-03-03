// Implements a dictionary's functionality

#include <stdio.h>
#include <string.h>
#include <ctype.h>
#include <strings.h>
#include <stdbool.h>
#include <stdlib.h>

#include "dictionary.h"

// Represents a node in a hash table
typedef struct node
{
    char word[LENGTH + 1];
    struct node *next;
}
node;

// TODO: Choose number of buckets in hash table
// The number of buckets determined by 1st char, 2nd char and the length of the word
// There are around 26 * (28 + 44) blank rows -> Acceptable (around 1 KB of memory) 
const unsigned int N = 26 * 45 * 28;

// Count words
int count;

// Hash table
node *table[N];

// Returns true if word is in dictionary, else false
bool check(const char *word)
{
    // TODO
    int key = hash(word);
    node *ptr = table[key];
    while (ptr != NULL)
    {
        if (strcasecmp(word, ptr->word) == 0)
        {
            return true;
        }
        ptr = ptr->next;
    }
    return false;
}

// Hashes word to a number
unsigned int hash(const char *word)
{
    // TODO: Improve this hash function
    int len = strlen(word);
    int first = toupper(word[0]) - 'A';
    int second;
    if (isalpha(word[1]))
    {
        second = toupper(word[1]) - 'A' + 1;
    }
    else
    {
        // 26 means an aspotrophe, 0 means strlen == 1
        second = (word[1] == '\0') ? 0 : 27;
    }
    int hashKey = ((first + 1) * 28 * 45 - 1) + (second * 45) + len;
    return hashKey;
}

// Loads dictionary into memory, returning true if successful, else false
bool load(const char *dictionary)
{
    // Initialize the table
    for (int i = 0; i < N; i++)
    {
        table[i] = NULL;
    }
    // Open dictionary
    FILE *dict = fopen(dictionary, "r");
    if (dict == NULL)
    {
        return false;
    }
    
    char s[LENGTH + 1];
    // create a new node and link to the table
    while (fscanf(dict, "%s", s) != EOF)
    {
        // Create a new node
        node *new = malloc(sizeof(node));
        if (new == NULL)
        {
            return false;
        }

        strcpy(new->word, s);
        new->next = NULL;
        int key = hash(new->word);

        // If this is the first element
        if (table[key] == NULL)
        {
            table[key] = new;
        }

        // If there are existing words in this row
        else
        {
            new->next = table[key];
            table[key] = new;
        }
        count++;
    }
    fclose(dict);
    return true;
}

// Returns number of words in dictionary if loaded, else 0 if not yet loaded
unsigned int size(void)
{
    return count;
}

// Unloads dictionary from memory, returning true if successful, else false
bool unload(void)
{
    // TODO
    for (int i = 0; i < N; i++)
    {
        while (table[i] != NULL)
        {
            node *tmp = table[i]->next;
            free(table[i]);
            table[i] = tmp;
        }
    }
    return true;
}
