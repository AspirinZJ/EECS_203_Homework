/** @file       question1.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/29/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>

int main()
{
	cv::Mat imageSrc(10, 10, CV_32FC1, cv::Scalar::all(0));
	const float P = 1;

	std::vector<float> yCoordinates;
	for (int i = 0; i < 11; ++i) { yCoordinates.push_back(0.3f * i); }

	//	for (int col = 0; col < 10; ++col)
	//	{
	//		float yMiddle = (yCoordinates.at(col) + yCoordinates.at(col + 1)) / 2.0f; // 中点y值
	//		for (int row = 0; row < 10; ++row)
	//		{
	//			float y = 9.0f - row + 0.5f;
	//
	//			if (y <= yMiddle) { imageSrc.at<float>(row, col) = P; }
	//			// else {imageSrc.at<float>(row, col) = 0.0f;}
	//		}
	//	}

	for (int col = 0; col < 10; ++col)
	{
		for (int row = 0; row < 10; ++row)
		{
			float y = 9 - row;
			if (y + 1 <= yCoordinates.at(col)) { imageSrc.at<float>(row, col) = P; }// 格子在直线下方
			else if (y >= yCoordinates.at(col + 1)) { ; }
			else	// 被直线穿过
			{

			}
		}
	}


	return 0;
}

