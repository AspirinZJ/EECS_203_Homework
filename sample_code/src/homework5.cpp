/** @file       homework5.cpp
 *  @author     jackzhang
 *  @version    1.0
 *  @date       05/10/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void geoMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void harmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void contraHarmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize, double Q);
void myMedian(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);

int main()
{
	cv::Mat imageSrc(250, 249, CV_8UC1, cv::Scalar::all(10));

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 20; j < 20 + 210; ++j)
		{
			for (int k = 0; k < 7; ++k) { imageSrc.ptr<uchar>(j)[25 + i * 24 + k] = 240; }
		}
	}

	std::vector<int> vKernelSize{3, 7, 9};

	//	for (int size: vKernelSize)
	//	{
	//		cv::Mat imageDst;
	//		cv::blur(imageSrc, imageDst, cv::Size(size, size), cv::Point(-1, -1), cv::BORDER_CONSTANT);
	//
	//		std::string winName("arithmetic mean, kernel: ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}


	// geo mean filter
	//	for (int size:vKernelSize)
	//	{
	//		cv::Mat imageDst(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
	//		geoMeanFilter(imageSrc, imageDst, size);
	//
	//		std::string winName("geometric mean filter, kernel: ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}

	//	for (int size: vKernelSize)
	//	{
	//		cv::Mat imageDst(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
	//		harmonicMeanFilter(imageSrc, imageDst, size);
	//
	//		std::string winName("harmonic filter, kernel: ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}
	//	for (int size: vKernelSize)
	//	{
	//		cv::Mat imageDst(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
	//		contraHarmonicMeanFilter(imageSrc, imageDst, size, -1.0);
	//
	//		std::string winName("constra harmonic filter, kernel: ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}

	for (int size: vKernelSize)
	{
		cv::Mat imageDst(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		for (int i = 0; i < 20; ++i)
		{
			// cv::medianBlur(imageSrc, imageDst, size);
			myMedian(imageSrc, imageDst, size);
		}

		std::string winName("median filter ");
		winName += std::to_string(size);
		cv::imshow(winName, imageDst);
	}

	//	for (int size: vKernelSize)
	//	{
	//		cv::Mat imageDst;
	//		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
	//
	//		cv::dilate(imageSrc, imageDst, kernel);
	//
	//		std::string winName("max filter ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}

	//	for (int size: vKernelSize)
	//	{
	//		cv::Mat imageDst;
	//		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
	//
	//		cv::erode(imageSrc, imageDst, kernel);
	//
	//		std::string winName("min filter ");
	//		winName += std::to_string(size);
	//		cv::imshow(winName, imageDst);
	//	}

	//		for (int size: vKernelSize)
	//		{
	//			cv::Mat imageDst1, imageDst2, imageDst;
	//			cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size));
	//
	//			cv::erode(imageSrc, imageDst1, kernel);
	//			cv::dilate(imageSrc, imageDst2, kernel);
	//			imageDst = (imageDst1 + imageDst2) / 2;
	//
	//			std::string winName("midpoint filter ");
	//			winName += std::to_string(size);
	//			cv::imshow(winName, imageDst);
	//		}



	//	cv::Rect region(20, 15, 20, 20);
	//	std::cout << imageDst(region) << std::endl;

	cv::waitKey(0);
	return 0;
}

void geoMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	if (0 == kernelSize % 2)
	{
		std::cerr << "Error: kernel size must be odd" << std::endl;
		exit(EXIT_FAILURE);
	}
	const int halfSize = kernelSize / 2;
	const double exp = 1.0 / kernelSize / kernelSize;
	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			//int multiplication = 1;
			double multiplication = 1.0;

			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j)
				{
					// multiplication *= imageSrc.at<uchar>(row + i, col +j);
					multiplication *= pow(imageSrc.at<uchar>(row + 1, col + j), exp);
				}
			}
			// imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(pow(multiplication, 1.0 / kernelSize / kernelSize));
			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(multiplication);
		}
	}
}

void harmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	if (0 == kernelSize % 2)
	{
		std::cerr << "Error: kernel size must be odd" << std::endl;
		exit(EXIT_FAILURE);
	}
	const int halfSize = kernelSize / 2;
	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			double sum = 0.0;
			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j)
				{
					sum += 1.0 / imageSrc.at<uchar>(row + i, col + j);
				}
			}
			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(kernelSize * kernelSize / sum);
		}
	}
}

void contraHarmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize, double Q)
{
	if (0 == kernelSize % 2)
	{
		std::cerr << "Error: kernel size must be odd" << std::endl;
		exit(EXIT_FAILURE);
	}
	const int halfSize = kernelSize / 2;
	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			double numSum = 0.0;
			double denoSum = 0.0;
			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j)
				{
					numSum += pow(imageSrc.at<uchar>(row + i, col + j), Q + 1);
					denoSum += pow(imageSrc.at<uchar>(row + i, col + j), Q);
				}
			}

			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(numSum / denoSum);
		}
	}
}

void myMedian(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	CV_Assert(kernelSize % 2); // kernel size must be odd
	const int halfSize = kernelSize / 2;
	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			// cv::Rect region(x, y, width, height)
			cv::Rect region(col - halfSize, row - halfSize, kernelSize, kernelSize);
			cv::Mat roi = imageSrc(region);

			std::vector<uchar> vRegion;
			if (roi.isContinuous()) { vRegion.assign(roi.data, roi.data + roi.total() * roi.channels()); }
			else
			{
				for (int i = 0; i < roi.rows; ++i)
				{
					vRegion.insert(vRegion.end(), roi.ptr<uchar>(i), roi.ptr<uchar>(i) + roi.cols * roi.channels());
				}
			}

			 std::nth_element(vRegion.begin(), vRegion.begin() + static_cast<int>(vRegion.size()) / 2, vRegion.end());
			// std::sort(vRegion.begin(), vRegion.end());
			imageDst.at<uchar>(row, col) = vRegion.at(vRegion.size() / 2);
		}
	}
}

