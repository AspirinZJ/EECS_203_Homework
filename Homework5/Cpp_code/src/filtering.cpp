/** @file       filtering.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/09/2021
 *  @brief      Apply different filters to a binary image
 *  @details    Apply arithmetic mean filter, geometric mean filter, harmonic mean filter, contraharmonic mean filter
 *  			median filter, max filter, min filter and midpoint filter
 *  @note
 *  @bug
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void arithmeticMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, const cv::Size &size);
void geometricMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void harmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void contraHarmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize, double coef);
void medianFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void maxFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);

int main()
{
	// create a image to be filtered
	cv::Mat imageSrc(250, 249, CV_8U, cv::Scalar::all(10));

	for (int i = 0; i < 9; ++i)
	{
		for (int j = 20; j < 230; ++j)
		{
			for (int k = 0; k < 7; ++k) { imageSrc.ptr<uchar>(j)[25 + i * 24 + k] = 240; }
		}
	}

	cv::Rect showRegion(19, 14, 19, 19);
	std::cout << "original image region: \n" << imageSrc(showRegion) << std::endl;

	// ~~~~~~~~~~~~~~arithmetic mean filter: cv::blur or cv::boxFilter~~~~~~~~~~~~~
	std::vector<int> vKernelSize = {3, 7, 9};
	for (int size: vKernelSize)
	{
		cv::Mat imageArithmeticMean(imageSrc.size(), imageSrc.type(), CV_8UC1);
		// cv::blur(imageSrc, imageArithmeticMean, cv::Size(size, size));
		arithmeticMeanFilter(imageSrc, imageArithmeticMean, cv::Size(size, size));
		std::string outName("../images/arithmetic_mean_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageArithmeticMean);

		// std::cout << "arithmetic mean kernel size: " << size << std::endl << imageArithmeticMean(showRegion)
		// << std::endl;
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~geometric mean filter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageGeoMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		geometricMeanFilter(imageSrc, imageGeoMean, size);
		std::string outName("../images/geometric_mean_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageGeoMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~Harmonic Mean Filter~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageHarmMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		harmonicMeanFilter(imageSrc, imageHarmMean, size);
		std::string outName("../images/harmonic_mean_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageHarmMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~Contra harmonic mean filter ~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		for (int Q : std::vector<int>{1, -1})
		{
			cv::Mat imageContraHarmonicMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
			contraHarmonicMeanFilter(imageSrc, imageContraHarmonicMean, size, Q);
			std::string outName("../images/contra_harmonic_mean_kernel_");
			outName += std::to_string(size) + "_Q_" + std::to_string(Q) + ".png";
			cv::imwrite(outName, imageContraHarmonicMean);
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~Median filter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMedian(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		medianFilter(imageSrc, imageMedian, size);
		// cv::medianBlur(imageSrc, imageMedian, size);

		std::string outName("../images/median_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageMedian);
	}

	// ~~~~~~~~~~~~~~~~~~~~~Max filter cv::dilate~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMax(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));

		// use cv::dilate to do the max filtering
		// get the kernel(morphological shape)
		// cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		// cv::dilate(imageSrc, imageMax, kernel);

		// or implement it by ourselves
		maxFilter(imageSrc, imageMax, size);

		std::string outName("../images/max_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageMax);
	}

	// ~~~~~~~~~~~~~~~~~~~Min filter cv::erode~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size : vKernelSize)
	{
		cv::Mat imageMin;

		// use cv::erode to do the min filtering
		// create the kernel(morphological shape)
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		cv::erode(imageSrc, imageMin, kernel);

		std::string outName("../images/min_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageMin);
	}

	// ~~~~~~~~~~~~~~~~~~Midpoint filter => mean of max filter and min filter~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMax, imageMin, imageMidpoint;

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		cv::dilate(imageSrc, imageMax, kernel);    // dilate the image => max filter
		cv::erode(imageSrc, imageMin, kernel); // erode the image => min filter
		imageMidpoint = (imageMax + imageMin) / 2;

		std::string outName("../images/midpoint_kernel_");
		outName += std::to_string(size) + ".png";
		cv::imwrite(outName, imageMidpoint);
	}

	cv::waitKey(0);
	return 0;
}


/**
 * Apply arithmetic mean filter to the input image
 * @param imageSrc 	const cv::Mat &: input image
 * @param imageDst 	cv::Mat &: output image
 * @param size		cv::Size &: kernel size
 */
void arithmeticMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, const cv::Size &size)
{
	CV_Assert(size.width == size.height); // for now can only process a squere shaped kernel
	CV_Assert(size.width % 2);
	const int halfSize = size.width / 2;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			int sum = 0;
			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j) { sum += imageSrc.at<uchar>(row + i, col + j); }
			}

			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(1.0 * sum / size.width / size.width);
		}
	}
}

/**
 * filter the input image with geometric mean filter
 * @param imageSrc		const cv::Mat &: input image
 * @param imageDst 		cv::Mat &: output image
 * @param kernelSize 	int: kernel size
 */
void geometricMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	CV_Assert(kernelSize % 2); // kernel size must be odd
	const int halfSize = kernelSize / 2;
	const double exponent = 1.0 / kernelSize / kernelSize;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)    // ignore the boundary pixels
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			double multiplication = 1.0;
			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j)
				{
					multiplication *= pow(imageSrc.at<uchar>(row + i, col + j), exponent);
				}
			}
			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(multiplication);
		}
	}
}

/**
 * use harmonic mean filter to filter the image
 * @param imageSrc		const cv::Mat &: input image
 * @param imageDst		cv::Mat &: output image
 * @param kernelSize	int: kernel size
 */
void harmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	CV_Assert(kernelSize % 2); // kernel size must be odd
	const int halfSize = kernelSize / 2;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)    // ignore the boundary pixels
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

/**
 * use contra harmonic mean filter to filter the image
 * @param imageSrc 		const cv::Mat &: input image
 * @param imageDst 		cv::Mat &: output image
 * @param kernelSize 	int: kernel size
 * @param coef			double: power exponential coefficient
 */
void contraHarmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize, double coef)
{
	CV_Assert(kernelSize % 2); // kernel size must be odd number
	const int halfSize = kernelSize / 2;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)    // ignore the boundary pixels
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			double numeratorSum = 0.0;
			double denominatorSum = 0.0;
			for (int i = -halfSize; i <= halfSize; ++i)
			{
				for (int j = -halfSize; j <= halfSize; ++j)
				{
					numeratorSum += pow(imageSrc.at<uchar>(row + i, col + j), coef + 1);
					denominatorSum += pow(imageSrc.at<uchar>(row + i, col + j), coef);
				}
			}
			imageDst.at<uchar>(row, col) = cv::saturate_cast<uchar>(numeratorSum / denominatorSum);
		}
	}
}

/**
 * Apply median filter to the image
 * @param imageSrc 		const cv::Mat &: input image
 * @param imageDst 		cv::Mat &: output image
 * @param kernelSize 	int: kernel size
 */
void medianFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	CV_Assert(kernelSize % 2); // kernel size must be an odd number
	const int halfSize = kernelSize / 2;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)    // ignore the boundary pixels
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			cv::Mat region = imageSrc(cv::Rect(col - halfSize, row - halfSize, kernelSize, kernelSize));

			std::vector<uchar> vRegion;
			if (region.isContinuous()) // if the mat is continuous, assign and convert it to a vector directly
			{
				vRegion.assign(region.data, region.data + region.total() * region.channels());
			}
			else // if it is not continuous in memory, assign and convert it to a vector row by row
			{
				for (int i = 0; i < region.rows; ++i)
				{
					vRegion.insert(vRegion.end(), region.ptr<uchar>(i),
							region.ptr<uchar>(i) + region.cols * region.channels());
				}
			}

			// STL??????nth_element()???????????????: ????????????nth_element(start, start+n, end)??????
			// ????????????n??????????????????n????????????0??????,?????????????????????n????????????
			// ?????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????????
			// since kernel size is odd we don't need to take even case into consideration
			std::nth_element(vRegion.begin(), vRegion.begin() + static_cast<int>(vRegion.size()) / 2, vRegion.end());

			imageDst.at<uchar>(row, col) = vRegion.at(vRegion.size() / 2);
		}
	}
}

/**
 * apply max filter to the input image
 * @param imageSrc		const cv::Mat &: input image
 * @param imageDst 		cv::Mat &: output image
 * @param kernelSize 	int: kernel size
 */
void maxFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize)
{
	CV_Assert(kernelSize % 2); // kernel size must be odd number
	const int halfSize = kernelSize / 2;

	for (int row = halfSize; row < imageSrc.rows - halfSize; ++row)
	{
		for (int col = halfSize; col < imageSrc.cols - halfSize; ++col)
		{
			cv::Rect region(col - halfSize, row - halfSize, kernelSize, kernelSize);
			cv::Mat roi = imageSrc(region); // select the region of area , generally it is not continuous in memory
			std::vector<uchar> vRoi;

			if (roi.isContinuous()) // if it is continuous in memory, assign the value to the vector directly
			{
				vRoi.assign(roi.data, roi.data + roi.total() * roi.channels());
			}
			else // if is not continuous in memory , we need to assign the value row by row
			{
				for (int i = 0; i < roi.rows; ++i)
				{
					vRoi.insert(vRoi.end(), roi.ptr<uchar>(i), roi.ptr<uchar>(i) + roi.cols * roi.channels());
				}
			}
			auto maxValInd = std::max_element(vRoi.begin(), vRoi.end());
			imageDst.at<uchar>(row, col) = *maxValInd;
		}
	}
}
