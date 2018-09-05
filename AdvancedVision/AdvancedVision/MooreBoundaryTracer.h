#pragma once
#include <opencv2/core/mat.hpp>

class MooreBoundaryTracer
{
public:
	MooreBoundaryTracer();
	~MooreBoundaryTracer();
	
	/// <summary>
	/// Finds the first non zero pixel starting from the left top.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="nonZeroPixel">The non zero pixel.</param>
	/// <param name="previousNeighbour">The previous neighbour.</param>
	static void FindFirstNonZeroPixel(const cv::Mat& image, cv::Point& nonZeroPixel, cv::Point& previousNeighbour);
	
	/// <summary>
	/// Prints the image to console.
	/// </summary>
	/// <param name="image">The image.</param>
	static void PrintImageToConsole(const cv::Mat image);
};

