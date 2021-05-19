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
cv::Mat calGaussianKernel(int rows, int cols, double sigma);
cv::Mat discreteFourierTrans(const cv::Mat &mat);
cv::Mat getDFTMagnitudeCentered(const cv::Mat &dftMat);
void displayMagnitude(cv::Mat &magnitudeMat, const char *winTitle);
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

	cv::Mat gaussianKernel = calGaussianKernel(31, 31, 7);
	cv::Mat imageDegraded;
	cv::filter2D(imageSrc, imageDegraded, imageSrc.depth(), gaussianKernel, cv::Point(-1, -1), 0, cv::BORDER_REPLICATE);
	// cv::GaussianBlur(imageSrc, imageDegraded, cv::Size(31, 31), 7, 7, cv::BORDER_REPLICATE);
	// TODO: align the size of two matrices
	cv::Size dftSize;
	// calculate the size of DFT transform
	dftSize.width = cv::getOptimalDFTSize(imageDegraded.cols + gaussianKernel.cols - 1);
	dftSize.height = cv::getOptimalDFTSize(imageDegraded.rows + gaussianKernel.rows - 1);

	cv::Mat tempImage(dftSize, imageDegraded.type(), cv::Scalar::all(0));
	cv::Mat tempGaussian(dftSize, gaussianKernel.type(), cv::Scalar::all(0));

	// copy imageDegraded and gaussianKernel to the top-left corners
	cv::Mat roiImage(tempImage, cv::Rect(0, 0, imageDegraded.cols, imageDegraded.rows));
	imageDegraded.copyTo(roiImage);
	cv::Mat roiGaussian(tempGaussian, cv::Rect(0, 0, gaussianKernel.cols, gaussianKernel.rows));
	gaussianKernel.copyTo(roiGaussian);

	// transform the padded imageDegraded and gaussianKernel in-place
	cv::dft(cv::Mat_<float>(tempImage), tempImage, 0, imageDegraded.rows);
	cv::dft(cv::Mat_<float>(tempGaussian), tempGaussian, 0, gaussianKernel.rows);

	tempGaussian.inv();

	// todo 4th parameter
	cv::mulSpectrums(tempImage, tempGaussian, tempImage, cv::DFT_ROWS);

	cv::Mat imageOut;
	imageOut.create(cv::abs(imageDegraded.rows - gaussianKernel.rows) + 1,
			cv::abs(imageDegraded.cols - gaussianKernel.cols) + 1, imageDegraded.type());
	cv::dft(tempImage, tempImage, cv::DFT_INVERSE | cv::DFT_SCALE, imageOut.rows);

	tempImage(cv::Rect(0, 0, imageOut.cols, imageOut.rows)).copyTo(imageOut);

	cv::normalize(imageOut, imageOut, 0, 255, cv::NORM_MINMAX);
	imageOut.convertTo(imageOut, CV_8UC1);
	cv::imshow("output image", imageOut);


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
	cv::Mat planes[2];
	cv::split(dftMat, planes); // split the 2-channel image to an array of length 2
	cv::Mat magnitudeMat;
	cv::magnitude(planes[0], planes[1], magnitudeMat); // compute the magnitude

	return magnitudeMat;
}

/**
 * Display the magnitude of a Fourier transformed mat
 * @param magnitudeMat cv::Mat &: magnitude
 * @param winTitle const char *: window title
 */
void displayMagnitude(cv::Mat &magnitudeMat, const char *winTitle)
{
	magnitudeMat += cv::Scalar::all(1); // log(magnitude + 1) => to make the magnitude more clear to show
	cv::log(magnitudeMat, magnitudeMat);

	// crop the image if it has odd number of rows of columns
	// -2 in memory => 11111110, num & -2 => set the lease significant bit as 0 => make num even
	magnitudeMat = magnitudeMat(cv::Rect(0, 0, magnitudeMat.cols & -2, magnitudeMat.rows & -2));

	int cx = magnitudeMat.cols / 2, cy = magnitudeMat.rows / 2;
	cv::Mat topLeft = magnitudeMat(cv::Rect(0, 0, cx, cy));
	cv::Mat topRight = magnitudeMat(cv::Rect(cx, 0, cx, cy));
	cv::Mat bottomLeft = magnitudeMat(cv::Rect(0, cy, cx, cy));
	cv::Mat bottomRight = magnitudeMat(cv::Rect(cx, cy, cx, cy));

	// swap the top left with bottom right, top right with bottom left
	cv::Mat tmp;
	topLeft.copyTo(tmp);
	bottomRight.copyTo(topLeft);
	tmp.copyTo(bottomRight);
	topRight.copyTo(tmp);
	bottomLeft.copyTo(topRight);
	tmp.copyTo(bottomLeft);

	cv::normalize(magnitudeMat, magnitudeMat, 0, 255, cv::NORM_MINMAX);
	magnitudeMat.convertTo(magnitudeMat, CV_8UC1);
	cv::imshow(winTitle, magnitudeMat);
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
