/** @file       test.cpp
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

using namespace cv;

void convolveDFT(const cv::Mat &A, const cv::Mat &B, cv::Mat C)
{
	// reallocate the output array if needed
	C.create(abs(A.rows - B.rows) + 1, abs(A.cols - B.cols) + 1, A.type());
	Size dftSize;
	// calculate the size of DFT transform
	dftSize.width = getOptimalDFTSize(A.cols + B.cols - 1);
	dftSize.height = getOptimalDFTSize(A.rows + B.rows - 1);

	// allocate temporary buffers and initialize them with 0's
	Mat tempA(dftSize, A.type(), Scalar::all(0));
	Mat tempB(dftSize, B.type(), Scalar::all(0));

	// copy A and B to the top-left corners of tempA and tempB, respectively
	Mat roiA(tempA, Rect(0, 0, A.cols, A.rows));
	A.copyTo(roiA);
	Mat roiB(tempB, Rect(0, 0, B.cols, B.rows));
	B.copyTo(roiB);

	// now transform the padded A & B in-place;
	// use "nonzeroRows" hint for faster processing
	dft(tempA, tempA, 0, A.rows);
	dft(tempB, tempB, 0, B.rows);

	// multiply the spectrums;
	// the function handles packed spectrum representations well
	std::cout << std::endl << tempA(Rect(50, 50, 5, 5)) << std::endl;
	std::cout << std::endl << tempB(Rect(50, 50, 5, 5)) << std::endl;
	// mulSpectrums(tempA, tempB, tempA, cv::DFT_ROWS);
	divide(tempA, tempB, tempA);
	std::cout << std::endl << tempA(Rect(50, 50, 5, 5)) << std::endl;

	// transform the product back from the frequency domain.
	// Even though all the result rows will be non-zero,
	// you need only the first C.rows of them, and thus you
	// pass nonzeroRows == C.rows
	dft(tempA, tempA, DFT_INVERSE + DFT_SCALE, C.rows);

	// now copy the result back to C.
	tempA(Rect(0, 0, C.cols, C.rows)).copyTo(C);
	cv::normalize(C, C, 0, 255, cv::NORM_MINMAX);
	C.convertTo(C, CV_8UC1);

	// all the temporary buffers will be deallocated automatically
	cv::imshow("image", C);
	cv::waitKey(0);
}

cv::Mat calGaussianKernel(int rows, int cols, double sigma)
{
	CV_Assert(rows % 2 && cols % 2); // kernel size should be odd number
	int halfRow = rows / 2, halfCol = cols / 2;
	cv::Mat kernel(rows, cols, CV_32FC1, cv::Scalar::all(0));
	for (int i = -halfRow; i <= halfRow; ++i)
	{
		auto *pPixel = kernel.ptr<float>(i + halfRow); // +halfRow 从中心开始计算
		for (int j = -halfCol; j <= halfCol; ++j)
		{
			*(pPixel + j + halfCol) = cv::exp(-static_cast<float>(0.5 * (i * i + j * j) / sigma / sigma));
		}
	}

	double sum = cv::sum(kernel)(0);
	kernel /= sum; // normalize the kernel to make it sum as 1

	return kernel;
}

int main()
{
	cv::Mat imageSrc = cv::imread("../images/cat.png", cv::IMREAD_GRAYSCALE);
	cv::Mat gaussian = calGaussianKernel(31, 31, 7);
	cv::Mat imageDegraded;
	cv::filter2D(cv::Mat_<float>(imageSrc), imageDegraded, CV_8U, gaussian, cv::Point(-1, -1), 0, cv::BORDER_CONSTANT);

	cv::Mat imageOut;
	convolveDFT(cv::Mat_<float>(imageDegraded), cv::Mat_<float>(gaussian), imageOut);

	return 0;
}
