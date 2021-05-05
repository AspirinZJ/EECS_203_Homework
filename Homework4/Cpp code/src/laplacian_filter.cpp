/** @file       laplacian_filter.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/04/2021
 *  @brief      Apply Laplacian filter and sharpen filter based on laplacian
 *  @details    Laplacian kernel: [1, 1, 1; 1, -8, 1; 1, 1, 1]
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
	cv::String keys = "{@input | lena.jpg | input image path}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}
	cv::imshow("original image", imageSrc);

	imageSrc.convertTo(imageSrc, CV_32F); // convert source image to 32bit float image

	cv::Mat imageLaplacian(imageSrc.size(), CV_32F, cv::Scalar::all(0)); // image filtered by laplacian kernel

	// ignore the boundary pixels, row and col start from 1 to End-1
	for (int row = 1; row < imageLaplacian.rows - 1; ++row)
	{
		const auto *previous = imageSrc.ptr<float>(row - 1);
		const auto *current = imageSrc.ptr<float>(row);
		const auto *next = imageSrc.ptr<float>(row + 1);

		auto *dst = imageLaplacian.ptr<float>(row);

		for (int col = 1; col < imageLaplacian.cols - 1; ++col)
		{
			dst[col] = previous[col - 1] + previous[col] + previous[col + 1] +
					   current[col - 1] - 8 * current[col] + current[col + 1] +
					   next[col - 1] + next[col] + next[col + 1];
		}
	}

	// cut the boundary pixels
	cv::Rect roi(1, 1, imageSrc.cols - 2, imageSrc.rows - 2); // ignore the boundary pixels
	cv::Mat imageLaplacianTrans = imageLaplacian(roi).clone();

	double minVal{}, maxVal{};
	cv::minMaxLoc(imageLaplacianTrans, &minVal, &maxVal);
	// for each element in imageLaplacianTrans, subtract it with the minimum value to make it none negative
	imageLaplacianTrans -= minVal;
	// map it to [0, 255]
	imageLaplacianTrans *= 255.0 / (maxVal - minVal);

	imageLaplacianTrans.convertTo(imageLaplacianTrans, CV_8U);
	cv::imshow("transformed laplacian", imageLaplacianTrans);
	cv::imwrite("../images_output/laplacian_image.png", imageLaplacianTrans);

	// subtract the original image by laplacian image get the sharpened image
	cv::Mat imageSharpen = imageSrc(roi) - imageLaplacian(roi);
	imageSharpen.convertTo(imageSharpen, CV_8U);
	cv::imshow("sharpened image", imageSharpen);
	cv::imwrite("../images_output/sharpened_image.png", imageSharpen);

	cv::waitKey(0);
	return 0;
}