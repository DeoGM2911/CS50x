#include <stdio.h>
#include <stdint.h>
#include <stdlib.h>

#define BLOCK 512

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
    BYTE *buffer = malloc(BLOCK);
    int i = 0;
    while (fread(buffer, BLOCK, 1, mem_card) == BLOCK)
    {
        FILE *image = fopen(image_name(i), "w");
        fwrite(buffer, 512, 1, image);
        fclose(image);
        i++;
    }

    // Avoid leaks
    free(buffer);
    fclose(mem_card);
    return 0;
}

char *image_name(int i)
{
    char *s;
    if (i < 10)
    {
        sprintf(s, "00%i.jpg", i);
        return s;
    }
    else if ((10 <= i) && (i < 100))
    {
        sprintf(s, "0%i.jpg", i);
        return s;
    }
    else
    {
        sprintf(s, "%i.jpg", i);
        return s;
    }
}