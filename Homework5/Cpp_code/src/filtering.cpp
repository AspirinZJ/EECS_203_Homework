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
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

void geometricMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void harmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);
void contraHarmonicMeanFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize, double coef);
void medianFilter(const cv::Mat &imageSrc, cv::Mat &imageDst, int kernelSize);

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

	// ~~~~~~~~~~~~~~arithmetic mean filter: cv::blur or cv::boxFilter~~~~~~~~~~~~~
	std::vector<int> vKernelSize = {3, 7, 9};
	for (int size: vKernelSize)
	{
		cv::Mat imageArithmeticMean;
		cv::blur(imageSrc, imageArithmeticMean, cv::Size(size, size));
		//		std::string winName = "arithmetic mean ";
		// cv::imshow(winName + std::to_string(size) + "x" + std::to_string(size), imageArithmeticMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~~~~~geometric mean filter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageGeoMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));

		geometricMeanFilter(imageSrc, imageGeoMean, size);
		//		std::string winName("geometric mean ");
		//		winName = std::to_string(size) + "x" + std::to_string(size);
		//		cv::imshow(winName, imageGeoMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~~Harmonic Mean Filter~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageHarmMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		harmonicMeanFilter(imageSrc, imageHarmMean, size);
		//				std::string winName("harmonic mean ");
		//				winName += std::to_string(size) + "x" + std::to_string(size);
		//				cv::imshow(winName, imageHarmMean);
	}

	// ~~~~~~~~~~~~~~~~~~~~~Contra harmonic mean filter ~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		for (int Q : std::vector<int>{1, -1})
		{
			cv::Mat imageContraHarmonicMean(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
			contraHarmonicMeanFilter(imageSrc, imageContraHarmonicMean, size, Q);

			//			std::string winName("contra hamonic mean ");
			//			winName += std::to_string(size) + "x" + std::to_string(size) + ". Q=" + std::to_string(Q);
			//			cv::imshow(winName, imageContraHarmonicMean);
		}
	}

	// ~~~~~~~~~~~~~~~~~~~~~~Median filter~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMedian(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));
		medianFilter(imageSrc, imageMedian, size);
		// cv::medianBlur(imageSrc, imageMedian, size);

		//		std::string winName("median filter");
		//		winName += std::to_string(size);
		//		cv::imshow(winName, imageMedian);
	}

	// ~~~~~~~~~~~~~~~~~~~~~Max filter cv::dilate~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMax(imageSrc.size(), imageSrc.type(), cv::Scalar::all(10));

		// use cv::dilate to do the max filtering
		// get the kernel(morphological shape)
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		cv::dilate(imageSrc, imageMax, kernel);

		//		std::string winName("max filter ");
		//		winName += std::to_string(size);
		//		cv::imshow(winName, imageMax);
	}

	// ~~~~~~~~~~~~~~~~~~~Min filter cv::erode~~~~~~~~~~~~~~~~~~~~~~~~
	for (int size : vKernelSize)
	{
		cv::Mat imageMin;

		// use cv::erode to do the min filtering
		// create the kernel(morphological shape)
		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		cv::erode(imageSrc, imageMin, kernel);

		//		std::string winName("Min filter ");
		//		winName += std::to_string(size);
		//		cv::imshow(winName, imageMin);
	}

	// ~~~~~~~~~~~~~~~~~~Midpoint filter => mean of max filter and min filter~~~~~~~~~~~~~~
	for (int size: vKernelSize)
	{
		cv::Mat imageMax, imageMin, imageMidpoint;

		cv::Mat kernel = cv::getStructuringElement(cv::MORPH_RECT, cv::Size(size, size), cv::Point(-1, -1));
		cv::dilate(imageSrc, imageMax, kernel);    // dilate the image => max filter
		cv::erode(imageSrc, imageMin, kernel); // erode the image => min filter
		imageMidpoint = (imageMax + imageMin) / 2;

		//		std::string winName("midpoint filter ");
		//		winName += std::to_string(size);
		//		cv::imshow(winName, imageMidpoint);
	}

	cv::waitKey(0);
	return 0;
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

			// STL中的nth_element()方法的使用: 通过调用nth_element(start, start+n, end)方法
			// 可以使第n大元素处于第n位置（从0开始,其位置是下标为n的元素）
			// 并且比这个元素小的元素都排在这个元素之前，比这个元素大的元素都排在这个元素之后，但不能保证他们是有序的
			// since kernel size is odd we don't need to take even case into consideration
			std::nth_element(vRegion.begin(), vRegion.begin() + static_cast<int>(vRegion.size()) / 2, vRegion.end());

			imageDst.at<uchar>(row, col) = vRegion.at(vRegion.size() / 2);
		}
	}
}
