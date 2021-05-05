#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "imageUtils.h"
#include "matplotlibcpp.h"
#include <complex>
#include <cmath>

#define ROWS 480
#define COLS 640
#define L 256


int main()
{
	auto *image = (uchar *) malloc(sizeof(uchar) * COLS * ROWS);

	// 把raw图片加载成了array
	loadRawImage("../images/cat.raw", image, ROWS, COLS);
	// 转换成opencv的格式
	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);
	// cv::imshow("source image", imgSrc);

	cv::Mat imgDst(imgSrc.size(), CV_32SC1);

	// cv::Mat kernel = (cv::Mat_<int>(3, 3) << 1, 1, 1, 1, -8, 1, 1, 1, 1);
	// cv::filter2D(imgSrc, imgDst, imgSrc.depth(), kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);

	for (int row = 0; row < imgSrc.rows; ++row)
	{
		for (int col = 0; col < imgSrc.cols; ++col)
		{
			// imgDst.at<uchar>(row, col) = 9 * imgSrc.at<uchar>(row, col) - imgSrc.at<uchar>(row -1, col-1) - imgSrc.at<uchar>(row, col-1)
		}
	}

	// find min number of imgDst

	//	double minVal, maxVal;
	//	cv::minMaxLoc(imgDst, &minVal, &maxVal);
	//
	//	imgDst -= minVal;
	//
	//	imgDst *= 255.0 / maxVal;


	//	cv::imshow("output image", imgDst);
	//	cv::waitKey(0);

	cv::Mat test(3, 3, CV_8UC1);
	for (int i = 0; i < 3; ++i)
	{
		for (int j = 0; j < 3; ++j)
		{
			test.at<uchar>(i, j) = i + j;
		}
	}

//	std::cout << "test: " << test << std::endl;
//
//	test += 100;
//	std::cout << "test: " << test << std::endl;

std::complex<int> complex1 = 1 + std::literals::1i;

	// cv::saturate_cast<uchar>()

	// DFT
	// Real Img

	cv::waitKey(0);

	return 0;
}
