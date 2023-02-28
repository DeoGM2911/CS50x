#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>

#include "wav.h"
#define HEADER_SIZE 44

bool check_format(WAVHEADER header);
int get_block_size(WAVHEADER header);

int main(int argc, char *argv[])
{
    // Ensure proper usage
    if (argc != 3)
    {
        printf("Usage: ./reverse infile outfile\n");
        return 1;
    }
    
    // Open input file for reading
    FILE *infile = fopen(argv[1], "r");
    if (infile == NULL)
    {
        printf("Can't open the file/\n");
        return 1;
    }
    
    // Read header
    WAVHEADER header;
    fread(&header, HEADER_SIZE, 1, infile);

    // Use check_format to ensure WAV format
    if (!check_format(header))
    {
        printf("Not a .wav file.\n");
        return 1;
    }

    // Open output file for writing
    FILE *outfile = fopen(argv[2], "w");
    if (outfile == NULL)
    {
        printf("Can't open output file/\n");
        return 1;
    }
    

    // Write header to file
    fwrite(&header, 1, HEADER_SIZE * sizeof(BYTE), outfile);

    // Use get_block_size to calculate size of block
    int BLOCK = get_block_size(header);

    // Write reversed audio to file
    BYTE buffer[BLOCK];
    // Set the pointer to 1 block past the last block
    fseek(infile, BLOCK, SEEK_END);
    while (ftell(infile) - BLOCK > HEADER_SIZE)
    {
        // Bring the pointer to the previous block
        if (fseek(infile, -2 * BLOCK, SEEK_CUR))
        {
            return 1;
        }
        fread(buffer, 1, BLOCK, infile);
        fwrite(buffer, 1, BLOCK, outfile);
    }
    
    fclose(outfile);
    fclose(infile);
}

bool check_format(WAVHEADER header)
{
    char s[] = "WAVE";
    for (int i = 0; i < 4; i++)
    {
        if (header.format[i] != s[i])
        {
            return false;
        }
    }
    return true;
}

int get_block_size(WAVHEADER header)
{
    // 1 byte = 8 bits
    return (header.numChannels * (header.bitsPerSample / 8));
}