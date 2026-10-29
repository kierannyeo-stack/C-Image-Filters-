#include "helpers.h"

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++)
        {
            int avg = round((image[h][w].rgbtRed+image[h][w].rgbtGreen+image[h][w].rbgtBlue)/3.0);
            image.rgbtRed[h][w] = avg;
            image.rgbtGreen[h][w] = avg;
            image.rgbtBlue[h][w] = avg;
        }
    }
    return;
}

// Convert image to sepia
void sepia(int height, int width, RGBTRIPLE image[height][width])
{
    //to index each pixel in the 2D array
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++) //compute values
        {
            int sepia[3];
            int sepia[0] = round(.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue);
            int sepia[1] = round(.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue);
            int sepia[2] = round(.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue);
            for (int n=0;n<3;n++)
            {
                if (sepia[n]>255)
                {
                    sepia[n]=255;
                }
            }
            image[h][w].rgbtRed = sepia[0]；
            image[h][w].rgbtGreen = sepia[1];
            image[h][w].rgbtBlue = sepia[2];
        }
    }
    return;
}

// Reflect image horizontally: Left pixel goes to the right and vice versa
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width/2;w++)
        {
            // Swapping pixels with memory addresses
            RGBTRIPLE *a = &image[h][w];
            RGBTRIPLE *b = &image[h][width-w-1];
            RGBTRIPLE temp = *a;
            *a = *b;
            *b = temp;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++)
        {
            //create a copy of each pixel before editing pixels
            copy[h][w]=image[h][w];
        }
    }

    // Algo to check if neighbouring pixels exist and sum evth
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++)
        {
            //each pixel needs a sum and avg
            int red=0;
            int blue=0;
            int green=0;
            int count=0;
            for (int dh=-1;dh<=1;dh++)
            {
                for (int dw=-1;dw<=1;dw++)
                {
                    //create sep variable for neighbouring pixels
                    int nh=h+dh;
                    int nw=w+dw;

                    //check if valid
                    if (nh>height-1||nh<0||nw>width-1||nw<0)
                    {
                        continue; //want to move on to next iteration
                    }
                    red+=copy[nh][nw].rgbtRed;
                    blue+=copy[nh][nw].rgbtBlue;
                    green+=copy[nh][nw].rgbtGreen;
                    count++;
                }
            }
            image.rgbtRed[h][w]=round((red/count));
            image.rgbtBlue[h][w]=round((blue/count));
            image.rgbtGreen[h][w]=round((green/count));
        }
    }
    return;
}

// Edges function - Sobel Operator
void edge(int height, int width, RGBTRIPLE image[height][width])
{
    
}
