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

cv::Mat calGaussianKernel(int rows, int cols, double sigma);

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

	cv::Mat rChannel(imageSrc.size(), CV_32FC1), gChannel(imageSrc.size(), CV_32FC1), bChannel(imageSrc.size(),
			CV_32FC1);

	imageSrc.convertTo(rChannel, CV_32FC1);
	imageSrc.convertTo(gChannel, CV_32FC1);
	imageSrc.convertTo(bChannel, CV_32FC1);

	gChannel *= 0.5;
	bChannel *= 0.2;

	for (int row = 0; row < imageSrc.rows; ++row)
	{
		auto *pG = gChannel.ptr<float>(row);
		auto *pB = bChannel.ptr<float>(row);
		for (int col = 0; col < imageSrc.cols; ++col)
		{
			*(pG + col) = cvRound(*(pG + col));
			*(pB + col) = cvRound(*(pB + col));
		}
	}

	cv::Mat imageColor(imageSrc.size(), CV_32FC3);
	cv::Mat channelArr[3] = {bChannel, gChannel, rChannel}; // BGR in OpenCV
	cv::merge(channelArr, 3, imageColor);
	imageColor.convertTo(imageColor, CV_8UC3);
	cv::imwrite("../images_output/color_image.png", imageColor);

	cv::Mat gaussianKernel = calGaussianKernel(31, 31, 7);
	cv::filter2D(imageColor, imageColor, CV_8U, gaussianKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
	cv::imwrite("../images_output/color_image_gaussian_filtered.png", imageColor);

	cv::waitKey(0);
	return 0;
}

/**
 * Generate Gaussian kernel
 * @details		kernel(x, y) = A * exp(-(x^2 + y^2) / (2*sigma^2))
 * @param rows	int: number of rows
 * @param cols	int: number of columns
 * @return		cv::Mat: Gaussian kernel
 */
cv::Mat calGaussianKernel(int rows, int cols, double sigma)
{
	CV_Assert(rows % 2 && cols % 2); // kernel size should be odd number
	int halfRow = rows / 2, halfCol = cols / 2;
	cv::Mat kernel(rows, cols, CV_32FC1, cv::Scalar::all(0));
	for (int i = -halfRow; i <= halfRow; ++i)
	{
		auto *pPixel = kernel.ptr<float>(i + halfRow); // +halfRow 从中心开始计算
		for (int j = -halfCol; j <= halfCol; ++j)
		{
			*(pPixel + j + halfCol) = cv::exp(-static_cast<float>(0.5 * (i * i + j * j) / sigma / sigma));
		}
	}

	double sum = cv::sum(kernel)(0);
	kernel /= sum; // normalize the kernel to make it sum as 1

	return kernel;
}