#include "helpers.h"
#include <math.h>

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

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Store copies of the original values
            BYTE originalRed = image[i][j].rgbtRed;
            BYTE originalGreen = image[i][j].rgbtGreen;
            BYTE originalBlue = image[i][j].rgbtBlue;
            // Converting to sepia
            image[i][j].rgbtRed = round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue);
            image[i][j].rgbtGreen = round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue);
            image[i][j].rgbtBlue = round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue);
            // Make sure no value exceed 255
            if (round(.393 * originalRed + .769 * originalGreen + .189 * originalBlue) > 255)
            {
                image[i][j].rgbtRed = 255;
            }
            if (round(.349 * originalRed + .686 * originalGreen + .168 * originalBlue) > 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            if (round(.272 * originalRed + .534 * originalGreen + .131 * originalBlue) > 255)
            {
                image[i][j].rgbtBlue = 255;
            }
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
    RGBTRIPLE avg;
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Consider corners
            // Top right
            if ((i == 0) && (j == 0))
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i + 1][j].rgbtRed +
                                     temp[i][j + 1].rgbtRed + temp[i + 1][j +  1].rgbtRed) / 4.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i + 1][j].rgbtGreen +
                                       temp[i][j + 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) / 4.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i + 1][j].rgbtBlue + 
                                      temp[i][j + 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) / 4.0);
                image[i][j] = avg;
            }
            // Bottom left
            if ((i == height - 1) && (j == width - 1))
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i - 1][j].rgbtRed + 
                                     temp[i][j - 1].rgbtRed + temp[i - 1][j - 1].rgbtRed) / 4.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i - 1][j].rgbtGreen +
                                       temp[i][j - 1].rgbtGreen + temp[i - 1][j - 1].rgbtGreen) / 4.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i - 1][j].rgbtBlue +
                                      temp[i][j - 1].rgbtBlue + temp[i - 1][j - 1].rgbtBlue) / 4.0);
                image[i][j] = avg;
            }
            // Bottom right 
            if ((i == height - 1) && (j == 0))
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i - 1][j].rgbtRed + 
                                     temp[i][j + 1].rgbtRed + temp[i - 1][j + 1].rgbtRed) / 4.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i - 1][j].rgbtGreen + 
                                       temp[i][j + 1].rgbtGreen + temp[i - 1][j + 1].rgbtGreen) / 4.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i - 1][j].rgbtBlue + 
                                      temp[i][j + 1].rgbtBlue + temp[i - 1][j + 1].rgbtBlue) / 4.0);
                image[i][j] = avg;
            }
            // Top left 
            if ((i == 0) && (j == width - 1))
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i + 1][j].rgbtRed + 
                                     temp[i][j - 1].rgbtRed + temp[i + 1][j - 1].rgbtRed) / 4.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i + 1][j].rgbtGreen + 
                                       temp[i][j - 1].rgbtGreen + temp[i + 1][j - 1].rgbtGreen) / 4.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i + 1][j].rgbtBlue + 
                                      temp[i][j - 1].rgbtBlue + temp[i + 1][j - 1].rgbtBlue) / 4.0);
                image[i][j] = avg;
            }
            // Consider side case
            // 1st row 
            if (i == 0)
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed + temp[i][j + 1].rgbtRed + 
                                     temp[i + 1][j].rgbtRed + temp[i + 1][j - 1].rgbtRed + temp[i + 1][j + 1].rgbtRed) / 6.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen + temp[i][j + 1].rgbtGreen + 
                                       temp[i + 1][j].rgbtGreen + temp[i + 1][j - 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) / 6.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue + temp[i][j + 1].rgbtBlue + 
                                      temp[i + 1][j].rgbtBlue + temp[i + 1][j - 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) / 6.0);
                image[i][j] = avg;
            }
            // 1st column
            if (j == 0)
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i + 1][j].rgbtRed + temp[i - 1][j].rgbtRed + 
                                     temp[i][j + 1].rgbtRed + temp[i + 1][j + 1].rgbtRed + temp[i - 1][j + 1].rgbtRed) / 6.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i + 1][j].rgbtGreen + temp[i - 1][j].rgbtGreen + 
                                       temp[i][j + 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen + temp[i - 1][j + 1].rgbtGreen) / 6.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i + 1][j].rgbtBlue + temp[i - 1][j].rgbtBlue + 
                                      temp[i][j + 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue + temp[i - 1][j + 1].rgbtBlue) / 6.0);
                image[i][j] = avg;
            }
            // last row
            if (i == height - 1)
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed + temp[i][j + 1].rgbtRed + 
                                     temp[i - 1][j - 1].rgbtRed + temp[i - 1][j + 1].rgbtRed + temp[i - 1][j].rgbtRed) / 6.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen + temp[i][j + 1].rgbtGreen + 
                                       temp[i - 1][j - 1].rgbtGreen + temp[i - 1][j + 1].rgbtGreen + temp[i - 1][j].rgbtGreen) / 6.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue + temp[i][j + 1].rgbtBlue + 
                                      temp[i - 1][j - 1].rgbtBlue + temp[i - 1][j + 1].rgbtBlue + temp[i - 1][j].rgbtBlue) / 6.0);
                image[i][j] = avg;
            }
            // last column
            if (j == width - 1)
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i + 1][j].rgbtRed + temp[i - 1][j].rgbtRed +
                                     temp[i][j - 1].rgbtRed + temp[i + 1][j - 1].rgbtRed + temp[i - 1][j - 1].rgbtRed) / 6.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i + 1][j].rgbtGreen + temp[i - 1][j].rgbtGreen + 
                                       temp[i][j - 1].rgbtGreen + temp[i + 1][j - 1].rgbtGreen + temp[i - 1][j - 1].rgbtGreen) / 6.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i + 1][j].rgbtBlue + temp[i - 1][j].rgbtBlue + 
                                      temp[i][j - 1].rgbtBlue + temp[i + 1][j - 1].rgbtBlue + temp[i - 1][j - 1].rgbtBlue) / 6.0);
                image[i][j] = avg;
            }
            // Consider normal case
            else
            {
                avg.rgbtRed = round((temp[i][j].rgbtRed + temp[i][j - 1].rgbtRed + temp[i][j + 1].rgbtRed + 
                                     temp[i - 1][j - 1].rgbtRed + temp[i - 1][j].rgbtRed + temp[i - 1][j + 1].rgbtRed + 
                                     temp[i + 1][j].rgbtRed + temp[i + 1][j - 1].rgbtRed + temp[i + 1][j + 1].rgbtRed) / 9.0);
                avg.rgbtGreen = round((temp[i][j].rgbtGreen + temp[i][j - 1].rgbtGreen + temp[i][j + 1].rgbtGreen + 
                                       temp[i - 1][j - 1].rgbtGreen + temp[i - 1][j].rgbtGreen + temp[i - 1][j + 1].rgbtGreen + 
                                       temp[i + 1][j].rgbtGreen + temp[i + 1][j - 1].rgbtGreen + temp[i + 1][j + 1].rgbtGreen) / 9.0);
                avg.rgbtBlue = round((temp[i][j].rgbtBlue + temp[i][j - 1].rgbtBlue + temp[i][j + 1].rgbtBlue + 
                                      temp[i - 1][j - 1].rgbtBlue + temp[i - 1][j].rgbtBlue + temp[i - 1][j + 1].rgbtBlue + 
                                      temp[i + 1][j].rgbtBlue + temp[i + 1][j - 1].rgbtBlue + temp[i + 1][j + 1].rgbtBlue) / 9.0);
                image[i][j] = avg;
            }
        }
    }
    return;
}
