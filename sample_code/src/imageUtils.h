#ifndef CPP_CODE_IMAGEUTILS_H
#define CPP_CODE_IMAGEUTILS_H

#include <opencv2/opencv.hpp>

typedef unsigned char uchar;

/**
 * @brief Load from a raw image into a 1D unsigned char image array
 * @details use 1D array to store a 2D image, store it row by row
 * @param fileName file name of the raw image
 * @param image 1D unsigned char image array
 * @return true if success
 */
bool loadRawImage(const char *fileName, uchar *image, int rows, int cols);

/**
 * @brief Save a 1D unsigned char image array
 * @details use 1D array to store a 2D image, store it row by row
 * @param fileName output file path
 * @param image 1D unsigned char image array
 * @return true if success
 */
bool saveRawImage(const char *fileName, uchar *image, int rows, int cols);

/**
 * convert an array in the memory to cv::Mat object
 * @param image image array
 * @param rows image rows
 * @param cols image columns
 * @return openCV Mat
 */
cv::Mat array2cvMat(const uchar *image, int rows, int cols);

#endif //CPP_CODE_IMAGEUTILS_H
