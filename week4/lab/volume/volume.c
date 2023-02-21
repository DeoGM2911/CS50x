// Modifies the volume of an audio file

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

// Number of bytes in .wav header
const int HEADER_SIZE = 44;

int main(int argc, char *argv[])
{
    // Check command-line arguments
    if (argc != 4)
    {
        printf("Usage: ./volume input.wav output.wav factor\n");
        return 1;
    }

    // Open files and determine scaling factor
    FILE *input = fopen(argv[1], "r");
    if (input == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    FILE *output = fopen(argv[2], "w");
    if (output == NULL)
    {
        printf("Could not open file.\n");
        return 1;
    }

    float factor = atof(argv[3]);

    // Copy header from input file to output file
    uint8_t header[HEADER_SIZE];
    // Write the header into the header array
    fread(&header, HEADER_SIZE, 1, input);
    // Extract data to output from the header array
    fwrite(&header, HEADER_SIZE, 1, output);

    // Read samples from input file and write updated data to output file
    int16_t buffer;
    // Read EACH sample (already read the header file) and write it to the output file
    // No more input -> return 0 -> exit the loop
    while (fread(&buffer, sizeof(int16_t), 1, input))
    {
        // Update the volume
        buffer *= factor;
        fwrite(&buffer, sizeof(int16_t), 1, output);
    }
    
    // Close files
    fclose(input);
    fclose(output);
}
