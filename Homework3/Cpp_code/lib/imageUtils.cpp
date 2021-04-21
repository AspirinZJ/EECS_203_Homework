#include <cstdio>
#include "imageUtils.h"

/**
 * @brief Load from a raw image into a 1D unsigned char image array
 * @details use 1D array to store a 2D image, store it row by row
 * @param fileName file name of the raw image
 * @param image 1D unsigned char image array
 * @return true if success
 */
bool loadRawImage(const char *fileName, uchar *image, int rows, int cols)
{
	FILE *filePtr = fopen(fileName, "rb");
	if (nullptr == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}

	for (int i = 0; i < rows; ++i) // read data row-by-row
	{
		if (cols != fread(image + cols * i, 1, cols, filePtr))
		{
			fprintf(stderr, "Error: cannot load enough stuff\n");
			return false;
		}
	}
	fclose(filePtr);
	return true;
}

/**
 * @brief Save a 1D unsigned char image array
 * @details use 1D array to store a 2D image, store it row by row
 * @param fileName output file path
 * @param image 1D unsigned char image array
 * @return true if success
 */
bool saveRawImage(const char *fileName, uchar *image, int rows, int cols)
{
	FILE *filePtr = fopen(fileName, "wb");
	if (nullptr == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}

	for (int i = 0; i < rows; ++i) // save image row-by-row
	{
		fwrite(image + i * cols, 1, cols, filePtr);
	}
	fclose(filePtr);
	return true;
}

/**
 * convert an array in the memory to cv::Mat object
 * @param image image array
 * @param rows image rows
 * @param cols image columns
 * @return openCV Mat
 */
cv::Mat array2cvMat(const uchar *image, int rows, int cols)
{
	cv::Mat imgMat(rows, cols, CV_8UC1);    // 8bit unsigned char, 1 channel(gray)

	for (int i = 0; i < rows; ++i)
	{
		auto *pixelPtr = imgMat.ptr<uchar>(i); // pointer pointing to the first pixel in a row
		for (int j = 0; j < cols; ++j)
		{
			*(pixelPtr + j) = *(image + i * cols + j);
		}
	}

	return imgMat;
}
