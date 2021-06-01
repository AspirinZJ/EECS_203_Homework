/** @file       question1.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/28/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <iomanip>
#include <opencv2/highgui.hpp>

int main()
{
	cv::Mat imageCenterSample(10, 10, CV_32FC1, cv::Scalar::all(0));
	std::vector<float> yCoordinates;
	for (int i = 0; i < 11; ++i) { yCoordinates.push_back(0.3f * static_cast<float>(i)); }

	for (int col = 0; col < imageCenterSample.cols; ++col)
	{
		for (int row = 0; row < imageCenterSample.rows; ++row)
		{
			float y = imageCenterSample.rows - row - 0.5f;
			if (y <= 0.5 * (yCoordinates.at(col) + yCoordinates.at(col + 1)))
			{
				imageCenterSample.at<float>(row, col) = 1.0f;
			}
				// else { imageCenterSample.at<float>(row, col) = 0.5f * (yCoordinates.at(col) - (int)yCoordinates.at(col) + yCoordinates.at(col + 1) - (int)yCoordinates.at(col + 1));}
			else { imageCenterSample.at<float>(row, col) = 0.0f; }
		}
	}

	std::cout << "Image sampled with the center value: \n" << imageCenterSample << std::endl;

	cv::Mat imageAvgSample(imageCenterSample.size(), CV_32FC1, cv::Scalar::all(0));
	for (int col = 0; col < imageAvgSample.cols; ++col)
	{
		for (int row = 0; row < imageAvgSample.rows; ++row)
		{
			float y = imageAvgSample.rows - row; // y coordinate, upper edge

			if (y <= yCoordinates.at(col)) // y is covered by the line y = 0.3x totally
			{
				imageAvgSample.at<float>(row, col) = 1.0f;
			}
			else if (y - 1 >= yCoordinates.at(col + 1))  // y is not covered by the line y = 0.3x at all
			{ imageAvgSample.at<float>(row, col) = 0.0f; }
			else // y is covered by the line y = 0.3x partially
			{
				float value;
				if (yCoordinates.at(col) < y && yCoordinates.at(col + 1) > y)
				{
					float triangleEdge1 = (int) yCoordinates.at(col) + 1 - yCoordinates.at(col);
					float triangleEdge2 = triangleEdge1 / 0.3f;
					value = 1.0f - 0.5 * triangleEdge1 * triangleEdge2;
					if (value < 0)
						std::cout << "triangleEdge1: " << triangleEdge1 << " triangleEdge2: " << triangleEdge2
								  << std::endl;
				}
				else if (yCoordinates.at(col) < y - 1 && yCoordinates.at(col + 1) > y - 1)
				{
					float triangleEdge1 = yCoordinates.at(col + 1) - (int) yCoordinates.at(col + 1);
					float triangleEdge2 = triangleEdge1 / 0.3f;
					value = 0.5f * triangleEdge1 * triangleEdge2;
				}
				else
				{
					value = 0.5f * (yCoordinates.at(col) - (int) yCoordinates.at(col) + yCoordinates.at(col + 1) -
								   (int) yCoordinates.at(col + 1));
				}
				imageAvgSample.at<float>(row, col) = value;
			}
		}
	}

	// 控制输出格式， std::setiosflags固定小数位数，否则是有效数字，setprecision保留位数， 需include <iomanip>
	std::cout << std::setiosflags(std::ios::fixed) << std::setprecision(3);
	std::cout << "Image sampled with avarage value: \n";
	for (int row = 0; row < imageAvgSample.rows; ++row)
	{
		const auto *pPixel = imageAvgSample.ptr<float>(row);
		for (int col = 0; col < imageAvgSample.cols; ++col)
		{
			std::cout << *(pPixel + col) << " ";
		}
		std::cout << std::endl;
	}

	cv::waitKey(0);

	return 0;
}