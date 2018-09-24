#pragma once
#include <vector>
#include <opencv2/core/mat.hpp>

class BoundaryFill
{
public:
	BoundaryFill();
	~BoundaryFill();
	
	/// <summary>
	/// Gets the pixels inside of a single contour.
	/// </summary>
	/// <param name="image">The original image for the img dimensions.</param>
	/// <param name="contourVec">The contour vec.</param>
	/// <param name="regionPixels">The region pixels.</param>
	/// <returns></returns>
	static int getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& contourVec, std::vector<cv::Point>& regionPixels);
	
	/// <summary>
	/// Gets the filled image by filling up the contour image
	/// </summary>
	/// <param name="image">The contour image.</param>
	/// <param name="filledImage">The image output.</param>
	/// <param name="regionPixels">The region pixels inside of the contour.</param>
	static void getFilledImage(const cv::Mat& image, cv::Mat filledImage, const std::vector<cv::Point>& regionPixels);

};

