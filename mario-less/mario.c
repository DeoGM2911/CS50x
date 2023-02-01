#include <cs50.h>
#include <stdio.h>

void build(int h, string c);

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
        build(i, c1);
    }
}

void build(int h, string s)
{
    for (int k = 1; k <= h; k++)
    {
        printf("%s", s);
    }
    printf("\n");
}