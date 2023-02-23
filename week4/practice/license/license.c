/* The original error is that when writing paltes[idx] = buffer, we are refering (not copy)
plates[idx] to buffer -> any change happens to buffer will change plate[idx]
Solution: allocate memory for plates[idx] and then strcpy(plates[idx], buffer)
, freeing the memory before finishing main!*/

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

int main(int argc, char *argv[])
{
    // Check for command line args
    if (argc != 2)
    {
        printf("Usage: ./read infile\n");
        return 1;
    }

    // Create buffer to read into
    char buffer[7];

    // Create array to store plate numbers
    char *plates[8];
    // Allocate memory for each string
    for (int i = 0; i < 8; i++)
    {
        plates[i] = malloc(7);
    }
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        return 2;
    }
    // Copy into plates
    int idx = 0;

    while (fread(buffer, 1, 7, infile) == 7)
    {
        // Replace '\n' with '\0'
        buffer[6] = '\0';

        // Save plate number in array
        strcpy(plates[idx], buffer);
        idx++;
    }

    for (int i = 0; i < 8; i++)
    {
        printf("%s\n", plates[i]);
    }
    // Free the memory
    for (int i = 0; i < 8; i++)
    {
        free(plates[i]);
    }
    fclose(infile);
    return 0;
}