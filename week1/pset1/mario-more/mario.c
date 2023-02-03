#include <cs50.h>
#include <stdio.h>

void build(int n, int h, string c);

int main(void)
{
    // Get the height
    int n;
    do
    {
        n = get_int("Height: ");
    }
    while (n < 1 || n > 8);

    // Build the pyramid
    string c1 = "#";
    for (int i = 1; i <= n; i++)
    {
        build(n, i, c1);
    }
}

void build(int n, int h, string s)
{
    // Print the spaces out first
    for (int u = 1; u <= (n - h); u++)
    {
        printf(" ");
    }
    // Print out the hashes a.k.a. the bricks
    for (int k = 1; k <= h; k++)
    {
        printf("%s", s);
    }
    // End pyramid No.1 with 2 spaces
    printf("  ");
    // Repeat the process but backward
    for (int v = 1; v <= h; v++)
    {
        printf("%s", s);
    }
    printf("\n");
}