//
// Created by Дмитрий on 05.05.2020.
//
#ifndef LW2_BLUR_H
#define LW2_BLUR_H

#include "bitmap_image.hpp"
#include <string>
#include <Windows.h>
#include <algorithm>
#include <vector>

using namespace std;

struct ThreadData {
    bitmap_image sourceImage;
    bitmap_image *blurImage;
    int indexStartWidth;
    int indexEndWidth;
    int indexStartHeight;
    int indexEndHeight;
    unsigned short blurRadius;
    unsigned short threadCount;
    unsigned short threadNumber;
};

class Blur {
public:
    Blur(const unsigned short threadsCount, const unsigned short coreCount);

    int BlurBmpImage(const string inputImagePath, const string outputImagePath, const unsigned short blurRadius);

private:

    unsigned short m_threadsCount;
    unsigned short m_coreCount;
};


#endif //LW2_BLUR_H
