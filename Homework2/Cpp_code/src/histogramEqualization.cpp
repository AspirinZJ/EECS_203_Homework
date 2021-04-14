/** @file       histogramEqualization.cpp.c
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/13/2021
 *  @brief      Perform histogram equalization
 *  @details    
 *  @note       
 */

#include <iostream>
#include <opencv2/opencv.hpp>

#include "imageUtils.h"
#include "matplotlibcpp.h"

#define ROWS 480
#define COLS 640
#define L 256 // grayscale

int *getHistogram(cv::Mat &image, int grayScale);
int *getCumuDist(const int *hist, int grayscale);
void plot(int *arr, int size, const char *title);
void histEqu1(cv::Mat &imgSrc);
void histEqu2(cv::Mat &imgSrc);

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		std::cerr << "Error: usage: " << argv[0] << " image_name\n";
		return 1;
	}

	auto *image = static_cast<uchar *>(malloc(sizeof(uchar) * ROWS * COLS)); // allocate memoryh for image
	if (loadRawImage(argv[1], image, ROWS, COLS)) { std::cout << "Load image successfully!" << std::endl; }
	// convert uchar * array to cv::Mat image
	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);
	cv::imshow("source image", imgSrc);
	cv::waitKey(0);

	// Method1
	histEqu1(std::ref(imgSrc));

	// Method2
	histEqu2(std::ref(imgSrc));

	free(image);
	return 0;
}

/**
 * @brief histogram equalization
 * @details Method1: match source cumulative distribution to desired cumulative distribution
 * @param imgSrc cv::Mat: input image
 */
void histEqu1(cv::Mat &imgSrc)
{
	// get histogram of source image
	int *histSrc = getHistogram(imgSrc, L);
	plot(histSrc, L, "source image histogram");
	// get cumulative distribution of source image
	int *cumuDistSrc = getCumuDist(histSrc, L);
	plot(cumuDistSrc, L, "source image cumulative distribution");
	int cumuDistDesired[L] = {0};
	double sumDesired = 0;
	double binNum = static_cast<double>(ROWS) * COLS / L; // the number of pixel each gray level has of desired output
	for (int &element : cumuDistDesired)
	{
		sumDesired += binNum;
		element = static_cast<int>(round(sumDesired));
	}

	// array which maps source image gray level to desired image gray level
	uchar src2des[L]{};
	for (int i = 0; i < L; ++i)
	{
		/// which element of desired cumulative distribution is closest to source cumulative distribution
		int min = std::abs(cumuDistDesired[0] - cumuDistSrc[i]);
		int minInd = 0;
		for (int j = 1; j < L; ++j)
		{
			// find the gray level on desired cumulative distribution which is closet to the current cumulative distribution
			if (std::abs(cumuDistDesired[j] - cumuDistSrc[i]) < min)
			{
				minInd = j;
				min = std::abs(cumuDistDesired[j] - cumuDistSrc[i]);
			}
		}
		src2des[i] = minInd; // give the index of the closet gray level to the corresponding array element
	}

	// get the histogram-equalized image
	// ! don't use cv::Mat imgHistEqu(imgSrc); changing imgHistEqu will change imgSrc
	// cv::Mat imgHistEqu(imgSrc.rows, imgSrc.cols, imgSrc.type());
	cv::Mat imgHistEqu = imgSrc.clone();
	for (int i = 0; i < imgHistEqu.rows; ++i)
	{
		for (int j = 0; j < imgHistEqu.cols; ++j) { imgHistEqu.at<uchar>(i, j) = src2des[imgSrc.at<uchar>(i, j)]; }
	}
	cv::imshow("histogram equalized image using method1", imgHistEqu);
	cv::waitKey(0);

	// get the histogram of the output image
	int *histDst = getHistogram(imgHistEqu, L);
	plot(histDst, L, "histogram of the output image using method1");
	// get the cumulative distribution of the output image
	int *cumuDistDst = getCumuDist(histDst, L);
	plot(cumuDistDst, L, "cumulative distribution of the output image using method1");

	delete[] histSrc;
	delete[] histDst;
	delete[] cumuDistSrc;
	delete[] cumuDistDst;
}

/**
 * @brief histogram equalization
 * @details Method2: use the logic similar to OpenCV's equalizeHist function
 * @param imgSrc cv::Mat: input image
 */
void histEqu2(cv::Mat &imgSrc)
{
	// get histogram of source image
	int *histSrc = getHistogram(imgSrc, L);
	plot(histSrc, L, "source image histogram");
	// get cumulative distribution of source image
	int *cumuDistSrc = getCumuDist(histSrc, L);
	plot(cumuDistSrc, L, "source image cumulative distribution");

	// 1. map the original cumulative distribution to [0, maxGrayLevel]
	double scale = static_cast<double>(L - 1) / *(cumuDistSrc + L - 1);
	int src2des2[L]{};
	for (int i = 0; i < L; ++i)
	{
		src2des2[i] = static_cast<int>(round(scale * *(cumuDistSrc + i)));
	}
	plot(src2des2, L, "src2des2");

	// 2. Use the rescaled cumulative distribution as the mapping from source image grayLevel to target image grayLevel
	cv::Mat imgHistEqu2 = imgSrc.clone();
	for (int i = 0; i < ROWS; ++i)
	{
		for (int j = 0; j < COLS; ++j) { imgHistEqu2.at<uchar>(i, j) = src2des2[imgSrc.at<uchar>(i, j)]; }
	}
	cv::imshow("histogram equalized image using OpenCV equalizeHist method", imgHistEqu2);
	cv::waitKey(0);

	int *histDst = getHistogram(imgHistEqu2, L);
	plot(histDst, L, "histogram distribution of output image using method2");
	int *cumuDistDst = getCumuDist(histDst, L);
	plot(cumuDistDst, L, "cumulative distribution of output image using method2");

	delete[] histSrc;
	delete[] histDst;
	delete[] cumuDistSrc;
	delete[] cumuDistDst;
}


/**
 * Calculate the histogram from a cv::Mat image
 * @param image cv::Mat: image
 * @param grayScale int: gray scale
 * @return int *: array of histogram on heap
 */
int *getHistogram(cv::Mat &image, int grayScale)
{
	// int histSrc[grayScale] = {0};
	int *histSrc = new int[grayScale](); // 每个元素初始化为0
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			// histSrc[image.at<uchar>(i, j)]++;
			(*(histSrc + image.at<uchar>(i, j)))++;
		}
	}

	return histSrc;
}

/**
 * get accumulative distribution from histogram
 * @param hist int *: input histogram
 * @param grayscale int: gray scale
 * @return int *: accumulative distribution
 */
int *getCumuDist(const int *hist, int grayscale)
{
	int *cumuDist = new int[grayscale](); // 初始化所有元素为0
	int sum = 0;
	for (int i = 0; i < grayscale; ++i)
	{
		sum += *(hist + i);
		*(cumuDist + i) = sum;
	}
	return cumuDist;
}

/**
 * plot the give array
 * @param arr int *: input array
 * @param size int: array size
 * @param title const char *: plot title
 */
void plot(int *arr, int size, const char *title)
{
	// convert the input array to vector
	std::vector<int> vec(arr, arr + size);

	matplotlibcpp::figure();
	matplotlibcpp::bar(vec);
	matplotlibcpp::title(title);
	matplotlibcpp::grid(true);
	matplotlibcpp::show();
}
