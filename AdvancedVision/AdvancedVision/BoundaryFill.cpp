#include "BoundaryFill.h"
#include "MooreBoundaryTracer.h"
#include "avansvisionlib.h"

// Defines for the algorithm pixel values

#define BORDER_PIXEL 100
#define FIRST_PIXEL 9
#define CURRENT_PIXEL 4
#define LEFT_PARENT 0
#define TOP_PARENT 1
#define RIGHT_PARENT 2
#define BOTTOM_PARENT 3

const std::vector<cv::Point> neighbour_coordinates_4
{
	cv::Point{ -1, 0 },		// left
	cv::Point{ 0, -1 }, 	// top
	cv::Point{ 1, 0 },		// right
	cv::Point{ 0, 1 },		// bottom
};

BoundaryFill::BoundaryFill()
{
}


BoundaryFill::~BoundaryFill()
{
}

int BoundaryFill::getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& contourVec, std::vector<cv::Point>& regionPixels)
{
	// Create boundary image first where boundary is value 100 and the rest is -1
	cv::Mat boundaryImg = cv::Mat(image.rows, image.cols, image.type(),cv::Scalar(-1));
	const std::vector<std::vector<cv::Point>> contourVecVec = { contourVec };
	MooreBoundaryTracer::generateBoundaryImage(boundaryImg, contourVecVec, 100);

	show16SImageStretch(boundaryImg, "TestBoundaryImg"); // TODO remove 
	
	// Get first pixel

	// Get rest of regionpixels

	return 1;
}

void BoundaryFill::getFilledImage(const cv::Mat& image, cv::Mat filledImage, const std::vector<cv::Point>& regionPixels)
{
	image.copyTo(filledImage);
	// set points to a value
	for (auto const& regionPixel : regionPixels)
	{
		filledImage.at<ushort>(regionPixel) = 1;
	}
}
