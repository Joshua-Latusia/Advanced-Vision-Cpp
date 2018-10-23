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
	static void toGrayImage(cv::Mat& source, cv::Mat& output);

	/// <summary>
	/// Gets the binary image.
	/// </summary>
	/// <param name="image">The image to treshold.</param>
	/// <param name="output">The output image.</param>
	/// <param name="thresh">The thresh.</param>
	/// <param name="maxval">The maxval.</param>
	static void getBinaryImage(const cv::Mat& image, OUT cv::Mat& output, double thresh, double maxval);

	/// <summary>
	/// Gets the bending energy of a contour.
	/// </summary>
	/// <param name="contourVec">The contour vec.</param>
	/// <returns>The bending energy</returns>
	static double getBendingEnergy(const std::vector<cv::Point>* contourVec);
};

