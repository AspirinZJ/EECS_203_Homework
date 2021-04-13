/** @file       OpenCVHistogramEqualization.cpp.c
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/12/2021
 *  @brief      Histogram equalization with OpenCV
 *  @details    
 *  @note       
 */

#include <iostream>
#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

#include "imageUtils.h"

#define ROWS 480
#define COLS 640

int main()
{
	auto *image = new uchar[sizeof(uchar) * ROWS * COLS];
	if (loadRawImage("../images/cat.raw", image, ROWS, COLS));

	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);

	cv::Mat imgDst;
	cv::equalizeHist(imgSrc, imgDst);

	cv::imshow("histogram-equalized image", imgDst);
	cv::waitKey(0);

	delete[] image;
	return 0;
}
