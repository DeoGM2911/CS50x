#include "helpers.h"
#include <math.h>

// Initialize constants for detecting edges
int Gx[3][3] = {{-1, 0, 1}, {-2, 0, 2}, {-1, 0, 1}};
int Gy[3][3] = {{-1, -2, -1}, {0, 0, 0}, {1, 2, 1}};

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
            }
            else if (c == 'g')
            {
                sum += copy[row][col].rgbtGreen;
            }
            else
            {
                sum += copy[row][col].rgbtBlue;
            }
            count++;
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

int gxy_calc(int i, int j, int height, int width, RGBTRIPLE copy[height][width], char c)
{
    int sum_gx = 0;
    int sum_gy = 0;
    // Calculate the weighted sum
    for (int row = i - 1; row <= i + 1; row++)
    {
        for (int col = j - 1; col <= j + 1; col++)
        {
            // Ignore pixel not in the image
            if (row < 0 || row > height - 1 || col < 0 || col > width - 1)
            {
                continue;
            }
            // Split into 3 colors
            if (c == 'r')
            {
                sum_gx += copy[row][col].rgbtRed * Gx[row + 1 - i][col + 1 - j];
                sum_gy += copy[row][col].rgbtRed * Gy[row + 1 - i][col + 1 - j];
            }
            if (c == 'g')
            {
                sum_gx += copy[row][col].rgbtGreen * Gx[row + 1 - i][col + 1 - j];
                sum_gy += copy[row][col].rgbtGreen * Gy[row + 1 - i][col + 1 - j];
            }
            if (c == 'b')
            {
                sum_gx += copy[row][col].rgbtBlue * Gx[row + 1 - i][col + 1 - j];
                sum_gy += copy[row][col].rgbtBlue * Gy[row + 1 - i][col + 1 - j];
            }
        }
    }
    // Calculate the final value using sobel operation
    int gxy = round(pow(pow(sum_gx, 2) + pow(sum_gy, 2), 0.5));
    if (gxy > 255)
    {
        gxy = 255;
    }
    return gxy;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
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
    // Detecting edges in the image
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image[i][j].rgbtRed = gxy_calc(i, j, height, width, temp, 'r');
            image[i][j].rgbtGreen = gxy_calc(i, j, height, width, temp, 'g');
            image[i][j].rgbtBlue = gxy_calc(i, j, height, width, temp, 'b');
        }   
    }
    return;
}
