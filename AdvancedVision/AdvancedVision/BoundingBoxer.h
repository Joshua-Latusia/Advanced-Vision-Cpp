#pragma once
#include <vector>
#include <opencv2/core/mat.hpp>

struct boundingBoxStruct
{
	cv::Point min = cv::Point(INT_MAX, INT_MAX);
	cv::Point max = cv::Point(0,0);
};

class BoundingBoxer
{
public:
	BoundingBoxer();
	~BoundingBoxer();
	
	/// <summary>
	/// Gets the bounding boxes of contours.
	/// </summary>
	/// <param name="contours">The contours.</param>
	/// <param name="bbs">The bounding boxes.</param>
	/// <returns></returns>
	int getBoundingBoxes(const	std::vector<std::vector<cv::Point>>& contours,
								std::vector<std::vector<cv::Point>>& bbs);
	
	/// <summary>
	/// Generates the bounding box with help of the x,y extremes of a contour.
	/// </summary>
	/// <param name="extremes">The extremes.</param>
	/// <returns>All points in a bounding box from top left clockwise</returns>
	std::vector<cv::Point> generateBoundingBox(boundingBoxStruct& extremes);

	/// <summary>
	/// Gets the boundary extremes.
	/// </summary>
	/// <param name="boundingBox">The bounding box.</param>
	/// <returns>The most extreme outer points in order: Top, Right, Down, Left</returns>
	boundingBoxStruct getContourExtremes(std::vector<cv::Point>& boundingBox);

	/// <summary>
	/// Draws the bounding boxes in the image.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="outputImage">The output image.</param>
	/// <param name="bbs">The BBS.</param>
	void drawBoundingBoxes(const cv::Mat& image, cv::Mat& outputImage, std::vector<std::vector<cv::Point>>& bbs);
};

