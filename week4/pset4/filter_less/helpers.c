#include "helpers.h"
#include <math.h>
#include <stdio.h>
#include <stdlib.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            // Calculate the average value of red, green, and blue values
            int grayscale = round((round(image[i][j].rgbtRed) + round(image[i][j].rgbtGreen) + round(image[i][j].rgbtBlue)) / 3);
            
            // Set new value to red, green, and blue
            image[i][j].rgbtRed = image[i][j].rgbtGreen = image[i][j].rgbtBlue = grayscale;
            
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
            // Calculate sepia values
            int sepiaRed = round(.393 * image[i][j].rgbtRed + .769 * image[i][j].rgbtGreen + .189 * image[i][j].rgbtBlue);
            int sepiaGreen = round(.349 * image[i][j].rgbtRed + .686 * image[i][j].rgbtGreen + .168 * image[i][j].rgbtBlue);
            int sepiaBlue = round(.272 * image[i][j].rgbtRed + .534 * image[i][j].rgbtGreen + .131 * image[i][j].rgbtBlue);
        
            // Check if new values are over 255
            if (sepiaRed >= 255)
            {
                sepiaRed = 255;
            }
            
            if (sepiaGreen >= 255)
            {
                sepiaGreen = 255;
            }
            
            if (sepiaBlue >= 255)
            {
                sepiaBlue = 255;
            }
            
            // Update new sepia values
            image[i][j].rgbtRed = sepiaRed;
            image[i][j].rgbtGreen = sepiaGreen;
            image[i][j].rgbtBlue = sepiaBlue;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    // Declare temp RGBTRIPLE
    RGBTRIPLE temp[width];

    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width / 2; j++)
        {
            
            // Swap values
            temp[j] = image[i][j];
            image[i][j] = image[i][width - j - 1];
            image[i][width - j - 1 ] = temp[j];

        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{

    RGBTRIPLE image_copy[height][width];

    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            image_copy[i][j] = image[i][j];
            
        }
    }
    
    for (int i = 0; i < height; i++)
    {
        for (int j = 0; j < width; j++)
        {
            
            // Starting with the current pixel
            float avg_red = image_copy[i][j].rgbtRed;
            float avg_green = image_copy[i][j].rgbtGreen;
            float avg_blue = image_copy[i][j].rgbtBlue;
            float count = 1;
            
            
            // Calculate current row
            // If the current pixel is not the first or last in the row
            if (j > 0 && j + 1 < width)
            {
                // Calculate pixel in the next column and previous column
                avg_red += image_copy[i][j + 1].rgbtRed;
                avg_red += image_copy[i][j - 1].rgbtRed;
                avg_green += image_copy[i][j + 1].rgbtGreen;
                avg_green += image_copy[i][j - 1].rgbtGreen;
                avg_blue += image_copy[i][j + 1].rgbtBlue;
                avg_blue += image_copy[i][j - 1].rgbtBlue;
                count += 2;
                
            }
            // If the current pixel is the first pixel of the row
            else if (j == 0 && j + 1 < width)
            {
                // Calculate pixel in the next column
                avg_red += image_copy[i][j + 1].rgbtRed;
                avg_green += image_copy[i][j + 1].rgbtGreen;
                avg_blue += image_copy[i][j + 1].rgbtBlue;
                count += 1;
            }
            
            // If the current pixel is the last pixel in the row
            else if (j > 0 && (j + 1 > width || j == width - 1))
            {

                avg_red += image_copy[i][j - 1].rgbtRed;
                avg_green += image_copy[i][j - 1].rgbtGreen;
                avg_blue += image_copy[i][j - 1].rgbtBlue;
                count += 1;
            }
            
            // Calculate previous row
            // If the current pixel is not the first or last in the row
            if (i > 0 && j > 0 && j + 1 < width)
            {
                
                avg_red += image_copy[i - 1][j + 1].rgbtRed;
                avg_red += image_copy[i - 1][j - 1].rgbtRed;
                avg_red += image_copy[i - 1][j].rgbtRed;
                
                avg_green += image_copy[i - 1][j + 1].rgbtGreen;
                avg_green += image_copy[i - 1][j - 1].rgbtGreen;
                avg_green += image_copy[i - 1][j].rgbtGreen;
                
                avg_blue += image_copy[i - 1][j + 1].rgbtBlue;
                avg_blue += image_copy[i - 1][j - 1].rgbtBlue;
                avg_blue += image_copy[i - 1][j].rgbtBlue;
                
                count += 3;
            }
            // If the current pixel is the last pixel in the row
            else if (i > 0 && j > 0 && (j + 1 > width || j == width - 1))
            {
                
                avg_red += image_copy[i - 1][j - 1].rgbtRed;
                avg_red += image_copy[i - 1][j].rgbtRed;
                
                avg_green += image_copy[i - 1][j - 1].rgbtGreen;
                avg_green += image_copy[i - 1][j].rgbtGreen;
                
                avg_blue += image_copy[i - 1][j - 1].rgbtBlue;
                avg_blue += image_copy[i - 1][j].rgbtBlue;
                
                count += 2;
            }
            // If the current pixel is the first pixel of the row
            else if (i > 0 && j == 0 && j + 1 < width)
            {
                
                avg_red += image_copy[i - 1][j + 1].rgbtRed;
                avg_red += image_copy[i - 1][j].rgbtRed;
                
                avg_green += image_copy[i - 1][j + 1].rgbtGreen;
                avg_green += image_copy[i - 1][j].rgbtGreen;
                
                avg_blue += image_copy[i - 1][j + 1].rgbtBlue;
                avg_blue += image_copy[i - 1][j].rgbtBlue;
                
                count += 2;
            }
            
            // Calculate next row
            // If the current pixel is not the first or last in the row
            if (i + 1 < height && j > 0 && j + 1 < width)
            {
                
                avg_red += image_copy[i + 1][j + 1].rgbtRed;
                avg_red += image_copy[i + 1][j - 1].rgbtRed;
                avg_red += image_copy[i + 1][j].rgbtRed;
                
                avg_green += image_copy[i + 1][j + 1].rgbtGreen;
                avg_green += image_copy[i + 1][j - 1].rgbtGreen;
                avg_green += image_copy[i + 1][j].rgbtGreen;
                
                avg_blue += image_copy[i + 1][j + 1].rgbtBlue;
                avg_blue += image_copy[i + 1][j - 1].rgbtBlue;
                avg_blue += image_copy[i + 1][j].rgbtBlue;
                
                count += 3;
            }
            // If the current pixel is the last pixel in the row
            else if (i + 1 < height && j > 0 && (j + 1 > width || j == width - 1))
            {
                
                avg_red += image_copy[i + 1][j - 1].rgbtRed;
                avg_red += image_copy[i + 1][j].rgbtRed;
                
                avg_green += image_copy[i + 1][j - 1].rgbtGreen;
                avg_green += image_copy[i + 1][j].rgbtGreen;
                
                avg_blue += image_copy[i + 1][j - 1].rgbtBlue;
                avg_blue += image_copy[i + 1][j].rgbtBlue;
                
                count += 2;
            }
            // If the current pixel is the first pixel of the row
            else if (i + 1 < height && j == 0 && j + 1 < width)
            {
                
                avg_red += image_copy[i + 1][j + 1].rgbtRed;
                avg_red += image_copy[i + 1][j].rgbtRed;
                
                avg_green += image_copy[i + 1][j + 1].rgbtGreen;
                avg_green += image_copy[i + 1][j].rgbtGreen;
                
                avg_blue += image_copy[i + 1][j + 1].rgbtBlue;
                avg_blue += image_copy[i + 1][j].rgbtBlue;
                
                count += 2;
            }
            
            // Calculate average values
            int calc_avg_red = round(avg_red / count);
            if (calc_avg_red >= 255)
            {
                image[i][j].rgbtRed = 255;
            }
            else 
            {
                image[i][j].rgbtRed = calc_avg_red;
            }
            
            int calc_avg_green = round(avg_green / count);
            if (calc_avg_green >= 255)
            {
                image[i][j].rgbtGreen = 255;
            }
            else
            {
                image[i][j].rgbtGreen = calc_avg_green;
            }
            
            int calc_avg_blue = round(avg_blue / count);
            if (calc_avg_blue >= 255)
            {
                image[i][j].rgbtBlue = 255;
            }
            else 
            {
                image[i][j].rgbtBlue = calc_avg_blue;
            }
            

        }
    }
    
    return;
}
