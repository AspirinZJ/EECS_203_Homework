/** @file       speed_test_OpenCV.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/01/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void method1(const cv::Mat &imageSrc, cv::Mat &imageDst)
{
	for (int row = 0; row < imageDst.rows; ++row)
	{
		for (int col = 0; col < imageDst.cols; ++col)
		{
			imageDst.at<uchar>(row, col) = static_cast<uchar>(imageSrc.at<uchar>(row, col) * 0.5);
		}
	}
}

void method2(const cv::Mat &imageSrc, cv::Mat &imageDst)
{
	for (int row = 0; row < imageDst.rows; ++row)
	{
		const auto *pSrc = imageSrc.ptr(row);
		auto *pDst = imageDst.ptr(row);
		for (int col = 0; col < imageDst.cols; ++col)
		{
			*(pDst + col) = static_cast<uchar>(*(pSrc + col) * 0.5);
		}
	}
}

int main(int argc, char **argv)
{
	cv::String keys = "{@input | ../images/cat.png | input image}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"));
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}
	cv::imshow("original image", imageSrc);

	cv::Mat imageDst1 = imageSrc.clone();
	cv::Mat imageDst2 = imageSrc.clone();

	method1(imageSrc, imageDst1);
	method2(imageSrc, imageDst2);
	// cv::imshow("dst1", imageDst1);
	// cv::imshow("dst2", imageDst2);


	cv::waitKey(0);

	return 0;
}
