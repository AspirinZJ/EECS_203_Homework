/** @file       DFT_OpenCV.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/05/2021
 *  @brief      Discrete Fourier Transform using OpenCV
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		std::cerr << "usage: " << argv[0] << " image_path" << std::endl;
		exit(EXIT_FAILURE);
	}
	cv::String keys = "{@input | ../images/cat.png | input image path}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image: " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}

	// pad image to multiplication of 2 before discrete fourier transform
	const int rowPad = cv::getOptimalDFTSize(imageSrc.rows); // get the optimal paading size
	const int colPad = cv::getOptimalDFTSize(imageSrc.cols);

	cv::Mat imagePad;
	cv::copyMakeBorder(imageSrc, imagePad, 0, rowPad - imageSrc.rows, 0, colPad - imageSrc.cols,
					   cv::BORDER_CONSTANT, cv::Scalar::all(0));

	// create a plane(cv::Mat) containing two cv::Mat(real and imaginary parts)
	cv::Mat planes[2] = {cv::Mat_<float>(imagePad), cv::Mat::zeros(imagePad.size(), CV_32F)};

	cv::Mat complexImage;
	cv::merge(planes, 2, complexImage); // merge two part into a complexImage

	// DFT
	cv::dft(complexImage, complexImage);

	// split two parts
	cv::split(complexImage, planes); // split the complex output into two images of planes

	// calculate the magnitude
	cv::Mat magMat;
	cv::magnitude(planes[0], planes[1], magMat);

	// convert the magnitude to log scale for better view: => log(1 + sqrt(real^2 + img^2))
	magMat += cv::Scalar::all(1.0);
	cv::log(magMat, magMat);

	// crop the image if it has odd number of rows or columns
	// -2 => 11111110, num & -2 => set the last digit of the number to 0, make it even number
	magMat = magMat(cv::Rect(0, 0, magMat.cols & (-2), magMat.rows & (-2)));

	// rearrange the quadrants of the Fourier image so that the origin is at the center of the image
	int cx = magMat.cols / 2, cy = magMat.rows / 2;
	cv::Mat topLeft = magMat(cv::Rect(0, 0, cx, cy));
	cv::Mat topRight = magMat(cv::Rect(cx, 0, cx, cy));
	cv::Mat botLeft = magMat(cv::Rect(0, cy, cx, cy));
	cv::Mat botRight = magMat(cv::Rect(cx, cy, cx, cy));
	// swap the top left with bottom right, the top right with bottom left
	cv::Mat tmpMat;
	topLeft.copyTo(tmpMat);
	botRight.copyTo(topLeft);
	tmpMat.copyTo(botRight);

	topRight.copyTo(tmpMat);
	botLeft.copyTo(topRight);
	tmpMat.copyTo(botLeft);

	// normalize to 0~255
	cv::normalize(magMat, magMat, 0, 255, cv::NORM_MINMAX);
	magMat.convertTo(magMat, CV_8U);

	cv::imshow("magnitude", magMat);
	cv::waitKey(0);
	cv::imwrite("../images_output/magnitude.png", magMat);
	return 0;
}