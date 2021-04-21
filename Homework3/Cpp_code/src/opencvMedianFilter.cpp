/** @file       opencvMedianFilter.cpp.c
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       04/20/2021
 *  @brief      utilize OpenCV median filter
 *  @details    
 *  @note       
 */

#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
	const char *imgPath = argc == 2 ? argv[1] : "../images/cat.png";
	cv::Mat imgSrc = cv::imread(imgPath, cv::IMREAD_UNCHANGED);
	if (imgSrc.empty())
	{
		std::cerr << "Error: cannot load image!\n";
		exit(EXIT_FAILURE);
	}
	cv::namedWindow("original image", cv::WINDOW_AUTOSIZE);
	cv::imshow("original image", imgSrc);

	cv::Mat imgDst(imgSrc.size(), imgSrc.type());

	// apply median filter
	cv::medianBlur(imgSrc, imgDst, 5);
	cv::namedWindow("median filtered image", cv::WINDOW_AUTOSIZE);
	cv::imshow("median filtered image", imgDst);

	cv::waitKey(0);

	return 0;
}
