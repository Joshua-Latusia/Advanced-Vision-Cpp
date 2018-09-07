#include "MooreBoundaryTracer.h"
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "ImageLoader.h"
#include "avansvisionlib.h"

const std::vector<cv::Point> neighbour_coordinates
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

MooreBoundaryTracer::MooreBoundaryTracer()
{
}


MooreBoundaryTracer::~MooreBoundaryTracer()
{
}

void MooreBoundaryTracer::findFirstNonZeroPixel(const cv::Mat & image, cv::Point& nonZeroPixel, cv::Point& previousNeighbour)
{
	// loop through pixels
	for(int y = 0; y < image.rows; y++) // y coordinates
	{
		for(int x = 0; x < image.cols; x++) // x coordinates
		{
			// Check if image is nonzero
			if(image.at<ushort>(cv::Point(x,y)) != 0)
			{
				nonZeroPixel.x = x;
				nonZeroPixel.y = y;

				// If there is no pixel on the left go upward
				if(x == 0)
				{
					// if its the top left pixel
					if(y == 0)
					{
						throw std::invalid_argument("The topleft pixel is the first non zero pixel");
					}
					previousNeighbour.x = x;
					previousNeighbour.y = -1;

				}
				else
				{
					previousNeighbour.x = x - 1;
					previousNeighbour.y = y;
				}
				return;
			}
		}
	}

	throw std::invalid_argument("No nonzero pixel found in the image");
}

std::vector<cv::Point> MooreBoundaryTracer::getBoundaryPoints(const cv::Mat& image, cv::Point currentPixel, cv::Point backtrackPixel)
{
	std::vector<cv::Point> boundaryPoints;
	// Save start pixel
	const cv::Point startingPoint = currentPixel;
	bool backAtStart = false;

	while(!backAtStart)
	{
		boundaryPoints.push_back(currentPixel);

		// Get 3 * 3 Matrix around starting point
		cv::Mat neighbours = image(cv::Rect(currentPixel.x - 1, currentPixel.y - 1, 3, 3));

		// Get backtrack offset and index in neighbour array
		printImageToConsole(neighbours); // debug only
		cv::Point offset = backtrackPixel - currentPixel;
		int offsetIndex = getOffSetIndex(offset);

		// Search clockwise for non zero pixel unit startpoint is back
		getNextClockwiseBoundaryPoint(neighbours, currentPixel, backtrackPixel, offsetIndex);
		
		if(startingPoint == currentPixel)
		{
			backAtStart = true;
		}
	}
	// std::cout << "Boundary points: " << boundaryPoints.size() << std::endl; // DEBUG
	return boundaryPoints;
}

void MooreBoundaryTracer::getNextClockwiseBoundaryPoint(const cv::Mat neighbours, cv::Point& OUT currentPixel, cv::Point& OUT backtrackPixel, int offsetIndex)
{
	for(uint i = 0; i < neighbour_coordinates.size(); i++)
	{
		// Check for non zero pixel
		cv::Point point = cv::Point(1, 1) + neighbour_coordinates[offsetIndex];
		if(neighbours.at<ushort>(point) != 0)
		{
			backtrackPixel = currentPixel + neighbour_coordinates[offsetIndex == 0 ? neighbour_coordinates.size() - 1 : offsetIndex - 1];
			currentPixel += neighbour_coordinates[offsetIndex];
			return;
		}

		// Reset offset if its greater than list
		offsetIndex++;
		if(offsetIndex >= neighbour_coordinates.size())
		{
			offsetIndex = 0;
		}
	}
}

void MooreBoundaryTracer::generateBoundaryImage(cv::Mat& image,
	const std::vector<std::vector<cv::Point>>& contourPoints)
{
	// set points to a value
	for(auto const& blob: contourPoints)
	{
		for(auto const& point: blob)
		{
			image.at<ushort>(point) = 1;
		}
	}
}

void MooreBoundaryTracer::printImageToConsole(const cv::Mat image)
{
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			const char *c = int(image.at<ushort>(i, j)) == 0 ? "0" : "1";
			std::cout << *c ;
		}
		std::cout << " Row:" << i <<"\n";
	}
}

int MooreBoundaryTracer::getOffSetIndex(const cv::Point& offset)
{
	return std::distance(neighbour_coordinates.begin(),std::find(neighbour_coordinates.begin(), neighbour_coordinates.end(), offset));
}
