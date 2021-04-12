/** @file       main.cpp.c
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/4/5/21/2021
 *  @brief      Subsample image and upsample image using nearest interpolation
 *  @details    Read and write .raw format image file. Subsample image. Upsample image using nearest interpolation.
 *  			Convert to jpg image and save.
 *  @note       
 */

#include <iostream>
#include <opencv2/opencv.hpp>

#include <cstdio>
#include <cstdlib>

#define ROWS 480
#define COLS 640

// typedef unsigned char uchar;

bool loadRawImage(const char *fileName, uchar *image, int rows, int cols);
bool saveRawImage(const char *fileName, uchar *image, int rows, int cols);
uchar *subSample(int ratio, const uchar *srcImg, int rows, int cols);
uchar *upSampleNearest(int ratio, const uchar *srcImg, int rows, int cols);
cv::Mat array2cvMat(const uchar *image, int rows, int cols);


int main(int argc, char **argv)
{
	// uchar image[ROWS][COLS];
	auto *image = (uchar *) malloc(sizeof(uchar) * ROWS * COLS); // allocate memory for image

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s image_name\n", argv[0]);
		exit(1);
	}
	if (loadRawImage(argv[1], image, ROWS, COLS)) { std::cout << "Load image successfully!\n"; }

	int ratio = 16;    // zoom and shrink ratio

	uchar *subImg = subSample(ratio, image, ROWS, COLS); // subsample original with the ratio
	uchar *upImg = upSampleNearest(ratio, subImg, ROWS / ratio, COLS / ratio); // upsample the subsampled image

	cv::Mat subImgMat = array2cvMat(subImg, ROWS / ratio, COLS / ratio);
	cv::Mat upImgMat = array2cvMat(upImg, ROWS, COLS);

	std::string subImgName("../subImg_ratio_");
	std::stringstream ss;
	ss << ratio;
	subImgName.append(ss.str());
	if (cv::imwrite(subImgName + ".jpg", subImgMat)) { std::cout << "write subsampled image successfully!\n"; }

	std::string upImgName("../upImg_ratio_");
	upImgName.append(ss.str());
	if (cv::imwrite(upImgName + ".jpg", upImgMat)) { std::cout << "write upsampled image successfully!\n"; }

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
 * Subsample the image
 * @param ratio subsample ratio
 * @param srcImg input image
 * @param rows the number of original image rows
 * @param cols the number of original image columns
 * @return the subsampled image
 */
uchar *subSample(int ratio, const uchar *srcImg, int rows, int cols)
{
	if (ratio <= 0)
	{
		fprintf(stderr, "Error: wrong subsample ratio.\n");
		exit(1);
	}

	int rowsSub = rows / ratio;
	int colsSub = cols / ratio;

	auto *subImg = (uchar *) malloc(sizeof(uchar) * rowsSub * colsSub);
	for (int i = 0; i < rowsSub; ++i)
	{
		int iSrc = i * ratio; // corresponding position on the source image
		for (int j = 0; j < colsSub; ++j)
		{
			int jSrc = j * ratio; // corresponding position on the source image
			*(subImg + colsSub * i + j) = *(srcImg + cols * iSrc + jSrc);
			// memcpy(subImg + colsSub * i + j, srcImg + colsSub * ratio * i * ratio + j * ratio, 1);
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
uchar *upSampleNearest(int ratio, const uchar *srcImg, int rows, int cols)
{
	if (ratio <= 0)
	{
		fprintf(stderr, "Error: wrong ratio!\n");
		exit(1);
	}

	int rowsUp = rows * ratio; // rows after upscaling
	int colsUp = cols * ratio; // cols after upscaling

	auto *upImg = (uchar *) malloc(sizeof(uchar) * rowsUp * colsUp);

	for (int i = 0; i < rowsUp; ++i)
	{
		/// corresponding i on source image, (int)(number + 0.5) => round(number)
		// int iSrc = (int) ((double) i / ratio + 0.5);
		int iSrc = cvRound(static_cast<double>(i) / ratio);
		// when i==rowsUp - 1; iSrc == cvRound(rows*ratio-1)/ratio == rows, which is beyond boundary
		if (iSrc >= rows - 1) { iSrc = rows - 1; }

		for (int j = 0; j < colsUp; ++j)
		{
			/// corresponding j on source image, (int)(number + 0.5) => round(number)
			// int jSrc = (int) ((double) j / ratio + 0.5);
			int jSrc = cvRound(static_cast<double>(j) / ratio);
			// when j==colsUp - 1; jSrc == cvRound(cols*ratio-1)/ratio == cols, which is beyond boundary
			if (jSrc >= cols - 1) { jSrc = cols - 1; }
			*(upImg + i * colsUp + j) = *(srcImg + iSrc * cols + jSrc);
		}
	}

	return upImg;
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
