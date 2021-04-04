// Created by jackzhang on 4/4/21.

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>

#define ROWS 480
#define COLS 640

bool loadRawImage(const char *fileName, unsigned char image[][COLS]);
bool saveRawImage(const char *fileName, unsigned char image[][COLS]);

void subSample(int ratio, unsigned char **srcImg)

int main(int argc, char *argv[])
{
	unsigned char image[ROWS][COLS];

	if (argc != 2)
	{
		fprintf(stderr, "usage: %s image_name\n", argv[0]);
		exit(1);
	}
	if (loadRawImage(argv[1], image)) { printf("Load image successfully!\n"); }

	if (saveRawImage("../cat_copy.raw", image)) { printf("Save image successfully!\n"); }

	return 0;
}

/**
 * Load from a raw image into a 2D unsigned char image array
 * @param fileName file name of the raw image
 * @param image 2D unsigned char image array
 * @return true if success
 */
bool loadRawImage(const char *fileName, unsigned char image[][COLS])
{
	FILE *filePtr = fopen(fileName, "rb");
	if (NULL == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}

	for (int i = 0; i < ROWS; ++i)
	{
		if (COLS != fread(image[i], 1, COLS, filePtr))
		{
			fprintf(stderr, "Error: cannot load enough stuff\n");
			return false;
		}
	}
	fclose(filePtr);

	return true;
}

/**
 * Save a 2D unsigned char image array
 * @param fileName output file path
 * @param image 2D unsigned char image array
 * @return true if success
 */
bool saveRawImage(const char *fileName, unsigned char image[][COLS])
{
	FILE *filePtr = fopen(fileName, "wb");
	if (NULL == filePtr)
	{
		fprintf(stderr, "Error: cannot open %s\n", fileName);
		return false;
	}

	for (int i = 0; i < ROWS; ++i)
	{
		fwrite(image[i], 1, COLS, filePtr);
	}
	fclose(filePtr);

	return true;
}


