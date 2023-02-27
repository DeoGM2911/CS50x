#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK 512

// define BYTE datatype
typedef uint8_t BYTE;

int main(int argc, char *argv[])
{
    // Ensure correct usage
    if (argc != 2)
    {
        printf("Usage: ./recover image\n");
        return 1;
    }

    // Open the file
    FILE *mem_card = fopen(argv[1], "r");
    if (mem_card == NULL)
    {
        printf("Can't not open the image.\n");
        return 1;
    }

    // Read 512B at a time and restore
    BYTE buffer[512];
    int i = 0;
    char f[8] = {0};
    FILE *image = NULL;
    while (fread(buffer, BLOCK, 1, mem_card) == 1)
    {
        // Encounter a new image -> close the old one and open a new one
        if ((buffer[0] == 0xff) && (buffer[1] == 0xd8) && (buffer[2] == 0xff)
            && (0xe0 <= buffer[3]) && (buffer[3] <= 0xef))
        {
            if (image != NULL)
            {
                fclose(image);
            }
            
            // i++ to move on to the next file
            sprintf(f, "%03d.jpg", i++);
            image = fopen(f, "w");
        }

        // If not a new image, continue writing
        if (image != NULL)
        {
            fwrite(buffer, BLOCK, 1, image);
        }
    }

    // Close the most recent image
    if (image != NULL)
    {
        fclose(image);
    }
    

    // Avoid leaks
    fclose(mem_card);
    return 0;
}
