#include "BoundaryFill.h"
#include "MooreBoundaryTracer.h"
#include "avansvisionlib.h"

// Defines for the algorithm pixel values

#define BORDER_PIXEL 100
#define DOUBLE_BOUNDARY_PIXEL 101
#define EMPTY_PIXEL '-1'
#define FIRST_PIXEL 9
#define CURRENT_PIXEL 4
#define LEFT_PARENT 0
#define TOP_PARENT 1
#define RIGHT_PARENT 2
#define BOTTOM_PARENT 3


const std::vector<cv::Point> neighbour_coordinates_four_connected
{
	Point{ -1, 0 },		// left
	Point{ 0, -1 }, 	// top
	Point{ 1, 0 },		// right
	Point{ 0, 1 },		// bottom
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
	cv::Mat boundaryImg = cv::Mat(image.rows, image.cols, image.type(),cv::Scalar(EMPTY_PIXEL));
	const std::vector<std::vector<cv::Point>> contourVecVec = { boundaryVec };
	MooreBoundaryTracer::generateBoundaryImage(boundaryImg, contourVecVec, BORDER_PIXEL);

	show16SImageStretch(boundaryImg, "TestBoundaryImg"); // TODO remove 

	// Generate Close boundary 
	cv::Mat closeBoundaryImg;
	std::vector<cv::Point> closeVec;
	generateDoubleBoundary(boundaryImg, closeBoundaryImg, boundaryVec, closeVec);
	show16SImageStretch(closeBoundaryImg, "Close Boundary"); // TODO remove 

	//MooreBoundaryTracer::printImageToConsole(boundaryImg(cv::Rect(420, 20, 100, 50)));
	std::cout << std::endl << std::endl;
	//MooreBoundaryTracer::printImageToConsole(closeBoundaryImg(cv::Rect(420, 20, 100, 50)));

	// Get first pixel

	BoundaryFill::fillImageFourConnected(boundaryImg, Point(41, 53));


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

void BoundaryFill::fillImageFourConnected(const cv::Mat & image, const cv::Point firstPixel)
{
	vector<Point> pixelsToCheck;
	vector<Point> lastVisitedPixels;

	int stepIndex = 1;
	cv::Mat filledImage = image;

	bool isNeighbourSet = false;

	filledImage.at<ushort>(firstPixel) = 0;
	pixelsToCheck.push_back(firstPixel);

	for (;;)
	{
		for (int visitedPixel = 0; visitedPixel < pixelsToCheck.size(); visitedPixel++)
		{
			for (int direction = 0; direction < neighbour_coordinates_four_connected.size(); direction++)
			{
				Point pixelToCheck = pixelsToCheck[visitedPixel] + neighbour_coordinates_four_connected[direction];

				ushort kek = filledImage.at<ushort>(pixelToCheck);
				if (filledImage.at<ushort>(pixelToCheck) == EMPTY_PIXEL)
				{
					isNeighbourSet = true;
					filledImage.at<ushort>(pixelToCheck) = stepIndex;
					lastVisitedPixels.push_back(pixelToCheck);
				}
			}
		}
		if (!isNeighbourSet)
			break;

		stepIndex++;
		pixelsToCheck = lastVisitedPixels;
		lastVisitedPixels.clear();
		isNeighbourSet = false;
	}

	BoundaryFill::cleanFilledImage(filledImage);

	show16SImageStretch(filledImage, "filled border");
}



void BoundaryFill::cleanFilledImage(cv::Mat & img)
{
	for (int y = 0; y < img.cols; y++)
	{
		for (int x = 0; x < img.rows; x++)
		{
			if (img.at<ushort>(Point(x, y)) == EMPTY_PIXEL)
				img.at<ushort>(Point(x, y)) = 0;
			else if (img.at<ushort>(Point(x, y)) == BORDER_PIXEL)
				img.at<ushort>(Point(x, y)) = 1;
			else 
				img.at<ushort>(Point(x, y)) = 1;
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
