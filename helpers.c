#include "helpers.h"
#include <math.h>

int round_maximum(float rgb);
int begin(int ij, int unit);
int end(int begin, int unit, int height, int width, int which);
int kernel(int clm, int row, int j, int i, int radius);
int plus_minus(int clm, int row, int j, int i, int which);

// Convert image to grayscale
void grayscale(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int r = image[j][i].rgbtRed;
            int g = image[j][i].rgbtGreen;
            int b = image[j][i].rgbtBlue;
            float num = (r + g + b) / 3.0;
            int average = round(num);
            image[j][i].rgbtRed = average;
            image[j][i].rgbtGreen = average;
            image[j][i].rgbtBlue = average;
        }
    }
    return;
}

// Reflect image horizontally
void reflect(int height, int width, RGBTRIPLE image[height][width])
{
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width / 2; i++)
        {
            RGBTRIPLE temporary;
            temporary = image[j][i];
            image[j][i] = image[j][width - 1 - i];
            image[j][width - 1 - i] = temporary;
        }
    }
    return;
}

// Blur image
void blur(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width]; 
    int radius = 3; 
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int begin_i = begin(i, radius);
            int begin_j = begin(j, radius);
            int end_i = end(i, radius, height, width, 2);
            int end_j = end(j, radius, height, width, 1);
            int R = 0;
            int G = 0;
            int B = 0;
            for (int clm = begin_j; clm <= end_j; clm++)
            {
                for (int row = begin_i; row <= end_i; row++)
                {
                    R = R + image[clm][row].rgbtRed;
                    G = G + image[clm][row].rgbtGreen;
                    B = B + image[clm][row].rgbtBlue;
                }
            }
            int h = end_j - begin_j + 1; //求取色范围的边长
            int w = end_i - begin_i + 1;
            float averageR = (float) R / (h * w); //我这种写法会低效吗？按理说只有四条边上的像素会出现不规则的取色范围，我这样写每一个像素都在计算自己的取值色域大小
            float averageG = (float) G / (h * w);
            float averageB = (float) B / (h * w);
            int r = round_maximum(averageR);
            int g = round_maximum(averageG);
            int b = round_maximum(averageB);
            copy[j][i].rgbtRed = r;
            copy[j][i].rgbtGreen = g;
            copy[j][i].rgbtBlue = b;
        }
    }
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            image[j][i].rgbtRed = copy[j][i].rgbtRed;
            image[j][i].rgbtGreen = copy[j][i].rgbtGreen;
            image[j][i].rgbtBlue = copy[j][i].rgbtBlue;
        }
    }
    return;
}

// Detect edges
void edges(int height, int width, RGBTRIPLE image[height][width])
{
    RGBTRIPLE copy[height][width];
    int radius = 3; //must be odd and >= 3;
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            int begin_i = begin(i, radius);
            int begin_j = begin(j, radius);
            int end_i = end(i, radius, height, width, 2);
            int end_j = end(j, radius, height, width, 1);
            
            float GxR = 0; //float赋值为0后期和其它整数做运算还需要标明（float）吗？
            float GyR = 0;
            float GxG = 0;
            float GyG = 0;
            float GxB = 0;
            float GyB = 0;

            for (int clm = begin_j; clm <= end_j; clm++)
            {
                for (int row = begin_i; row <= end_i; row++)
                {
                    int r = image[clm][row].rgbtRed;
                    int g = image[clm][row].rgbtGreen;
                    int b = image[clm][row].rgbtBlue;
                    int k = kernel(clm, row, j, i, radius); //soble kernel number
                    int x = plus_minus(clm, row, j, i, 1) * k;
                    int y = plus_minus(clm, row, j, i, 2) * k;
                    GxR = GxR + r * x;
                    GyR = GyR + r * y;
                    GxG = GxG + g * x;
                    GyG = GyG + g * y;
                    GxB = GxB + b * x;
                    GyB = GyB + b * y;
                }
            }
            int red = round_maximum(sqrt(GxR * GxR + GyR * GyR));
            int green = round_maximum(sqrt(GxG * GxG + GyG * GyG));
            int blue = round_maximum(sqrt(GxB * GxB + GyB * GyB));
            copy[j][i].rgbtRed = red;
            copy[j][i].rgbtGreen = green;
            copy[j][i].rgbtBlue = blue; 
        }
    }
    for (int j = 0; j < height; j++)
    {
        for (int i = 0; i < width; i++)
        {
            image[j][i].rgbtRed = copy[j][i].rgbtRed;
            image[j][i].rgbtGreen = copy[j][i].rgbtGreen;
            image[j][i].rgbtBlue = copy[j][i].rgbtBlue;
        }
    }
    return;
}

int round_maximum(float rgb)
{
    int n = round(rgb);
    if (n > 255)
    {
        n = 255;
    }
    return n;
}

int begin(int ij, int radius)
{
    int n = ij - (radius - 1) / 2;
    if (n < 0)
    {
        n = 0;
    }
    return n;
}

int end(int ij, int radius, int height, int width, int which)
{
    int begin = ij - (radius - 1) / 2;
    int n = begin + radius - 1;
    if (which == 1) //==才是比较！==才是比较！==才是比较！==才是比较！==才是比较！！！
    {
        if (n > height - 1)
        {
            n = height - 1;
        }
    }
    if (which == 2)
    {
        if (n > width - 1)
        {
            n = width - 1;
        }
    }
    return n;
}

int kernel(int clm, int row, int j, int i, int radius)  //RGBTRIPLE image可以只输入image不输入长宽吗？我计算好了所有的ij值不会访问过界
{
    int kernel = 0;
    if (clm <= j && row <= i)
    {
        kernel = (radius - ((i + j) - (row + clm)));
        return kernel;
    }
    else if (clm <= j && row > i)
    {
        row = i - (row - i);
        kernel = (radius - ((i + j) - (row + clm)));
        return kernel;
    }
    else if (clm > j && row <= i)
    {
        clm = j - (clm - j);
        kernel = (radius - ((i + j) - (row + clm)));
        return kernel;
    }
    else
    {
        row = i - (row - i);
        clm = j - (clm - j);
        kernel = (radius - ((i + j) - (row + clm)));
        return kernel;
    }
}

int plus_minus(int clm, int row, int j, int i, int which)
{
    int n = 0;
    if (which == 1) //Gx
    {
        if (row < i)
        {
            n = -1;
            return n;
        }
        else if (row > i)
        {
            n = 1;
            return n;
        }
        else
        {
            return n;
        }
    }
    if (which == 2) //Gy
    {
        if (clm < j)
        {
            n = -1;
            return n;
        }
        else if (clm > j)
        {
            n = 1;
            return n;
        }
        else
        {
            return n;
        }
    }
}