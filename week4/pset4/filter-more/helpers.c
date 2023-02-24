#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Take the floor division to calculate the average
            BYTE avg = round((image[i][j].rgbtRed + image[i][j].rgbtGreen + image[i][j].rgbtBlue) / 3.0);
            // Split the line for readability
            image[i][j].rgbtRed = avg;
            image[i][j].rgbtGreen = avg;
            image[i][j].rgbtBlue = avg;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < (width / 2); j++)
    {
        // Swap element in which thir heights add up to height - 1, the middle elements stay the same
        for (int i = 0; i < height; i++)
        {
            RGBTRIPLE temp_pixel = image[i][j];
            image[i][j] = image[i][width - 1 - j];
            image[i][width - 1 - j] = temp_pixel;
        }
    }
    return;
}

// Calculate the average value for RGB
BYTE avg_box(int i, int j, int height, int width, RGBTRIPLE copy[height][width], char c)
{
    int sum = 0;
    float count = 0.0;
    // Loop through a 3x3 box centered at (i,j)
    for (int row = i - 1; row <= i + 1; row++)
    {
        for (int col = j - 1; col <= j + 1; col++)
        {
            // Ignore pixel not in the image
            if (row < 0 || row > height - 1 || col < 0 || col > width - 1)
            {
                continue;
            }
            if (c == 'r')
            {
                sum += copy[row][col].rgbtRed;
                count++;
            }
            else if (c == 'g')
            {
                sum += copy[row][col].rgbtGreen;
                count++;
            }
            else
            {
                sum += copy[row][col].rgbtBlue;
                count++;
            }
        }
    }
    return (BYTE)(round(sum / count));
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    // Create a copy of the image
    RGBTRIPLE temp[height][width];
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            temp[i][j] = image[i][j];
        }
    }
    // Bluring by taking average of neighbors
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = avg_box(i, j, height, width, temp, 'r');
            image[i][j].rgbtGreen = avg_box(i, j, height, width, temp, 'g');
            image[i][j].rgbtBlue = avg_box(i, j, height, width, temp, 'b');
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    return;
}
