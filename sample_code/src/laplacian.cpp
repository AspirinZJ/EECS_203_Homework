/** @file       laplacian.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/05/2021
 *  @brief      Apply Laplacian filter and sharpening filter to the image
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat imageSrc = cv::imread("../images/cat.png", cv::IMREAD_GRAYSCALE);
	cv::Mat imageLapcian(imageSrc.size(), CV_32F, cv::Scalar::all(0));

	double timeStamp = static_cast<double>(cv::getTickCount());
	for (int row = 1; row < imageLapcian.rows - 1; ++row)
	{
		//		const auto *previous = imageSrc.ptr<uchar>(row - 1);
		//		const auto *current = imageSrc.ptr<uchar>(row);
		//		const auto *next = imageSrc.ptr<uchar>(row + 1);
		//		auto *dst = imageLapcian.ptr<float>(row);

		for (int col = 1; col < imageLapcian.cols - 1; ++col)
		{
			//			dst[col] = static_cast<float>(previous[col - 1] + previous[col] + previous[col + 1] + current[col - 1] -
			//										  8 * current[col] + current[col + 1] + next[col - 1] + next[col] +
			//										  next[col + 1]);
			imageLapcian.at<float>(row, col) = static_cast<float>(imageSrc.at<uchar>(row - 1, col - 1) +
																  imageSrc.at<uchar>(row - 1, col) +
																  imageSrc.at<uchar>(row - 1, col + 1)
																  + imageSrc.at<uchar>(row, col - 1) -
																  8 * imageSrc.at<uchar>(row, col) +
																  imageSrc.at<uchar>(row, col + 1)
																  + imageSrc.at<uchar>(row + 1, col - 1) +
																  imageSrc.at<uchar>(row + 1, col) +
																  imageSrc.at<uchar>(row + 1, col + 1));
		}
	}
	timeStamp = (static_cast<double>(cv::getTickCount()) - timeStamp) / cv::getTickFrequency();
	std::cout << "Time: " << timeStamp * 1000 << "ms" << std::endl;

	//	cv::Rect roi(1, 1, imageSrc.cols - 2, imageSrc.rows - 2);
	//	cv::Mat laplacian = imageLapcian(roi).clone();
	//
	//	double minVal, maxVal;
	//	cv::minMaxLoc(laplacian, &minVal, &maxVal);
	//	laplacian -= minVal;
	//	laplacian *= 255.0 / (maxVal - minVal);
	//
	//	laplacian.convertTo(laplacian, CV_8U);
	//	cv::imshow("original image", imageSrc);
	//	cv::imshow("laplacian image", laplacian);
	//
	//	imageSrc.convertTo(imageSrc, CV_32F);
	//	cv::Mat imageSharpen = imageSrc(roi) - imageLapcian(roi);
	//	imageSharpen.convertTo(imageSharpen, CV_8U);
	//	cv::imshow("sharpened image", imageSharpen);
	//
	//
	//	cv::waitKey(0);

	return 0;
}
