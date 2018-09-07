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
	static void findFirstNonZeroPixel(const cv::Mat& image, cv::Point& nonZeroPixel, cv::Point& previousNeighbour);
	
	/// <summary>
	/// Gets the boundary points of an image using the Moore boundary algorithm.
	/// https://www.codeproject.com/Articles/1105045/Tracing-Boundary-in-D-Image-Using-Moore-Neighborho
	/// </summary>
	/// <param name="image">The image.</param>
	static std::vector<cv::Point> getBoundaryPoints(const cv::Mat& image, cv::Point currentPixel, cv::Point backtrackPixel);
		
	/// <summary>
	/// Gets the boundary point by rotating clockwise over the 3x3 matrix.
	/// </summary>
	/// <param name="currentPixel">The current pixel.</param>
	/// <param name="offsetIndex">Index of the offset.</param>
	static void getNextClockwiseBoundaryPoint(cv::Mat neighbours,cv::Point& currentPixel, cv::Point& backtrackPixel, int offsetIndex);
	
	/// <summary>
	/// Generates the boundary image.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="contourPoints">The contour points.</param>
	static void generateBoundaryImage(cv::Mat& image, const std::vector<std::vector<cv::Point>>& contourPoints);

	/// <summary>
	/// Prints the image to console.
	/// </summary>
	/// <param name="image">The image.</param>
	static void printImageToConsole(const cv::Mat image);

private:
	
	/// <summary>
	/// Gets the index of the offset from the neighbours vector.
	/// </summary>
	/// <param name="offset">The offset.</param>
	/// <returns></returns>
	static int getOffSetIndex(const cv::Point& offset);
};

