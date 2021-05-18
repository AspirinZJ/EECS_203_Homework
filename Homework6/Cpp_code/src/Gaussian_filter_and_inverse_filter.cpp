/** @file       Gaussian_filter_and_inverse_filter.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/18/2021
 *  @brief      Applay Gaussian filter and do the inverse filtering
 *  @details    Apply a 31x31 Gaussian filter to the input image, then use the inverse filtering method
 *  			to restore the image
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

inline cv::Vec2f complexDivision(const cv::Vec2f &num1, const cv::Vec2f &num2);
cv::Mat getGaussianKernel(int rows, int cols, double sigma);
cv::Mat discreteFourierTrans(const cv::Mat &mat);
cv::Mat getDFTMagnitudeCentered(const cv::Mat &dftMat);
cv::Mat complexImagesDivision(const cv::Mat &image1, const cv::Mat &image2);

int main(int argc, char **argv)
{
	cv::String keys = "{@input | ../images/triangle.png | input image path}";
	cv::CommandLineParser parser(argc, argv, keys);
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}

	cv::Mat gaussianKernel = getGaussianKernel(31, 31, 7);
	// std::cout << "Gaussian kernel: \n" << gaussianKernel << std::endl;

	cv::Mat imageDegraded;
	cv::filter2D(imageSrc, imageDegraded, imageSrc.depth(), gaussianKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
	// cv::GaussianBlur(imageSrc, imageDegraded, cv::Size(31, 31), 7, 7, cv::BORDER_REPLICATE);

	cv::Mat ftDegraded = discreteFourierTrans(imageDegraded);
	cv::Mat ftGaussian = discreteFourierTrans(gaussianKernel);
	// TODO: align the size of two matrices

	//	cv::Mat ftImage = complexImagesDivision(ftDegraded, ftGaussian);
	//
	//	cv::Mat imageRestored;
	//	cv::dft(ftImage, imageRestored, cv::DFT_INVERSE, cv::DFT_REAL_OUTPUT);
	//
	cv::Mat planes[2];
	cv::split(ftDegraded, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat mag = planes[0];
	mag += cv::Scalar::all(1);
	cv::log(mag, mag);
	cv::normalize(mag, mag, 0, 1, CV_MINMAX);
	cv::imshow("magnitude", mag);
	// cv::imshow("degraded image", imageDegraded);
	cv::waitKey(0);

	return 0;
}

/**
 * complex number division
 * @param num1 cv::Vec2f: input complex number 1, 1st element real part, 2nd element imaginary part
 * @param num2 cv::Vec2f: input complex number 1, 1st element real part, 2nd element imaginary part
 * @return	cv::Vec2f: input complex number 1, 1st element real part, 2nd element imaginary part
 */
inline cv::Vec2f complexDivision(const cv::Vec2f &num1, const cv::Vec2f &num2)
{
	float a = num1(0), b = num1(1), c = num2(0), d = num2(1);
	float realPart = (a * c + b * d) / (c * c + d * d);
	float imgPart = (b * c - a * d) / (c * c + d * d);
	return cv::Vec2f(realPart, imgPart);
}

/**
 * Generate Gaussian kernel
 * @details		kernel(x, y) = A * exp(-(x^2 + y^2) / (2*sigma^2))
 * @param rows	int: number of rows
 * @param cols	int: number of columns
 * @return		cv::Mat: Gaussian kernel
 */
cv::Mat getGaussianKernel(int rows, int cols, double sigma)
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

/**
 * Do the discrete Fourier transform
 * @param mat 		const cv::Mat &: input matrix
 * @param isPadding bool: choose to pad or not, true by default
 * @return 			cv::Mat: output matrix, complex
 */
cv::Mat discreteFourierTrans(const cv::Mat &mat)
{
	cv::Mat padded; // expand input matrix to optimal size
	int m = cv::getOptimalDFTSize(mat.rows), n = cv::getOptimalDFTSize(mat.cols);
	cv::copyMakeBorder(mat, padded, 0, m - mat.rows, 0, n - mat.cols, cv::BORDER_CONSTANT, cv::Scalar::all(0));
	cv::Mat planes[2] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32FC1)}; // imaginary part is all 0
	cv::Mat complex;
	cv::merge(planes, 2, complex); // merge to a 2 channel complex mat, channel 0 real part, channel 1 img part
	cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);
	return complex;
}

/**
 * Get the magnitude of the Fourier Transform signal, image center as origin
 * @param dftMat const cv::Mat &: input matrix, 2 channel, first channel real part, second channel imaginary part
 * @return cv::Mat: magnitude, image center as origin
 */
cv::Mat getDFTMagnitudeCentered(const cv::Mat &dftMat)
{

}

/**
 * Divide two complex OpenCV images
 * @param image1 const cv::Mat &: numerator, complex image, two channels, channel 1 real part, channel 2 imaginary part
 * @param image2 const cv::Mat &: denominator, complex image, two channels, channel 1 real part, channel 2 imaginary part
 * @return cv::Mat: output image, two channels, channel 1 real part, channel 2 imaginary part
 */
cv::Mat complexImagesDivision(const cv::Mat &image1, const cv::Mat &image2)
{
	CV_Assert(image1.size() == image2.size());

	cv::Mat imageOut(image1.size(), CV_32FC2);
	for (int row = 0; row < image1.rows; ++row)
	{
		// const cv::Vec2f *pPixel1 = image1.ptr<cv::Vec2f>(row);
		const auto *pPixel1 = image1.ptr<cv::Vec2f>(row);
		const auto *pPixel2 = image2.ptr<cv::Vec2f>(row);
		auto *pPixelOut = imageOut.ptr<cv::Vec2f>(row);
		for (int col = 0; col < image1.cols; ++col)
		{
			*(pPixelOut + col) = complexDivision(*(pPixel1 + col), *(pPixel2 + col));
		}
	}
	return imageOut;
}
