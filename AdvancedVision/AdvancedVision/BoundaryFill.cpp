#include "BoundaryFill.h"
#include "MooreBoundaryTracer.h"
#include "avansvisionlib.h"

// Defines for the algorithm pixel values

#define BORDER_PIXEL 100
#define DOUBLE_BOUNDARY_PIXEL 101
#define EMPTY_PIXEL -1
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

int BoundaryFill::getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels)
{
	// Create boundary image first where boundary is value 100 and the rest is -1
	cv::Mat boundaryImg = cv::Mat(image.rows, image.cols, image.type(),cv::Scalar(0));
	const std::vector<std::vector<cv::Point>> contourVecVec = { boundaryVec };
	MooreBoundaryTracer::generateBoundaryImage(boundaryImg, contourVecVec, 100);

	show16SImageStretch(boundaryImg, "TestBoundaryImg"); // TODO remove 

	// Generate Close boundary 
	cv::Mat closeBoundaryImg;
	std::vector<cv::Point> closeVec;
	generateDoubleBoundary(boundaryImg, closeBoundaryImg, boundaryVec, closeVec);
	show16SImageStretch(closeBoundaryImg, "Close Boundary"); // TODO remove 

	MooreBoundaryTracer::printImageToConsole(boundaryImg(cv::Rect(420, 20, 100, 50)));
	std::cout << std::endl << std::endl;
	MooreBoundaryTracer::printImageToConsole(closeBoundaryImg(cv::Rect(420, 20, 100, 50)));

	// Get first pixel

	// Get rest of regionpixels

	return 1;
}


void BoundaryFill::generateDoubleBoundary(const cv::Mat& image, cv::Mat& doubleBoundaryImage, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& doubleBoundaryVec)
{
	image.copyTo(doubleBoundaryImage);
	
	// First point of the second boundary is the first pixel of the first boundary with y = firstboundary.y -1
	const Point firstClosePixel = boundaryVec.front() + cv::Point(0,-1);
	doubleBoundaryVec.push_back(firstClosePixel);

	// Go around the boundary and construct double boundary
	// i = 1  -> Skip first pixel
	const auto vecSize = boundaryVec.size();
	for(auto i = 1; i < vecSize ;++i)
	{
		cv::Point boundaryPoint = boundaryVec[i];

		// Get direction between previous and current pixel 
		const cv::Point direction = boundaryVec[i] - boundaryVec[i - 1];

		// Previous doubleBoundaryVec item + direction => current doubleBoundaryVec item
		const cv::Point closeBoundaryPoint = doubleBoundaryVec[i - 1] + direction;
		doubleBoundaryVec.push_back(closeBoundaryPoint);

		// Only change pixel if its not a border
		if(image.at<ushort>(closeBoundaryPoint) != BORDER_PIXEL)
		{
			doubleBoundaryImage.at<ushort>(closeBoundaryPoint) = DOUBLE_BOUNDARY_PIXEL;
		}
	}
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
