/** @file       homeworkQuestion3.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/22/2021
 *  @brief      
 *  @details    
 *  @note       
 */

#include <opencv2/core.hpp>

#include "imageUtils.h"
#include "matplotlibcpp.h"

std::vector<int> getHistogram(const cv::Mat &image, int grayscale);

int main()
{
	cv::Mat image1(512, 512, CV_8UC1, cv::Scalar(0));
	for (int i = 0; i < image1.rows; ++i)
	{
		for (int j = 256; j < image1.cols; ++j) { image1.at<uchar>(i, j) = 200; }
	}

	cv::Mat image2(512, 512, CV_8UC1, cv::Scalar(0));
	for (int i = 0; i < image2.rows; ++i)
	{
		for (int j = 0; j < image2.cols; ++j)
		{
			if ((i / 8 + j / 8) % 2 == 1) { image2.at<uchar>(i, j) = 0; }
			else { image2.at<uchar>(i, j) = 200; }
		}
	}

	cv::imshow("image1", image1);
	cv::imshow("image2", image2);

	cv::Mat image1Filtered, image2Filtered;
	cv::filter2D(image1, image1Filtered, -1, cv::Mat_<float>(3, 3, 1.0 / 9),
				 cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
	cv::filter2D(image2, image2Filtered, -1, cv::Mat_<float>(3, 3, 1.0 / 9), cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

	cv::imshow("image1 filtered", image1Filtered);
	cv::imshow("image2 filtered", image2Filtered);

	cv::waitKey(0);

	matplotlibcpp::figure();
	matplotlibcpp::bar(getHistogram(image1Filtered, 256));
	matplotlibcpp::title("histogram of filtered image1");
	matplotlibcpp::savefig("../images_out/image1_filtered.png");
	matplotlibcpp::show();

	matplotlibcpp::figure();
	matplotlibcpp::bar(getHistogram(image2Filtered, 256));
	matplotlibcpp::title("histogram of filtered image2");
	matplotlibcpp::savefig("../images_out/image2_filtered.png");
	matplotlibcpp::show();

	return 0;
}


/**
 * calculate histogram given an image
 * @param image cv::Mat &: input image
 * @param grayscale int: grayscale
 * @return std::vector<int>: histogram
 */
std::vector<int> getHistogram(const cv::Mat &image, int grayscale)
{
	std::vector<int> vHist(grayscale);
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j) { ++(vHist.at(image.at<uchar>(i, j))); }
	}
	return vHist;
}
