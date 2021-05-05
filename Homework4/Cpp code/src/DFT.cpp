/** @file       DFT.cpp
 *  @author     Jack Zhang
 *  @version    1.0
 *  @date       05/04/2021
 *  @brief      Discrete Fourier Transform with DC in the center
 *  @details    
 *  @note       
 */
#include <iostream>
#include <memory>
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

	int M = imageSrc.rows, N = imageSrc.cols;
	auto *sinTable = new double[M * N];
	auto *cosTable = new double[M * N];

	// calculate the sin and cos coefficients table
	getSinCosTable(sinTable, cosTable, M, N);

	auto *realPart = new double[M * N];
	auto *imgPart = new double[M * N];

	auto *imageArr = new uchar[M * N];

	for (int row = 0; row < imageSrc.rows; ++row)
	{
		for (int col = 0; col < imageSrc.cols; ++col)
		{
			*(imageArr + row * imageSrc.cols + col) = imageSrc.ptr<uchar>(row)[col];
		}
	}

	getDFT(imageArr, realPart, imgPart, sinTable, cosTable, M, N);

	cv::Mat magnitude(M, N, CV_32F);

	for (int row = 0; row < M; ++row)
	{
		auto *pixelPtr = magnitude.ptr<float>(row);
		for (int col = 0; col < N; ++col)
		{
			pixelPtr[col] = sqrt(realPart[row*N + col] * realPart[row*N + col] + imgPart[row*N + col] * imgPart[row * N + col]);
		}
	}

	magnitude.convertTo(magnitude, CV_8U);
	cv::imshow("magnitude", magnitude);
	cv::waitKey(0);



	delete[] sinTable;
	delete[] cosTable;
	delete[] realPart;
	delete[] imgPart;
	delete[] imageArr;
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
		std::cout << "Finished row: " << u << std::endl;
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
			// TODO: whether to divide with MN
			*(realPart + u*cols + v) = cosPart;
			*(imgPart + u*cols + v) = sinPart;
		}
	}
}
