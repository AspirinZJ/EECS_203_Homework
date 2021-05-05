/** @file       grayLevelTransformation.cpp.c
 *  @author     jackzhang
 *  @version    1.0
 *  @date       04/13/2021
 *  @brief      gray level transformation
 *  @details    
 *  @note       
 */

#include <iostream>
#include <opencv2/core.hpp>
#include <vector>

#include "imageUtils.h"
#include "matplotlibcpp.h"

#define ROWS 480
#define COLS 640
#define L 256 // grayscale
#define GAMMA 0.4

int main()
{
	auto *image = static_cast<uchar *>(malloc(sizeof(uchar) * ROWS * COLS)); // allocate memory for image
	if (loadRawImage("../images/triangle.raw", image, ROWS, COLS)) { std::cout << "Load image successfully!\n"; }
	// convert uchar * array to cv::Mat image
	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);
	cv::imshow("source image", imgSrc);
	cv::imwrite("../images/out_img.png", imgSrc);
	cv::waitKey(0);

	std::vector<int> grayLevelIn; // [0, L-1]
	for (int i = 0; i < L; ++i) { grayLevelIn.push_back(i); }

	// 用于GLT变换将输入图像的gray level 映射到输出图像的 gray level
	std::vector<double> grayLevelGamma(grayLevelIn.begin(), grayLevelIn.end());

	// 将vector容器中的每个元素变为GAMMA次方， for_each的第三个参数可以是lambda表达式
	std::for_each(grayLevelGamma.begin(), grayLevelGamma.end(),
				  [](double &element) -> void { element = pow(element, GAMMA); });

	// std::max_element(iterator1, iterator2)返回最大值处的迭代器
	double maxElement = *std::max_element(grayLevelGamma.begin(), grayLevelGamma.end());

	// 将vector中的每个元素映射到[0, L-1]范围内
	std::for_each(grayLevelGamma.begin(), grayLevelGamma.end(),
			   [maxElement](double &element)->void {element = round((L-1)/maxElement * element);});

	matplotlibcpp::figure_size(720, 720);
	matplotlibcpp::plot(grayLevelIn, grayLevelGamma);
	matplotlibcpp::xlabel("r");
	matplotlibcpp::ylabel("s");
	matplotlibcpp::title("/gamma = 0.4");
	matplotlibcpp::xlim(0, L - 1);
	matplotlibcpp::ylim(0, L - 1);
	matplotlibcpp::grid(true);
	matplotlibcpp::savefig("../GLT.png");
	matplotlibcpp::show();


	cv::Mat imgDst = imgSrc.clone();
	for (int i = 0; i < imgSrc.rows; ++i)
	{
		for (int j = 0; j < imgSrc.cols; ++j)
		{
			imgDst.at<uchar>(i, j) = static_cast<uchar>(grayLevelGamma.at(imgSrc.at<uchar>(i, j)));
		}
	}

	cv::imshow("gray level transformed image", imgDst);
	cv::waitKey(0);

	return 0;
}
