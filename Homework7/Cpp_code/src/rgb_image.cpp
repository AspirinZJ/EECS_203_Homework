/** @file       rgb_image.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/24/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main(int argc, char **argv)
{
	cv::String keys = "{@input | ../images/triangle.png | input image path}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image: " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}

	cv::Mat rChannel, gChannel, bChannel;
	imageSrc.copyTo(rChannel);
	imageSrc.copyTo(gChannel);
	imageSrc.copyTo(bChannel);

	gChannel


	return 0;
}
