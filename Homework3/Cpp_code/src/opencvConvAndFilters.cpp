/** @file       opencvLinearFilter.cpp.c
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       04/20/2021
 *  @brief      Utilize OpenCV linear filters
 *  @details    
 *  @note       
 */

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
	const char *imagePath = argc > 2 ? argv[1] : "../images/cat.png";
	cv::Mat imgSrc = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
	if (imgSrc.empty())
	{
		std::cerr << "Error: cannot load image. " << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::imshow("original image", imgSrc);

	// we create an average/mean filter all coefficients of this filter are the same
	// this filter is also normalized
	cv::Mat imgDst(imgSrc.size(), imgSrc.type());
	cv::blur(imgSrc, imgDst, cv::Size(5, 5), cv::Point(-1, -1), cv::BORDER_REPLICATE);
	cv::imshow("image using blur function", imgDst);


	// Gaussian blur
	cv::Mat imgGaussian(imgSrc.size(), imgSrc.type());
	cv::GaussianBlur(imgSrc, imgGaussian, cv::Size(5, 5), 1, 0, cv::BORDER_DEFAULT);
	cv::imshow("image blurred by Gaussian blur", imgGaussian);

	// Directly use filter2D in OpenCV to do the convolution calculation
	// kernel must be normalized and single channel, floating point matrix
	cv::Mat imgCov(imgSrc.size(), imgSrc.type());
	cv::Mat kernel = cv::Mat::ones(5, 5, CV_32F) / static_cast<float>(5 * 5);
	// cv::filter2D(srcMat, dstMat, ddepth, kernel, anchor, delta, borderType);
	// ddepth is bit of depth like 8-bit, 16-bit, 32-bit, etc. If it is negative, the depth is the same as the source image
	// delta is a value added to all the pixels after convolution
	// anchor is the relative position of the anchor in the kernel matrix
	cv::filter2D(imgSrc, imgCov, -1, kernel, cv::Point(-1, -1), 0.0, cv::BORDER_DEFAULT);
	cv::imshow("image blurred using openCV filter2D function", imgCov);

	cv::waitKey(0);

	return 0;
}
