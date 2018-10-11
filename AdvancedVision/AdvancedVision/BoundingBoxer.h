#pragma once
#include <vector>
#include <opencv2/core/mat.hpp>

/// <summary>
/// Struct to determine the extreme x and y values of a boundinbox
/// </summary>
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
	/// <param name="boundingBoxStructs">The bounding box extremes</param>
	/// <returns></returns>
	static int getBoundingBoxes(const	std::vector<std::vector<cv::Point>>& contours,
								std::vector<std::vector<cv::Point>>& bbs,
								std::vector<boundingBoxStruct>& boundingBoxStructs);
	
	/// <summary>
	/// Generates the bounding box with help of the x,y extremes of a contour.
	/// </summary>
	/// <param name="extremes">The extremes.</param>
	/// <returns>All points in a bounding box from top left clockwise</returns>
	static std::vector<cv::Point> generateBoundingBox(boundingBoxStruct& extremes);

	/// <summary>
	/// Draws the bounding boxes in the image.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="outputImage">The output image.</param>
	/// <param name="bbs">The BBS.</param>
	static void drawBoundingBoxes(const cv::Mat& image, cv::Mat& outputImage, std::vector<std::vector<cv::Point>>& bbs);
	
	/// <summary>
	/// Saves all the bounding box sub images to seperate files seprated by e.g. "_1"
	/// </summary>
	/// <param name="image">The image on where to get the sub images from.</param>
	/// <param name="boundingBoxStructs">The bounding box structs extremes.</param>
	/// <returns>True if all saves succeed otherwise false</returns>
	static bool saveBoundingBoxImages(const cv::Mat& image, const std::vector<boundingBoxStruct>& boundingBoxStructs,
		const std::string& path, const std::string& filename, const std::string
		& fileExtension = ".jpg");
	
	/// <summary>
	/// Calculates the length width ratio.
	/// By dividing length by width.
	/// ratio of 1 means both sides are the same.
	/// </summary>
	/// <param name="extremes">The extremes of the bounding box.</param>
	/// <returns>value giving the ratio between smallest divided by biggest </returns>
	static double calculateLengthWidthRatio(boundingBoxStruct& extremes);
	
	/// <summary>
	/// Calculates the length width ratio.
	/// By dividing smallest side by longest side.
	/// ratio of 1 means both sides are the same.
	/// </summary>
	/// <param name="length">The length.</param>
	/// <param name="width">The width.</param>
	/// <returns>value giving the ratio between smallest divided by biggest</returns>
	static double calculateLengthWidthRatio(int length, int width);

private:
	/// <summary>
	/// Gets the boundary extremes.
	/// </summary>
	/// <param name="boundingBox">The bounding box.</param>
	/// <returns>The most extreme outer points in order: Top, Right, Down, Left</returns>
	static boundingBoxStruct getContourExtremes(std::vector<cv::Point>& contour);
};

