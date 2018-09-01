#include "ImageConverter.h"
#include <opencv2/shape/hist_cost.hpp>



ImageConverter::ImageConverter()
{
}


ImageConverter::~ImageConverter()
{
}

void ImageConverter::ToGrayImage(cv::Mat& source, cv::Mat& output)
{
	cv::cvtColor(source, output, CV_BGR2GRAY);
}

// https://www.codeproject.com/Articles/1105045/Tracing-Boundary-in-D-Image-Using-Moore-Neighborho
// https://github.com/TarasMartynyuk/Boundary-Tracing-Moore-Neighbours
int ImageConverter::GetContours(const cv::Mat& image, OUT std::vector<std::vector<cv::Point>>& contourVecVec)
{
	cv::Point b0;
	cv::Point c0;

	// Call Moore boundary file

	// Get upermost left boundary
	
	// Set west neighbour of this left upermost 

	// looping through the rest

	return 1;
}

void ImageConverter::GetBinaryImage(const cv::Mat& image, cv::Mat& output, double thresh, double maxval)
{
	cv::threshold(image, output, thresh, maxval, cv::THRESH_BINARY);
}

double ImageConverter::GetBendingEnergy(const std::vector<cv::Point>* contourVec)
{
	return 0.0;
}
