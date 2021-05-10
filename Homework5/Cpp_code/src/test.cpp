/** @file       test.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/09/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>

int main()
{
//	cv::Mat imageSrc = cv::Mat::eye(cv::Size(5, 5), CV_8UC1);
//	cv::Mat region = imageSrc(cv::Rect(1, 1, 3, 3));
//	std::cout << region << std::endl;
cv::Mat region = cv::Mat::eye(cv::Size(3, 3), CV_8UC1);

	//	std::vector<uchar> vec = region.reshape(0, 1);
	//	vec.at(3) = 10;

	std::vector<uchar> vec;
	if (region.isContinuous())
	{
		// vec.assign(region.data, region.data + region.total() * region.channels());
		vec = region.reshape(0, 1);
		std::cout << "is continous" << std::endl;
	}
	else
	{
		for (int i = 0; i < region.rows; ++i)
		{
			vec.insert(vec.end(), region.ptr<uchar>(i), region.ptr<uchar>(i) + region.cols * region.channels());
		}
	}

	vec.at(3) = 8;
	for (auto ele : vec)
	{
		std::cout << static_cast<int>(ele) << " ";
	}
	std::cout << std::endl;

	std::cout << region << std::endl;
}
