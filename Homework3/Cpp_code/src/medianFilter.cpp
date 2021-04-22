/** @file       medianFilter.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       04/20/2021
 *  @brief      implement median filter
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

#include <vector>
#include <algorithm>

#include "imageUtils.h"
#include "matplotlibcpp.h"

#define ROWS 480
#define COLS 640
#define L 256 // grayscale

cv::Mat medianFilter(const cv::Mat &imgSrc, int kernelSize, const std::string &paddingMethod);
std::vector<int> getHistogram(cv::Mat &image, int grayScale);

int main(int argc, char **argv)
{
	// load image and convert it to cv::Mat format
	const char *imagePath = 2 == argc ? argv[1] : "../images/triangle.raw";
	auto *image = static_cast<uchar *>(malloc(sizeof(uchar) * ROWS * COLS));
	if (loadRawImage(imagePath, image, ROWS, COLS)) { std::cout << "Load image successfully!\n"; }
	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);
	cv::imshow("original image", imgSrc);

	cv::Mat imgFiltered = medianFilter(imgSrc, 11, "replicate");
	cv::imshow("output image", imgFiltered);
	cv::waitKey(0);
	cv::imwrite("..images_out/image_median.png", imgFiltered);

	// get the histogram of output image
	std::vector<int> vHistFiltered = getHistogram(imgFiltered, L);
	matplotlibcpp::figure();
	matplotlibcpp::bar(vHistFiltered);
	matplotlibcpp::title("output image histogram");
	matplotlibcpp::savefig("../images_out/filtered_image_hist.png");
	matplotlibcpp::show();

	return 0;
}

/**
 * @brief Apply a median filter to the image
 * @param imgSrc cv::Mat: input image
 * @param kernelSize int: kernel size
 * @param paddingMethod const std::string &: padding method
 * @return cv::Mat: image processed by the filter
 */
cv::Mat medianFilter(const cv::Mat &imgSrc, int kernelSize, const std::string &paddingMethod)
{
	if (kernelSize % 2 != 1) // kernel size must be an odd number
	{
		std::cerr << "Error: kernel size must be an odd number!\n";
		exit(EXIT_FAILURE);
	}

	if ("replicate" == paddingMethod)
	{
		int paddingSize = kernelSize / 2; // use this number to pad original image

		// pad image
		cv::Mat imgPad;
		cv::copyMakeBorder(imgSrc, imgPad, paddingSize, paddingSize, paddingSize, paddingSize, cv::BORDER_REPLICATE);

		cv::Mat imgDst(imgSrc.size(), imgSrc.type());

		for (int i = 0; i < imgDst.rows; ++i)
		{
			for (int j = 0; j < imgDst.cols; ++j)
			{
				// apply median filter within the kernel range
				std::vector<int> vnPixelVal;    // vector storing the pixel value with the kernel range
				for (int p = -paddingSize; p <= paddingSize; ++p)
				{
					for (int q = -paddingSize; q <= paddingSize; ++q)
					{
						vnPixelVal.push_back(imgPad.at<uchar>(i + paddingSize + p, j + paddingSize + q));
					}
				}
				std::sort(vnPixelVal.begin(), vnPixelVal.end());
				imgDst.at<uchar>(i, j) = static_cast<uchar>(vnPixelVal.at(kernelSize * kernelSize / 2));
			}
		}

		return imgDst;
	}
}

/**
 * Calculate the histogram from a cv::Mat image
 * @param image cv::Mat: image
 * @param grayScale int: gray scale
 * @return std::vector<int>: histogram vector
 */
std::vector<int> getHistogram(cv::Mat &image, int grayScale)
{
	std::vector<int> vHist(grayScale); // initialize each element as 0 by default
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j) { ++(vHist.at(image.at<uchar>(i, j))); }
	}

	return vHist;
}
