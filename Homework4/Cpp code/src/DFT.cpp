/** @file       DFT.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/04/2021
 *  @brief      Discrete Fourier Transform with DC in the center
 *  @details    
 *  @note       
 */
#include <iostream>
#include <opencv2/core.hpp>
#include <opencv2/highgui.hpp>

void getSinCosTable(double *sinTable, double *cosTable, int rows, int cols);
void getDFT(const uchar *image, double *realPart, double *imgPart, const double *sinTable, const double *cosTable, int rows, int cols);

int main(int argc, char **argv)
{
	if (2 != argc)
	{
		std::cerr << "usage: " << argv[0] << " image_path\n";
		exit(EXIT_FAILURE);
	}
	cv::CommandLineParser parser(argc, argv, "{@input | cat.png | input image path}");
	cv::Mat imageSrc = cv::imread(parser.get<cv::String>("@input"), cv::IMREAD_GRAYSCALE);
	if (imageSrc.empty())
	{
		std::cerr << "Error: cannot load image: " << parser.get<cv::String>("@input");
		exit(EXIT_FAILURE);
	}

	int M = imageSrc.cols, N = imageSrc.rows;




	return 0;
}


/**
 * calculate the fourier transform table given rows and columns
 * @param sinTable	double *: 2D sin table stored in 1D array
 * @param cosTable	double *: 2D cos table stored in 1D array
 * @param rows	int: number of rows
 * @param cols	int: number of columns
 */
void getSinCosTable(double *sinTable, double *cosTable, int rows, int cols)
{
	for (int x = 0; x <rows ; ++x)
	{
		for (int y = 0; y < cols; ++y)
		{
			double coeff = -2.0 * CV_PI * (1.0 * x / rows + 1.0 * y / cols);
			sinTable[x*cols + y] = sin(coeff); // sinTable[x][y] => sinTable[x*cols + y] in 1D
			cosTable[x*cols + y] = cos(coeff);
		}
	}
}


/**
 * calculate the real part and imaginary part of DFT given a 2D cv::Mat image
 * @param image		uchar *: 1D array storing 2D image
 * @param realPart	double *: real part of the DFT (2D matrix stored in 1D array)
 * @param imgPart	double *: imaginary part of the DFT (2D matrix stored in 1D array)
 * @param sinTable	double *: sin coefficients table
 * @param cosTable	double *: cos coefficients table
 * @param rows		int: number of rows
 * @param cols		int: number of columns
 */
void getDFT(const uchar *image, double *realPart, double *imgPart, const double *sinTable, const double *cosTable, int rows, int cols)
{
	for (int u = 0; u < rows; ++u)
	{
		for (int v = 0; v < cols; ++v)
		{
			double sinPart = 0, cosPart = 0;
			for (int x = 0; x < rows; ++x)
			{
				for (int y = 0; y < cols; ++y)
				{
					uchar pixelVal = *(image + x*cols + y);
					if (0 == pixelVal) { continue; }

					// because of periodicity, e.g. (x + kM) / M => x / M
					int ind1 = (u * x) % rows;
					int ind2 = (v * y) % cols;
					int ind = ind1 * cols + ind2;

					sinPart += pixelVal * sinTable[ind];
					cosPart += pixelVal * cosTable[ind];
				}
			}
			*(realPart + u*cols + v) = cosPart;
			*(imgPart + u*cols + v) = sinPart;
		}
	}
}
