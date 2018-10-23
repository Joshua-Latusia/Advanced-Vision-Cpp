#include "ImageConverter.h"
#include <opencv2/shape/hist_cost.hpp>



ImageConverter::ImageConverter()
{
}


ImageConverter::~ImageConverter()
{
}

void ImageConverter::toGrayImage(cv::Mat& source, cv::Mat& output)
{
	cvtColor(source, output, CV_BGR2GRAY);
}

void ImageConverter::getBinaryImage(const cv::Mat& image, cv::Mat& output, double thresh, double maxval)
{
	threshold(image, output, thresh, maxval, cv::THRESH_BINARY);
}

double ImageConverter::getBendingEnergy(const std::vector<cv::Point>* contourVec)
{
	return 0.0;
}
