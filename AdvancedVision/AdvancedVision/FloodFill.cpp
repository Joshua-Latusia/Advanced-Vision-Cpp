#include "FloodFill.h"
#include "FileIO.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>


// Defines for the algorithm pixel values

#define BORDER_PIXEL 100
#define DOUBLE_BOUNDARY_PIXEL 101
#define EMPTY_PIXEL '-1'
#define FLOODFILL_PIXEL 42
#define FIRST_PIXEL 9
#define CURRENT_PIXEL 4
#define LEFT_PARENT 0
#define TOP_PARENT 1
#define RIGHT_PARENT 2
#define BOTTOM_PARENT 3


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

FloodFill::FloodFill()
{
}


FloodFill::~FloodFill()
{
}

int FloodFill::getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels,bool animation)
{
	// Create boundary image first where boundary is value 100 and the rest is -1
	cv::Mat boundaryImg = cv::Mat(image.rows, image.cols, image.type(),cv::Scalar(EMPTY_PIXEL));
	const std::vector<std::vector<cv::Point>> contourVecVec = { boundaryVec };
	MooreBoundaryTracer::generateBoundaryImage(boundaryImg, contourVecVec, BORDER_PIXEL);

	// Generate Close boundary 
	cv::Mat closeBoundaryImg;
	std::vector<cv::Point> closeVec;
	generateDoubleBoundary(boundaryImg, closeBoundaryImg, boundaryVec, closeVec);

	fillImageEightConnected(closeBoundaryImg, boundaryVec, regionPixels, animation);
	return regionPixels.size();
}

cv::Point FloodFill::calculateFirstPixel(const cv::Mat& img, const std::vector<cv::Point>& boundaryVec)
{
const std::vector<cv::Point> bottomPoints
	{
		cv::Point{ 0, 1 },		// bottom
		cv::Point{ 1, 1 },		// bottom right
		cv::Point{ -1 ,1 } 	// bottom left
	};
	cv::Point firstBoundaryPoint = boundaryVec[0];
	bool found = false;

	while (!found)
	{
		for (int i = 0; i < bottomPoints.size(); i++)
		{
			if (img.at<ushort>(firstBoundaryPoint + bottomPoints[i]) == EMPTY_PIXEL)
			{
				cv::Point firstPoint = firstBoundaryPoint + bottomPoints[i];
				found = true;
				return firstPoint;
			}

		}
		// if the first pixel is not found move to the next pixel in the boundary
		const int nextBoundaryPoint = (std::find(boundaryVec.begin(), boundaryVec.end(), firstBoundaryPoint) - boundaryVec.begin()) +1;
		firstBoundaryPoint = boundaryVec[nextBoundaryPoint];
		
	}
	return cv::Point(-1,-1);
}


void FloodFill::generateDoubleBoundary(const cv::Mat& image, cv::Mat& doubleBoundaryImage, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& doubleBoundaryVec)
{
	image.copyTo(doubleBoundaryImage);
	
	// First point of the second boundary is the first pixel of the first boundary with y = firstboundary.y -1
	const cv::Point firstClosePixel = boundaryVec.front() + cv::Point(0,-1);
	doubleBoundaryVec.push_back(firstClosePixel);

	// Go around the boundary and construct double boundary
	// i = 1  -> Skip first pixel
	const auto vecSize = boundaryVec.size();
	for(auto i = 1; i < vecSize ;++i)
	{
		cv::Point boundaryPoint = boundaryVec[i];

		// Get direction between previous and current pixel 
		const cv::Point direction = boundaryVec[i] - boundaryVec[i - 1];

		// Previous doubleBoundaryVec item + direction => current closeBoundaryPoint item
		cv::Point closeBoundaryPoint = doubleBoundaryVec[i - 1] + direction;

		// check if the direction is going up or down.
		if (direction.x == 0 && direction.y == 1)
		{
			closeBoundaryPoint = boundaryVec[i - 1] + cv::Point(1,direction.y);
			doubleBoundaryVec.push_back(closeBoundaryPoint);
		}
		else if(direction.x == 0 && direction.y == -1)
		{
			closeBoundaryPoint = boundaryVec[i - 1] + cv::Point(-1, direction.y);
			doubleBoundaryVec.push_back(closeBoundaryPoint);
		}
		else
		{
			doubleBoundaryVec.push_back(closeBoundaryPoint);
		}
		
		// Only change pixel if its not a border
		if(image.at<ushort>(closeBoundaryPoint) != BORDER_PIXEL)
		{
			doubleBoundaryImage.at<ushort>(closeBoundaryPoint) = DOUBLE_BOUNDARY_PIXEL;
		}
	}
	doubleBoundaryImage.at<ushort>(boundaryVec.front() + cv::Point(-1, 0)) = DOUBLE_BOUNDARY_PIXEL;
	doubleBoundaryVec.push_back(boundaryVec.front() + cv::Point(-1, 0));
}

void FloodFill::fillImageEightConnected(const cv::Mat & image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels, bool animation)
{
	vector<cv::Point> pixelsToCheck;
	vector<cv::Point> lastVisitedPixels;
	//calculates the firstpixel inside of the boundary
	const cv::Point firstPixel = FloodFill::calculateFirstPixel(image, boundaryVec);
	cv::Mat filledImage = image;
	bool isNeighbourSet = false;

	//add the first pixel to the pixelsToCheck to start the algorithm
	filledImage.at<ushort>(firstPixel) = FLOODFILL_PIXEL;
	pixelsToCheck.push_back(firstPixel);
	int animationIndex = 0;

	for (;;)
	{
		//check each visited pixel for neighbours
		for (int visitedPixel = 0; visitedPixel < pixelsToCheck.size(); visitedPixel++)
		{
			for (int direction = 0; direction < neighbour_coordinates_eight_connected.size(); direction++)
			{
				// pixel to check is the pixel from the lastvisitedpixellist + one of the eight directins
				const cv::Point pixelToCheck = pixelsToCheck[visitedPixel] + neighbour_coordinates_eight_connected[direction];

				if (filledImage.at<ushort>(pixelToCheck) == EMPTY_PIXEL)
				{
					isNeighbourSet = true;
					regionPixels.push_back(pixelToCheck);
					filledImage.at<ushort>(pixelToCheck) = FLOODFILL_PIXEL;
					lastVisitedPixels.push_back(pixelToCheck);
				}
			}
		}
		if (!isNeighbourSet)
			break;

		pixelsToCheck = lastVisitedPixels;
		lastVisitedPixels.clear();
		isNeighbourSet = false;


		//used to animate the fill image.
		if (animation)
		{
			animationIndex++;
			if (animationIndex > 10)
			{
				show16SImageStretch(filledImage, "filled border");
				cv::waitKey(1) & 0XFF;
				animationIndex = 0;
			}
		}
	}

	show16SImageStretch(filledImage, "filled border");
}


bool FloodFill::saveEnclosedPixelsImages(const cv::Mat& image,
	std::vector<std::vector<cv::Point>>& enclosedPixels, const std::string& path, const std::string& filename,
	const std::string& fileExtension)
{
	const std::string dirName = path + '\\' + filename;

	// Check if there is a dir with the filename as dirname, otherwise create it
	if (!CreateDirectory(dirName.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		std::cout << "Failed to create directory: " << dirName << " Since it already exists " << std::endl;
		return false;
	}

	// Save indivual images to this dir
	for (int subPixels = 0; subPixels < enclosedPixels.size(); subPixels++)
	{
		//calculate the extremes from the contour
		FloodFillExtremesStruct extremes; 
		FloodFill::getExtremes(enclosedPixels[subPixels], extremes);

		//create a sub image with the extreme values
		cv::Mat subImage = cv::Mat(extremes.imageSize.height+1, extremes.imageSize.width+1 , image.type(), cv::Vec3b(255,255,255));
		for (int i = 0; i < enclosedPixels[subPixels].size(); i++)
		{
			//correct the position from old to new image
			cv::Point correctedPixel = enclosedPixels[subPixels][i];
			correctedPixel.x -= (extremes.min.x);
			correctedPixel.y -= (extremes.min.y);
			subImage.at<cv::Vec3b>(correctedPixel) = image.at<cv::Vec3b>(enclosedPixels[subPixels][i]);
		}
		//save the image
		const bool result = FileIO::saveImage(subImage, dirName, filename + '_' + std::to_string(subPixels), fileExtension);
		if (result)
		{
			return true;
		}
	}
	return false;
}

void FloodFill::getExtremes(std::vector<cv::Point>& enclosedPixels, FloodFillExtremesStruct& extremesStruct)
{
	for (int i = 0; i < enclosedPixels.size(); i++)
	{
		if (enclosedPixels[i].x > extremesStruct.max.x)
			extremesStruct.max.x = enclosedPixels[i].x;
		if(enclosedPixels[i].x < extremesStruct.min.x)
			extremesStruct.min.x = enclosedPixels[i].x;

		if (enclosedPixels[i].y > extremesStruct.max.y)
			extremesStruct.max.y = enclosedPixels[i].y;
		if (enclosedPixels[i].y < extremesStruct.min.y)
			extremesStruct.min.y = enclosedPixels[i].y;
	}

	extremesStruct.imageSize.width = extremesStruct.max.x - extremesStruct.min.x;
	extremesStruct.imageSize.height = extremesStruct.max.y - extremesStruct.min.y;
}


void FloodFill::getFilledImage(const cv::Mat& image, cv::Mat filledImage, const std::vector<cv::Point>& regionPixels)
{
	image.copyTo(filledImage);
	// set points to a value
	for (auto const& regionPixel : regionPixels)
	{
		filledImage.at<ushort>(regionPixel) = 1;
	}
}
