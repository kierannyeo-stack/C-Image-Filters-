#include "helpers.h"
#include <math.h>

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++)
        {
            int avg = round((image[h][w].rgbtRed+image[h][w].rgbtGreen+image[h][w].rgbtBlue)/3.0);
            image[h][w].rgbtRed = avg;
            image[h][w].rgbtGreen = avg;
            image[h][w].rgbtBlue = avg;
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
            sepia[0] = round(.393 * image[h][w].rgbtRed + .769 * image[h][w].rgbtGreen + .189 * image[h][w].rgbtBlue);
            sepia[1] = round(.349 * image[h][w].rgbtRed + .686 * image[h][w].rgbtGreen + .168 * image[h][w].rgbtBlue);
            sepia[2] = round(.272 * image[h][w].rgbtRed + .534 * image[h][w].rgbtGreen + .131 * image[h][w].rgbtBlue);
            for (int n=0;n<3;n++)
            {
                if (sepia[n]>255)
                {
                    sepia[n]=255;
                }
            }
            image[h][w].rgbtRed = sepia[0];
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
            image[h][w].rgbtRed=round((double)(red/count));
            image[h][w].rgbtBlue=round((double)(blue/count));
            image[h][w].rgbtGreen=round((double)(green/count));
        }
    }
    return;
}

// Edges function - Sobel Operator

int Gx[3][3]={
    {-1,0,1},
    {-2,0,2},
    {-1,0,1}
};

int Gy[3][3]={
    {-1,-2,-1},
    {0,0,0},
    {1,2,1}
};

void edge(int height, int width, RGBTRIPLE image[height][width])
{
    //create copy of pixels
    RGBTRIPLE copy[height][width];
    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++)
        {
            copy[h][w]=image[h][w];
        }
    }

    for (int h=0;h<height;h++)
    {
        for (int w=0;w<width;w++) //index each pixel
        {
            //calculate sum of indiv pixels
            int redx = 0;
            int redy = 0;
            int greenx = 0;
            int greeny = 0;
            int bluex = 0;
            int bluey = 0;
            //algo to check if edge or normal pixel
            for (int dh=-1;dh<=1;dh++)
            {
                for (int dw=-1;dw<=1;dw++)
                {
                    // to check if pixels are within image
                    if (h+dh<height && h+dh>=0 && w+dw<width && w+dw>=0) //all base cases
                    {
                        // offset ranges [-1,1] but matrix goes from [0,2]
                        redx += copy[h+dh][w+dw].rgbtRed*Gx[1+dh][1+dw]; // iteration starts at top left pixel, so Gx equivalent is Gx[0][0]
                        redy += copy[h+dh][w+dw].rgbtRed*Gy[1+dh][1+dw];
                        greenx += copy[h+dh][w+dw].rgbtGreen*Gx[1+dh][1+dw]; // iteration starts at top left pixel, so Gx equivalent is Gx[0][0]
                        greeny += copy[h+dh][w+dw].rgbtGreen*Gy[1+dh][1+dw];
                        bluex += copy[h+dh][w+dw].rgbtBlue*Gx[1+dh][1+dw]; // iteration starts at top left pixel, so Gx equivalent is Gx[0][0]
                        bluey += copy[h+dh][w+dw].rgbtBlue*Gy[1+dh][1+dw];
                    }
                    else // if pixels are OB then multiply by 0, so
                    {
                        continue;
                    }
                }
            }

            image[h][w].rgbtRed = round(sqrt(redx*redx+redy*redy));
            if (image[h][w].rgbtRed>255)
            {
                image[h][w].rgbtRed=255;
            }

            image[h][w].rgbtBlue = round(sqrt(bluex*bluex+bluey*bluey));
            if (image[h][w].rgbtBlue>255)
            {
                image[h][w].rgbtBlue=255;
            }

            image[h][w].rgbtGreen = round(sqrt(greenx*greenx+greeny*greeny));
            if (image[h][w].rgbtGreen>255)
            {
                image[h][w].rgbtGreen=255;
            }
        }
    }
}
