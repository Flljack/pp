//
// Created by Дмитрий on 05.05.2020.
//

#include "Blur.h"

/**
 * init blur class
 *
 * @param threadsCount
 * @param coreCount
 */
Blur::Blur(const unsigned short threadsCount, const unsigned short coreCount) {
    m_threadsCount = threadsCount;
    m_coreCount = coreCount;
}

/**
 * main function for blur
 *
 * @param thread
 */
void BlurPixels(ThreadData &thread) {
    for (int height = thread.indexStartHeight; height < thread.indexEndHeight; ++height) {
        for (int width = thread.indexStartWidth; width < thread.indexEndWidth; ++width) {
            int totalRed = 0, totalGreen = 0, totalBlue = 0, pixelCount = 0;

            for (int i = height - thread.blurRadius; i <= height + thread.blurRadius; ++i) {
                for (int j = width - thread.blurRadius; j <= width + thread.blurRadius; ++j) {
                    int minHeight = min(thread.indexEndHeight - 1, max(i, 0));
                    int minWidth = min(thread.indexEndWidth - 1, max(j, 0));

                    rgb_t pixel = thread.sourceImage.get_pixel(minWidth, minHeight);

                    totalRed += pixel.red;
                    totalGreen += pixel.green;
                    totalBlue += pixel.blue;
                    pixelCount++;
                }
            }

            rgb_t colour;
            colour.red = (unsigned char) (totalRed / pixelCount);
            colour.green = (unsigned char) (totalGreen / pixelCount);
            colour.blue = (unsigned char) (totalBlue / pixelCount);

            thread.blurImage->set_pixel(width, height, colour);
        }
    }
}

/**
 * thread function
 *
 * @param lpParam
 * @return
 */
DWORD WINAPI ThreadProc(CONST LPVOID lpParam) {
    ThreadData *thread = (ThreadData *) lpParam;

    int width = thread->sourceImage.width();
    int height = thread->sourceImage.height();
    unsigned short threadCount = thread->threadCount;
    unsigned short threadNumber = thread->threadNumber;

    int partWidth = width / threadCount;
    int partHeight = height / threadCount;

    for (size_t i = 0; i < threadCount; ++i) {
        int endHeight = (threadNumber + 1) * partHeight;
        thread->indexStartHeight = threadNumber * partHeight;
        thread->indexEndHeight = endHeight + (threadNumber == threadCount - 1 ? height - endHeight : 0);

        int endWidth = (i + 1) * partWidth;
        thread->indexStartWidth = i * partWidth;
        thread->indexEndWidth = endWidth + (i == threadCount - 1 ? width - endWidth : 0);

        BlurPixels(*thread);
    }
    ExitThread(0);
}

/**
 * blur bmp image
 *
 * @param inputImage
 * @param outputImage
 * @param blurRadius
 * @return
 */
int Blur::BlurBmpImage(const string inputImagePath, const string outputImagePath, const unsigned short blurRadius) {

    bitmap_image sourceImage(inputImagePath);
    if (!sourceImage) {
        return 1;
    }

    bitmap_image blurImage(sourceImage);

    vector<ThreadData> threadsData;

    for (int i = 0; i < m_threadsCount; ++i) {
        ThreadData threadData;
        threadData.sourceImage = sourceImage;
        threadData.blurImage = &blurImage;
        threadData.blurRadius = blurRadius;
        threadData.threadCount = m_threadsCount;
        threadData.threadNumber = i;

        threadsData.push_back(threadData);
    }

    vector<HANDLE> handles(threadsData.size());
    int affinityMask = (1 << m_coreCount) - 1;
    for (size_t i = 0; i < threadsData.size(); ++i) {
        handles[i] = CreateThread(NULL, 0, &ThreadProc, &threadsData[i], CREATE_SUSPENDED, NULL);
        SetThreadAffinityMask(handles[i], affinityMask);
    }

    for (const auto &handle : handles) {
        ResumeThread(handle);
    }

    WaitForMultipleObjects((DWORD) handles.size(), handles.data(), true, INFINITE);
    blurImage.save_image(outputImagePath);
    return 0;
}