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
	static int getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels);
	
	/// <summary>
	/// Generates the extra boundary around the regular boundary so BoundaryFill algorithm will work 8-connected.
	/// </summary>
	/// <param name="image">The boundary image image with the boundaries having a value &gt; 0.</param>
	/// <param name="closeBoundaryImage">The boundary image with extra border with the value of 101.</param>
	/// <param name="boundaryVec">The boundary vector of the image. In the original order of the boundary path</param>
	/// <param name="doubleBoundaryVec">The double boundary vector, also contains the points that should be a double border but already are a single border.</param>
	static void generateDoubleBoundary(const cv::Mat& image, cv::Mat& doubleBoundaryImage, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& doubleBoundaryVec);

	/// <summary>
	/// Gets the filled image by filling up the contour image
	/// </summary>
	/// <param name="image">The contour image.</param>
	/// <param name="filledImage">The image output.</param>
	/// <param name="regionPixels">The region pixels inside of the contour.</param>
	static void getFilledImage(const cv::Mat& image, cv::Mat filledImage, const std::vector<cv::Point>& regionPixels);

};

