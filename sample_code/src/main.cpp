#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>

#include "imageUtils.h"
#include "matplotlibcpp.h"

#define ROWS 480
#define COLS 640
#define L 256


int main()
{
	auto *image = (uchar *) malloc(sizeof(uchar) * COLS * ROWS);

	// 把raw图片加载成了array
	loadRawImage("../images/cat.raw", image, ROWS, COLS);
	// 转换成opencv的格式
	cv::Mat imgSrc = array2cvMat(image, ROWS, COLS);
	cv::imshow("source image", imgSrc);

	// 做一个映射， 把原始灰度映射到目标灰度
	std::vector<double> grayLevelIn; // vector 存输入图像的灰度
	for (int i = 0; i < L; ++i)
	{
		grayLevelIn.push_back(i); //  插值
	}

	// 将原始图像灰度做0.4次方变换
	std::vector<double> grayLevelGamma(grayLevelIn.begin(), grayLevelIn.end()); // 目标灰度
	// lambda表达式
	std::for_each(grayLevelGamma.begin(), grayLevelGamma.end(),
				  [](double &ele) -> void { ele = pow(ele, 0.4); }); // for_each对vector每个元素都做相同的函数变换
	// 把目标灰度限定到0～255
	double maxElement = *std::max_element(grayLevelGamma.begin(), grayLevelGamma.end()); // 变换后的灰度最大值 -> 255
	std::for_each(grayLevelGamma.begin(), grayLevelGamma.end(),
				  [maxElement](double &ele) -> void { ele = round((L - 1) / maxElement * ele); });

	matplotlibcpp::figure_size(700, 700);
	matplotlibcpp::plot(grayLevelIn, grayLevelGamma);
	matplotlibcpp::xlabel("r");
	matplotlibcpp::ylabel("s");
	matplotlibcpp::title("gamma = 0.4");
	matplotlibcpp::xlim(0, 255);
	matplotlibcpp::ylim(0, 255);
	matplotlibcpp::show();
	matplotlibcpp::savefig("../plot.png");


	cv::Mat imgTrans(imgSrc); // 新建一个变换后的图像
	for (int i = 0; i < imgSrc.rows; ++i)
	{
		for (int j = 0; j < imgSrc.cols; ++j)
		{
			// 转换，按照grayLevelIn -> grayLevelGamma 进行转换
			// imgTrans.at<uchar>(i, j)表示imagTrans在i行j列的像素值
			imgTrans.at<uchar>(i, j) = static_cast<uchar>(grayLevelGamma.at(imgSrc.at<uchar>(i, j))); // grayLevelGamma[imgSrc[i, j]]
		}
	}
	cv::imshow("transformed image", imgTrans);


	cv::waitKey(0);

	return 0;
}
