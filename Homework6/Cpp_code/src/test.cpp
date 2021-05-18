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

cv::Mat getDFT(const cv::Mat &mat);

int main()
{
	cv::Mat imageSrc = cv::imread("../images/triangle.png", cv::IMREAD_GRAYSCALE);
	cv::imshow("input image", imageSrc);
	cv::Mat complex = getDFT(imageSrc);
	cv::Mat planes[2];
	cv::split(complex, planes);
	cv::magnitude(planes[0], planes[1], planes[0]);
	cv::Mat mag = planes[0];
	mag += cv::Scalar::all(1);
	cv::log(mag, mag);
	cv::normalize(mag, mag, 0, 1, CV_MINMAX);
	cv::imshow("magnitude", mag);

//	cv::Mat inverseTmp, inverse;
//	//cv::dft(complex, inverse, cv::DFT_INVERSE | cv::DFT_REAL_OUTPUT);
//	cv::idft(complex, inverse, cv::DFT_REAL_OUTPUT);
//	double minVal, maxVal;
//	cv::minMaxLoc(inverse, &minVal, &maxVal);
//	std::cout << minVal << " " << maxVal << std::endl;
//	cv::normalize(inverse, inverse, 0, 255, CV_MINMAX);
//	inverse.convertTo(inverse, CV_8UC1);
//	cv::imshow("output image", inverse);
	cv::waitKey(0);

	return 0;
}
cv::Mat getDFT(const cv::Mat &mat)
{
	cv::Mat padded;
	int m = cv::getOptimalDFTSize(mat.rows), n = cv::getOptimalDFTSize(mat.cols);
	cv::copyMakeBorder(mat, padded, 0, m - mat.rows, 0, n - mat.cols, cv::BORDER_CONSTANT,
			cv::Scalar::all(0));
	cv::Mat planes[2] = {cv::Mat_<float>(padded), cv::Mat::zeros(padded.size(), CV_32F)};
	cv::Mat complex;
	cv::merge(planes, 2, complex);
	cv::dft(complex, complex, cv::DFT_COMPLEX_OUTPUT);
	return complex;
}
