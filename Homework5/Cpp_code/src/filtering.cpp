/** @file       filtering.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/09/2021
 *  @brief      Apply different filters to a binary image
 *  @details    Apply arithmetic mean filter, geometric mean filter, harmonic mean filter, contraharmonic mean filter
 *  			median filter, max filter, min filter and midpoint filter
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

int main()
{
	// create a image to be filtered
	cv::Mat imageSrc(250, 249, CV_8U, cv::Scalar::all(10));

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 20; j < 230; ++j)
		{
			for (int k = 0; k < 7; ++k) { imageSrc.ptr<uchar>(j)[25 + i * 24 + k] = 240; }
		}
	}
	// cv::imshow("original image", imageSrc);

	// ~~~~~~~~~~~~~~arithmetic mean filter: cv::blur or cv::boxFilter~~~~~~~~~~~~~
	std::vector<int> vKernelSize = {3, 7, 9};
	for (int size: vKernelSize)
	{
		cv::Mat imageArithmeticMean;
		cv::blur(imageSrc, imageArithmeticMean, cv::Size(size, size));
		std::string winName = "arithmetic mean ";
		// cv::imshow(winName + std::to_string(size) + "x" + std::to_string(size), imageArithmeticMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~geometric mean filter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		CV_Assert(size % 2); // kernel size must be an odd number
		const int halfSize = size / 2;
		const double exponent = 1.0 / size / size;
		cv::Mat imageGeoMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));

		for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
		{
			for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
			{
				double multiplication = 1.0;
				for (int i = -halfSize; i <= halfSize; ++i)
				{
					for (int j = -halfSize; j <= halfSize; ++j)
					{
						multiplication *= pow(imageSrc.at<uchar>(row + i, col + j), exponent);
					}
				}

				imageGeoMean.at<uchar>(row, col) = cv::saturate_cast<uchar>(multiplication);
			}
		}

		std::string winName("geometric mean ");
		winName = std::to_string(size) + "x" + std::to_string(size);
		//		cv::imshow(winName, imageGeoMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~Harmonic Mean Filter~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		CV_Assert(size % 2); // kernel size must be odd
		const int halfSize = size / 2;
		cv::Mat imageHarmMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));

		for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
		{
			for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
			{
				double sum = 0.0;
				for (int i = -halfSize; i <= halfSize; ++i)
				{
					for (int j = -halfSize; j <= halfSize; ++j)
					{
						sum += 1.0 / imageSrc.at<uchar>(row + i, col + j);
					}
				}
				imageHarmMean.at<uchar>(row, col) = cv::saturate_cast<uchar>(size * size / sum);
			}
		}
		std::string winName("harmonic mean ");
		winName += std::to_string(size) + "x" + std::to_string(size);
		cv::imshow(winName, imageHarmMean);
	}


	cv::waitKey(0);
	return 0;
}
