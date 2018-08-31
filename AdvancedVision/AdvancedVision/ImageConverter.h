#pragma once
#define OUT
#include <opencv2/core/mat.hpp>

class ImageConverter
{
public:
	ImageConverter();
	~ImageConverter();
	
	/// <summary>
	/// Converts image To the gray image.
	/// </summary>
	/// <param name="source">The source.</param>
	/// <param name="output">The output.</param>
	static void ToGrayImage(cv::Mat& source, cv::Mat& output);

};

