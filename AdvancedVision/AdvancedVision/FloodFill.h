#pragma once
#include <vector>
#include "avansvisionlib.h"
#include <thread>         // std::this_thread::sleep_for
#include <chrono>  


struct FloodFillExtremesStruct
{
	cv::Point min = cv::Point(INT_MAX, INT_MAX);
	cv::Point max = cv::Point(0, 0);
	cv::Rect imageSize;
};

class FloodFill
{
public:
	FloodFill();
	~FloodFill();

	/// <summary>
	/// Gets the pixels inside of a single contour.
	/// </summary>
	/// <param name="image">The original image for the img dimensions.</param>
	/// <param name="contourVec">The contour vec.</param>
	/// <param name="regionPixels">The region pixels.</param>
	/// <returns></returns>
	static int getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels, bool animation = false);

	/// <summary>
	/// Calculates the first pixel.
	/// </summary>
	/// <param name="img">The img.</param>
	/// <param name="boundaryVec">The boundary vec.</param>
	/// <returns></returns>
	static cv::Point calculateFirstPixel(const cv::Mat & img, const std::vector<cv::Point>& boundaryVec);
	
	/// <summary>
	/// Fills the image four connected.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="firstPixel">The first pixel.</param>
	static void fillImageEightConnected(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels, bool animation = false);


	static bool saveEnclosedPixelsImages(const cv::Mat & image, std::vector<std::vector<cv::Point>>& enclosedPixels, const std::string & path, 
		const std::string & filename, const std::string & fileExtension = ".jpg");


	static void getExtremes(std::vector<cv::Point>& enclosedPixels, FloodFillExtremesStruct & extremesStruct);

	/// <summary>
	/// Generates the extra boundary around the regular boundary so FloodFill algorithm will work 8-connected.
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

