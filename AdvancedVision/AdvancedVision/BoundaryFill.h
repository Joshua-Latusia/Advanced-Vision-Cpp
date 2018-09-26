#pragma once
#include <vector>
#include <opencv2/core/mat.hpp>



class BoundaryFill
{
public:

	const std::vector<cv::Point> neighbour_coordinates_eight_connected
	{
		cv::Point{ -1, 0 },		// left
		cv::Point{ -1, -1 }, 	// top left
		cv::Point{ 0, -1 }, 	// top
		cv::Point{ 1, -1 }, 	// top right
		cv::Point{ 1, 0 },		// right
		cv::Point{ 1, 1 },		// bottom right
		cv::Point{ 0, 1 },		// bottom
		cv::Point{ -1 ,1 }, 	// bottom left
	};




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
	/// Fills the image four connected.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="firstPixel">The first pixel.</param>
	static void fillImageFourConnected(const cv::Mat& image, const cv::Point firstPixel);

	/// <summary>
	/// Cleans the filled image.
	/// </summary>
	/// <param name="img">The img.</param>
	static void cleanFilledImage(cv::Mat & img);

	/// <summary>
	/// Gets the filled image by filling up the contour image
	/// </summary>
	/// <param name="image">The contour image.</param>
	/// <param name="filledImage">The image output.</param>
	/// <param name="regionPixels">The region pixels inside of the contour.</param>
	static void getFilledImage(const cv::Mat& image, cv::Mat filledImage, const std::vector<cv::Point>& regionPixels);

};

