/** @file       laplacian_OpenCV_built_in.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/04/2021
 *  @brief      use OpenCV's built-in function to do laplacian transformation
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		std::cerr << "usage: " << argv[0] << " image_path" << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::Mat imageSrc = cv::imread(argv[1], cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image " << argv[1] << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::imshow("original image", imageSrc);

	// use OpenCV's filter2d to do the filtering(convolution) process
	cv::Mat kernel = (cv::Mat_<int>(3, 3) << 0, 1, 0, 1, -4, 1, 0, 1, 0);
	cv::Mat imageLaplacian;
	cv::filter2D(imageSrc, imageLaplacian, imageSrc.depth(), kernel, cv::Point(-1, -1), 0, cv::BORDER_DEFAULT);
	cv::imshow("laplacian", imageLaplacian);

	cv::Mat kernel2 = (cv::Mat_<int>(3, 3) << 0, -1, 0, -1, 5, -1, 0, -1, 0);
	cv::Mat imageSharpened;
	cv::filter2D(imageSrc, imageSharpened, -1, kernel2);
	cv::imshow("sharpened image", imageSharpened);

	// use OpenCV's cv::Laplacian to do the process
	cv::Mat imageLaplacian2;
	cv::Laplacian(imageSrc, imageLaplacian2, CV_16S, 3, 1,0,  cv::BORDER_DEFAULT);
	// convert CV_16S back to CV_8U
	cv::convertScaleAbs(imageLaplacian2, imageLaplacian2);
	cv::imshow("laplacian2", imageLaplacian2);

	cv::waitKey(0);
	return 0;
}