/** @file       linearAvgFilter.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/21/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <iostream>
#include <vector>
#include <algorithm>

#define ROWS 480
#define COLS 640

cv::Mat linearAvgFilter(cv::Mat &image, int kernelSize, const std::string &paddingMethod);

int main(int argc, char **argv)
{
	const char *imagePath = argc == 2 ? argv[1] : "../images/cat.raw";

	cv::Mat imgSrc = cv::imread(imagePath, cv::IMREAD_UNCHANGED);
	if (imgSrc.empty())
	{
		std::cerr << "Error: cannot load image!\n";
		exit(EXIT_FAILURE);
	}
	cv::imshow("original image", imgSrc);

	cv::Mat imgDst = linearAvgFilter(imgSrc, 11, "replicate");
	cv::imshow("filtered image", imgDst);

	cv::waitKey(0);


	return 0;
}

cv::Mat linearAvgFilter(cv::Mat &image, int kernelSize, const std::string &paddingMethod)
{
	if (kernelSize % 2 != 1)
	{
		std::cerr << "Error: kernel size must be odd!\n";
		exit(EXIT_FAILURE);
	}

	if ("replicate" == paddingMethod)
	{
		int paddingSize = kernelSize / 2;
		cv::Mat imgPad;
		cv::copyMakeBorder(image, imgPad, paddingSize, paddingSize, paddingSize, paddingSize, cv::BORDER_REPLICATE);

		cv::Mat imgDst(image.size(), image.type());

		for (int i = 0; i < imgDst.rows; ++i)
		{
			for (int j = 0; j < imgDst.cols; ++j)
			{
				std::vector<int> vnPixelVal;
				for (int p = -paddingSize; p <= paddingSize; ++p)
				{
					for (int q = -paddingSize; q <= paddingSize; ++q)
					{
						vnPixelVal.push_back(imgPad.at<uchar>(i + paddingSize + p, j + paddingSize + q));
					}
				}

				std::sort(vnPixelVal.begin(), vnPixelVal.end());
				imgDst.at<uchar>(i, j) = static_cast<uchar>(vnPixelVal.at(kernelSize * kernelSize / 2));
			}
		}
		return imgDst;
	}

}
