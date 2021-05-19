/** @file       homework6.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/18/2021
 *  @brief      
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

cv::Mat getKernel(int rows, int cols, double sigma)
{
	CV_Assert(rows % 2 && cols % 2);

	int halfRow = rows / 2, halfCol = cols / 2;
	cv::Mat kernel(rows, cols, CV_32FC1);

	for (int row = -halfRow; row <= halfRow; ++row)
	{
		auto *pPixel = kernel.ptr<float>(row + halfRow);
		for (int col = -halfCol; col <= halfCol; ++col)
		{
			*(pPixel + col + halfCol) = cv::exp(-(row * row + col * col) / sigma / sigma / 2.0f);
		}
	}

	double sum = cv::sum(kernel)(0);
	kernel /= sum;

	return kernel;
}

cv::Mat discreteFourierTrans(const cv::Mat &image)
{
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(image.rows), n = cv::getOptimalDFTSize(image.cols);
	cv::copyMakeBorder(image, padded, 0, m - image.rows, 0, n - image.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::Mat planes[2] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32FC1)};
	cv::Mat complex;
	cv::merge(planes, 2, complex);
	cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);

	return complex;
}

inline cv::Vec2f complexDivision(const cv::Vec2f &num1, const cv::Vec2f &num2)
{
	float a = num1(0), b = num1(1), c = num2(0), d = num2(1);
	float realPart = (a * c + b * d) / (c * c + d * d);
	float imgPart = (b * c - a * d) / (c * c + d * d);
	return cv::Vec2f(realPart, imgPart);
}

cv::Mat complexMatDivision(const cv::Mat &mat1, const cv::Mat &mat2)
{
	CV_Assert(mat1.size() == mat2.size());
	cv::Mat matOut(mat1.size(), CV_32FC2);

	for (int row = 0; row < matOut.rows; ++row)
	{
		const auto *pPixel1 = mat1.ptr<cv::Vec2f>(row);
		const auto *pPixel2 = mat2.ptr<cv::Vec2f>(row);
		auto *pPixelOut = matOut.ptr<cv::Vec2f>(row);
		for (int col = 0; col < matOut.cols; ++col)
		{
			// 对应像素做复数除法
			*(pPixelOut + col) = complexDivision(*(pPixel1 + col), *(pPixel2 + col));
		}
	}
	return matOut;
}

int main(int argc, char **argv)
{
	cv::String keys = "{@input | ./cat.png | input image}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image: " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}

	cv::Mat gaussianKernel = getKernel(31, 31, 7);
	// std::cout << "Gaussian kernel: \n" << gaussianKernel << std::endl;
	cv::Mat imageDegraded;
	cv::filter2D(imageSrc, imageDegraded, CV_8U, gaussianKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);

	// cv::Mat imageDegradedDFT = discreteFourierTrans(imageDegraded); // => G
	// cv::Mat gaussianKernelDFT = discreteFourierTrans(gaussianKernel); // => H

	// F = G / H
	// todo: align the size
	// cv::Mat imageDFT = complexMatDivision(imageDegradedDFT, gaussianKernelDFT);
	cv::Size dftSize;
	dftSize.width = cv::getOptimalDFTSize(imageDegraded.cols + gaussianKernel.cols - 1);
	dftSize.height = cv::getOptimalDFTSize(imageDegraded.rows + gaussianKernel.rows - 1);

	cv::Mat tempImage(dftSize, imageDegraded.type(), cv::Scalar::all(0));
	cv::Mat tempGaussian(dftSize, gaussianKernel.type(), cv::Scalar::all(0));

	// copy imageDegraded and gaussianKernel to top left corner
	cv::Mat roiImage(tempImage, cv::Rect(0, 0, imageDegraded.cols, imageDegraded.rows));
	imageDegraded.copyTo(roiImage);

	cv::Mat roiGaussian(tempGaussian, cv::Rect(0, 0, gaussianKernel.cols, gaussianKernel.rows));
	// gaussianKernel.copyTo(tempGaussian);
	gaussianKernel.copyTo(roiGaussian);

	cv::dft(cv::Mat_<float>(tempImage), tempImage, 0, imageDegraded.rows);
	cv::dft(cv::Mat_<float>(tempGaussian), tempGaussian, 0, gaussianKernel.rows);

	// cv::divide(tempImage, tempGaussian, tempImage); // G / H => F
	cv::Mat division = complexMatDivision(tempImage, tempGaussian);
	cv::Mat imageOut(cv::abs(imageDegraded.rows - gaussianKernel.rows + 1),
			cv::abs(imageDegraded.cols - gaussianKernel.cols + 1), CV_32FC1);
	cv::dft(division, imageOut, cv::DFT_INVERSE);
	cv::normalize(imageOut, imageOut, 0, 255, cv::NORM_MINMAX);
	imageOut.convertTo(imageOut, CV_8UC1);
	cv::imshow("output image", imageOut);
	cv::waitKey(0);

	return EXIT_SUCCESS;
}
