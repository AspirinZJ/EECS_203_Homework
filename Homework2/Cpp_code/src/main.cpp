#include <iostream>
#include <opencv2/opencv.hpp>
#include <vector>
#include <algorithm>

#include "imageUtils.h"
// run pip install matplotlib
#include "matplotlibcpp.h" // for plotting like Matlab

#define ROWS 480
#define COLS 640
#define L 256    // grayscale
#define GAMMA 2.5

void grayLevelTransformation(cv::Mat &image);
void histogramEqualization(cv::Mat &image);
int *getHistogram(cv::Mat &image, int grayScale);
int *getCumuDist(const int *hist, int grayscale);
void plot(int *arr, int size, const char *title);
void plotSave(const int *arr, int size, const char *title, const char *savePath);

int main(int argc, char **argv)
{
	if (argc != 2)
	{
		std::cerr << "usage: " << argv[0] << " image_name" << std::endl;
		return 1;
	}

	auto *image = static_cast<uchar *>(malloc(sizeof(uchar) * ROWS * COLS)); // allocate memory for image
	if (loadRawImage(argv[1], image, ROWS, COLS)) { std::cout << "Load image successfully!\n"; }

	// convert uchar * array to cv::Mat object
	cv::Mat imgMat = array2cvMat(image, ROWS, COLS);
	cv::imshow("source image", imgMat);
	grayLevelTransformation(std::ref(imgMat));
	histogramEqualization(std::ref(imgMat));

	return 0;
}

/**
 * gray level transformation
 * @param imageName image name
 */
void grayLevelTransformation(cv::Mat &image)
{
	std::vector<int> grayLevelIn; // 0~255
	for (int i = 0; i < L; ++i) { grayLevelIn.push_back(i); }

	// 用于GLT变换将输入图像的gray level映射到输出图像的gray level
	std::vector<double> grayLevelGamma1(grayLevelIn.begin(), grayLevelIn.end());

	// 将vector容器中的每个元素变为0.4次方， for_each的第三个参数可以接收lambda表达式
	// [](double &ele)->void{ele = pow(ele, GAMMA); }是一个lambda表达式
	std::for_each(grayLevelGamma1.begin(), grayLevelGamma1.end(), [](double &ele) -> void { ele = pow(ele, GAMMA); });

	// std::max_element(iterator1, iterator2); 返回最大值出的迭代器
	double maxElement = *std::max_element(grayLevelGamma1.begin(), grayLevelGamma1.end());
	// 将vector容器中的每个元素映射到[0, L-1]范围内
	std::for_each(grayLevelGamma1.begin(), grayLevelGamma1.end(),
				  [maxElement](double &ele) -> void { ele = round((L - 1) / maxElement * ele); });

	matplotlibcpp::figure_size(720, 720);
	matplotlibcpp::plot(grayLevelIn, grayLevelGamma1);
	matplotlibcpp::xlabel("r");
	matplotlibcpp::ylabel("s");
	matplotlibcpp::title("/gamma = 0.4");
	matplotlibcpp::xlim(0, L - 1);
	matplotlibcpp::ylim(0, L - 1);
	matplotlibcpp::grid(true);
	matplotlibcpp::savefig("../GLT.png");
	matplotlibcpp::show();


	cv::Mat imgOut(image);

	for (int i = 0; i < image.rows; ++i)
	{
		// auto *pixelPtr = image.ptr<uchar>(i);
		for (int j = 0; j < image.cols; ++j)
		{
			// 按graylevel转换表进行转换
			imgOut.at<uchar>(i, j) = static_cast<uchar>(grayLevelGamma1.at(image.at<uchar>(i, j)));
		}
	}
	cv::imshow("gray level transformed image", imgOut);

	cv::waitKey(0);
	cv::imwrite("../cat_GLT.jpg", imgOut);
}

/**
 * histogram equalization demo
 * @param image cv::Mat, input image
 */
void histogramEqualization(cv::Mat &image)
{
	// get histogram of source image
	// TODO: free it
	int *histSrc = getHistogram(std::ref(image), L);
	plotSave(histSrc, L, "source image histogram", "../histogram_original.png");

	// get cumulative distribution of source image

	// TODO: free it
	int *cumuDistSrc = getCumuDist(histSrc, L);
	plotSave(cumuDistSrc, L, "source image cumulative distribution", "../cumulative_dist_original.png");

	// calculate cumulative distribution of desired image
	int cumuDistDes[L] = {0};
	double sumDes = 0;
	double histDesEach = static_cast<double>(image.rows) * image.cols / L;
	for (int &element : cumuDistDes)
	{
		sumDes += histDesEach;
		element = static_cast<int>(round(sumDes));
	}

	// map which maps source image gray level to desired image gray level
	uchar src2DesMap[L]{};
	for (int i = 0; i < L; ++i)
	{
		int min = std::abs(cumuDistDes[0] - cumuDistSrc[i]);
		int minInd = 0;
		for (int j = 1; j < L; ++j)
		{
			// find the gray scale on desired cumulative distribution which is closest to the current cumulative distribution
			if (std::abs(cumuDistDes[j] - cumuDistSrc[i]) < min)
			{
				minInd = j;
				min = std::abs(cumuDistDes[j] - cumuDistSrc[i]);
			}
		}
		src2DesMap[i] = minInd; // give the index of closest to src to des map
	}

	// get histogram-equalized image
	cv::Mat histEquImg(image);
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			histEquImg.at<uchar>(i, j) = src2DesMap[image.at<uchar>(i, j)];
		}
	}
	cv::imshow("histogram-equalized image", histEquImg);
	cv::imwrite("../histogram_equalized_cat.png", histEquImg);
	cv::waitKey(0);

	// get histogram of histogram-equalized image
	// TODO: free it
	int *histDst = getHistogram(std::ref(histEquImg), L);
	plotSave(histDst, L, "output image histogram", "../histogram_output.png");

	int *cumuDst = getCumuDist(histDst, L);
	plotSave(cumuDst, L, "output image cumulative distribution", "../cumulative_dist_output.png");
}

/**
 * Calculate the histogram from a cv::Mat image
 * @param image cv::Mat: image
 * @param grayScale int: gray scale
 * @return int *: array of histogram on heap
 */
int *getHistogram(cv::Mat &image, int grayScale)
{
	// int histSrc[grayScale] = {0};
	int *histSrc = new int[grayScale](); // 每个元素初始化为0
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			// histSrc[image.at<uchar>(i, j)]++;
			(*(histSrc + image.at<uchar>(i, j)))++;
		}
	}

	return histSrc;
}

/**
 * get accumulative distribution from histogram
 * @param hist int *: input histogram
 * @param grayscale int: gray scale
 * @return int *: accumulative distribution
 */
int *getCumuDist(const int *hist, int grayscale)
{
	int *cumuDist = new int[grayscale](); // 初始化所有元素为0
	int sum = 0;
	for (int i = 0; i < grayscale; ++i)
	{
		sum += *(hist + i);
		*(cumuDist + i) = sum;
	}
	return cumuDist;
}

/**
 * plot the input array and save png image
 * @param arr const int *: input array
 * @param size int: array size
 * @param title const char *: plot title
 * @param savePath const char *: path of the image to save
 */
void plotSave(const int *arr, int size, const char *title, const char *savePath)
{
	// convert the input array to vector
	std::vector<int> vec(arr, arr + size);

	matplotlibcpp::figure();
	matplotlibcpp::bar(vec);
	matplotlibcpp::title(title);
	matplotlibcpp::grid(true);
	matplotlibcpp::savefig(savePath);
	matplotlibcpp::show();
}

/**
 * plot the give array
 * @param arr int *: input array
 * @param size int: array size
 * @param title const char *: plot title
 */
void plot(int *arr, int size, const char *title)
{
	// convert the input array to vector
	std::vector<int> vec(arr, arr + size);

	matplotlibcpp::figure();
	matplotlibcpp::bar(vec);
	matplotlibcpp::title(title);
	matplotlibcpp::grid(true);
	matplotlibcpp::show();
}
