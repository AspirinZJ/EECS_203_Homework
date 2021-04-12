// Created by jackzhang on 4/5/21.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 480
#define COLS 640

typedef unsigned char uchar;


bool loadRawImage(const char *fileName, uchar *image, int rows, int cols);
bool saveRawImage(const char *outName, uchar *image, int rows, int cols);
// 1. subsample 4x 16x
uchar *subSample(int ratio, const char *srcImg, int rows, int cols);

// 2. upsample using nearest interpolation
uchar *upSample(int ratio, const char *srcImg, int rows, int cols);

int main(int argc, char *argv[])
{
	const char *imageName = "../images/cat.raw";
	// 用char是因为一个char占一个byte，8bit, 用unsigned是因为像素值大于0
	uchar *image = (uchar *) malloc(sizeof(uchar) * ROWS * COLS);

	bool ret = loadRawImage(imageName, image, ROWS, COLS);
	if (ret) { printf("load image successfully!\n"); }

	uchar *subImg = subSample(16, image, ROWS, COLS);

	uchar *upImg = upSample(16, subImg, ROWS/16, COLS/16);

	saveRawImage("../test.raw", upImg, ROWS, COLS);

	return 0;
}


/**
 * @brief load image from a raw image file
 * @param fileName image name
 * @param image image array
 * @param rows image rows
 * @param cols image cols
 * @return
 */
bool loadRawImage(const char *fileName, uchar *image, int rows, int cols)
{
	// 打开文件流，操作本地文件
	FILE *filePtr = fopen(fileName, "rb"); // r: read, b: binary
	if (NULL == filePtr)
	{
		fprintf(stderr, "Error: cannot load image file.\n");
		exit(1);
	}

	for (int i = 0; i < rows; ++i)
	{
		// 一次读取一行， row-by-row
		if (cols != fread(image + i * cols, 1, cols, filePtr))
		{
			fprintf(stderr, "Error: read error!\n");
			exit(1);
		}
	}

	// 不要忘记关闭文件流
	fclose(filePtr);

	return true;
}

uchar *subSample(int ratio, const char *srcImg, int rows, int cols)
{
	if (ratio <= 0) { exit(1); }

	int rowsSub = rows / ratio;
	int colsSub = cols / ratio;

	// 创建新图片
	uchar *subImg = (uchar *) malloc(sizeof(uchar) * rowsSub * colsSub);

	// subsample 遍历没一个subimage的像素点
	// **************
	for (int i = 0; i < rowsSub; ++i)
	{
		for (int j = 0; j < colsSub; ++j)
		{
			// !! 重要，核心代码
			int iSrc = i * ratio;
			int jSrc = j * ratio;
			*(subImg + i * colsSub + j) = *(srcImg + cols * iSrc + jSrc);
		}
	}

	return subImg;
}

uchar *upSample(int ratio, const char *srcImg, int rows, int cols)
{
	int rowsUp = rows * ratio;
	int colsUp = cols * ratio;

	uchar *upImg = (uchar *) malloc(sizeof(uchar) * rowsUp * colsUp);

	for (int i = 0; i < rowsUp; ++i)
	{
		for (int j = 0; j < colsUp; ++j)
		{
			// !! core code
			int iSrc = (int) (i / ratio + 0.5);
			int jSrc = (int) (j / ratio + 0.5);
			*(upImg + i * colsUp + j) = *(srcImg + iSrc * cols + jSrc);
		}
	}
	return upImg;
}

bool saveRawImage(const char *outName, uchar *image, int rows, int cols)
{
	FILE *filePtr = fopen(outName, "wb");
	if (NULL == filePtr) { exit(1); }

	// 按行写出
	for (int i = 0; i < rows; ++i)
	{
		fwrite(image + i*cols, 1, cols, filePtr);
	}


	fclose(filePtr);
	return true;
}
