#include <iostream>
#include <fstream>
#include <vector>
#include <unordered_map>
#include <limits.h>
#include <cfloat>

#include "BMP.hpp"


const double gamma = 0.9;

const int actualHeight = 16, actualWidth = 8,
chunkHeight = 1, chunkWidth = 1;  // Errors when lowering chunk size
const double brightness_div = chunkHeight * chunkWidth;
const int imageScale = 2;


std::vector<int> getChars()
{
    std::vector<int> vec;
    for (int i = 14; i < 27; i++)
        vec.push_back(i);
    for (int i = 28; i <= 254; i++)
        vec.push_back(i);
    return vec;
}


void printCharsWithNums()
{
    for (int i : getChars())
        std::cout << i << ": " << (char)i << "\n";
}


void printChars()
{
    for (int i : getChars())
        std::cout << (char)i;
}


std::unordered_map<char, std::vector<double>> readCharsMap(std::string path)
{
    bitmap_image image(path);

    if (!image)
    {
        std::cout << "Error - Failed to open: input.bmp\n";
    }

    unsigned int total_number_of_pixels = 0;

    const unsigned int height = image.height();
    const unsigned int width = image.width();

    std::vector<int> chars = getChars();
    std::unordered_map<char, std::vector<double>> brightnesses;
    for (int idx = 0; idx < chars.size(); idx++)
    {
        std::vector<double> brightness((actualHeight / chunkHeight) * (actualWidth / chunkWidth), 0.);
        for (int y = 0; y < actualHeight; y++)
            for (int x = 0; x < 8; x++)
            {
                rgb_t pixel = image.get_pixel(idx * actualWidth + x, y);
                double br = (pixel.blue + pixel.green + pixel.red) / brightness_div;
                brightness[y / chunkHeight * actualWidth / chunkWidth + x / chunkWidth] += br;
            }

        brightnesses.insert(make_pair(chars[idx], brightness));
    }

    return brightnesses;
}


std::vector<std::vector<double>> readImage(std::string path, int& charCountY, int& charCountX)
{
    bitmap_image image(path);

    if (!image)
    {
        std::cout << "Error - Failed to open: image.bmp\n";
    }

    const unsigned int height = image.height();
    const unsigned int width = image.width();

    charCountY = height / actualHeight;
    charCountX = width / actualWidth;

    std::vector<std::vector<double>> chunks(charCountY * charCountX);
    for (int chY = 0; chY < charCountY; chY++)
    {
        for (int chX = 0; chX < charCountX; chX++)
        {
            std::vector<double> brightness((actualHeight / chunkHeight) * (actualWidth / chunkWidth), 0.);

            for (int y = 0; y < actualHeight; y++)
            {
                for (int x = 0; x < actualWidth; x++)
                {
                    rgb_t pixel = image.get_pixel(chX * actualWidth + x, chY * actualHeight + y);
                    double br = (pixel.blue + pixel.green + pixel.red) / brightness_div * gamma;
                    brightness[y / chunkHeight * actualWidth / chunkWidth + x / chunkWidth] += br;
                }
            }

            chunks[chY * charCountX + chX] = brightness;
        }
    }

    return chunks;
}


std::string convertToString(std::unordered_map<char, std::vector<double>> chars, std::vector<std::vector<double>> image, int charCountY, int charCountX)
{
    std::string out = "";

    for (int y = 0; y < charCountY; y++)
    {
        for (int x = 0; x < charCountX; x++)
        {
            char best = 0;
            double bestDiff = DBL_MAX / 1.1;

            std::vector<double> chunk = image[y * charCountX + x];

            for (auto& [chr, brightness] : chars)
            {
                double diff = 0;
                for (int i = 0; i < chunk.size(); i++)
                    diff += abs(chunk[i] - brightness[i]);

                if (diff < bestDiff)
                {
                    bestDiff = diff;
                    best = chr;
                }
            }

            out += best;
        }
        out += "\n";
    }

    return out;
}



int main()
{
    auto chars = readCharsMap(".\\chars.bmp");

    int chY, chX;
    auto img = readImage(".\\shrek.bmp", chY, chX);

    std::string output = convertToString(chars, img, chY, chX);

    system("chcp 1252");
    std::cout << output;


    std::ofstream ofstr;
    ofstr.open(".\\shrek.txt");
    ofstr << output;
    ofstr.close();


    return 0;
}