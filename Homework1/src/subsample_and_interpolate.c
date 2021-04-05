// Created by jackzhang on 4/4/21.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <math.h>

#define ROWS 480
#define COLS 640

typedef unsigned char uchar;

bool loadRawImage(const char *fileName, uchar *image, int rows, int cols);
bool saveRawImage(const char *fileName, uchar *image, int rows, int cols);
uchar *subSample(int ratio, uchar *srcImg, int rows, int cols);
uchar *upSampleNearest(int ratio, uchar *srcImg, int rows, int cols);

int main(int argc, char *argv[])
{
	// uchar image[ROWS][COLS];
	uchar *image = (uchar *) malloc(sizeof(uchar) * ROWS * COLS);

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s image_name\n", argv[0]);
		exit(1);
	}
	if (loadRawImage(argv[1], image, ROWS, COLS)) { printf("Load image successfully!\n"); }
	if (saveRawImage("../cat_copy.raw", image, ROWS, COLS)) { printf("Save image successfully!\n"); }


	uchar *subImg = subSample(4, image, ROWS, COLS);
	if (saveRawImage("../subImg.raw", subImg, ROWS / 4, COLS / 4)) { printf("Save subimage successfully!\n"); }

	uchar *upImg = upSampleNearest(4, subImg, ROWS / 4, COLS / 4);
	if (saveRawImage("../upImg.raw", upImg, ROWS, COLS)) { printf("Save upimage successfully!\n"); }

	free(subImg);
	free(upImg);
	return 0;
}

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
	if (NULL == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}

	for (int i = 0; i < rows; ++i)
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
	if (NULL == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}
	for (int i = 0; i < rows; ++i)
	{
		fwrite(image + i * cols, 1, cols, filePtr);
	}
	fclose(filePtr);

	return true;
}

/**
 * Subsample the image
 * @param ratio subsample ratio
 * @param srcImg input image
 * @param rows the number of original image rows
 * @param cols the number of original image columns
 * @return the subsampled image
 */
uchar *subSample(int ratio, uchar *srcImg, int rows, int cols)
{
	if (ratio <= 0)
	{
		fprintf(stderr, "Error: wrong subsample ratio.\n");
		exit(1);
	}

	int rowsSub = rows / ratio;
	int colsSub = cols / ratio;

	uchar *subImg = (uchar *) malloc(sizeof(uchar) * rowsSub * colsSub);
	for (int i = 0; i < rowsSub; ++i)
	{
		for (int j = 0; j < colsSub; ++j)
		{
			*(subImg + colsSub * i + j) = *(srcImg + colsSub * ratio * i * ratio + ratio * j);
			//			memcpy(subImg + colsSub * i + j, srcImg + colsSub * ratio * i * ratio + j * ratio, 1);
		}
	}

	return subImg;
}

/**
 * @brief Upscale the image using nearest interpolation method
 * @param ratio upscale ratio
 * @param srcImg source image
 * @param rows source image rows
 * @param cols source image columns
 * @return upscaled image
 */
uchar *upSampleNearest(int ratio, uchar *srcImg, int rows, int cols)
{
	if (ratio <= 0)
	{
		fprintf(stderr, "Error: wrong ratio!\n");
		exit(1);
	}

	int rowsUp = rows * ratio; // rows after upscaling
	int colsUp = cols * ratio; // cols after upscaling

	uchar *upImg = (uchar *) malloc(sizeof(uchar) * rowsUp * colsUp);

	for (int i = 0; i < rowsUp; ++i)
	{
		/// corresponding i on source image, (int)(number + 0.5) => round(number)
		int iSrc = (int) ((double) i / ratio + 0.5);
		for (int j = 0; j < colsUp; ++j)
		{
			/// corresponding j on source image, (int)(number + 0.5) => round(number)
			int jSrc = (int) ((double) j / ratio + 0.5);

			*(upImg + i * colsUp + j) = *(srcImg + iSrc * cols + jSrc);
		}
	}

	return upImg;
}
