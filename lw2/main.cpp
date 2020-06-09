//
// Created by Дмитрий on 05.05.2020.
//

#include <ctime>
#include <iostream>
#include "Blur.h"

using namespace std;

/**
 * init program
 *
 * @param argc
 * @param argv
 * @return int
 */
int main(int argc, char* argv[])
{
    clock_t startTime = clock();
    if (argc != 6) {
        cout << "Not enough arguments" << endl;
        cout << "Use: " << argv[0] << " <input.bmp> <output.bmp> <number threads> <number core> <blur radius>" << endl;
        return 1;
    }

    string inputImage;
    string outputImage;
    unsigned short threadsCount;
    unsigned short coreCount;
    unsigned short blurRadius;

    try {
        inputImage = argv[1];
        outputImage = argv[2];
        threadsCount = stoi(argv[3]);
        coreCount = stoi(argv[4]);
        blurRadius = stoi(argv[5]);
    } catch (const invalid_argument) {
        cout << "Invalid argument" << endl;
        cout << "Use: " << argv[0] << " <input.bmp> <output.bmp> <number threads> <number core> <blur radius>" << endl;
        return 1;
    }

    Blur blurClass(threadsCount, coreCount);
    blurClass.BlurBmpImage(inputImage, outputImage, blurRadius);

    clock_t endTime = clock();
    cout << "Runtime: " << difftime(endTime, startTime) << " ms" << endl;
    return 0;
}