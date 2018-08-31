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
