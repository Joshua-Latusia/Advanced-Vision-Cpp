#include "MooreBoundaryTracer.h"
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "ImageLoader.h"
#include "avansvisionlib20.h"

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

void MooreBoundaryTracer::getBackTrackPixel(const cv::Mat & image, cv::Point& currentPixel, cv::Point& backtrackPixel)
{
	// if the currentPixel is invalid
	if (image.at<ushort>(currentPixel) == 0)
	{
		throw std::invalid_argument("Currentpixel should not be zero");
	}

	// If there is no pixel on the left go upward
	if (currentPixel.x == 0)
	{
		// if its the top left pixel
		if (currentPixel.y == 0)
		{
			throw std::invalid_argument("The topleft pixel is the first non zero pixel");
		}
		backtrackPixel.x = currentPixel.x;
		backtrackPixel.y = -1;

	}
	else
	{
		backtrackPixel.x = currentPixel.x - 1;
		backtrackPixel.y = currentPixel.y;
	}
	return;

}

// https://www.codeproject.com/Articles/1105045/Tracing-Boundary-in-D-Image-Using-Moore-Neighborho
// https://github.com/TarasMartynyuk/Boundary-Tracing-Moore-Neighbours
/// <summary>
/// Gets the contours.
/// </summary>
/// <param name="image">The image.</param>
/// <param name="contourVecVec">The contour vec vec.</param>
/// <param name="threshAreaMin">The thresh area minimum.</param>
/// <param name="threshAreaMax">The thresh area maximum.</param>
/// <returns></returns>
int MooreBoundaryTracer::getContours(const cv::Mat& image, OUT std::vector<std::vector<cv::Point>>& contourVecVec, int threshAreaMin, int threshAreaMax)
{
	cv::Mat blobImage;
	std::vector<cv::Point2d *> firstPixels;
	std::vector<cv::Point2d *> posVec;
	std::vector<int> areaVec;

	// Get first pixels of all blobs
	labelBLOBsInfo(image, blobImage, firstPixels, posVec, areaVec, threshAreaMin, threshAreaMax);

	// looping through the pixels
	// Note in point 2d x and y are switched
	for(cv::Point2d* point: firstPixels)
	{
		cv::Point currentPixel = cv::Point(point->y,point->x);
		cv::Point backtrackPixel;
		getBackTrackPixel(image, currentPixel, backtrackPixel);
		contourVecVec.push_back(getBoundaryPoints(image, currentPixel, backtrackPixel));
	}

	return firstPixels.size();
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
		//printImageToConsole(neighbours); // debug only
		int offsetIndex = getOffSetIndex(backtrackPixel - currentPixel);

		// Search clockwise for non zero pixel unit startpoint is back
		getNextClockwiseBoundaryPoint(neighbours, currentPixel, backtrackPixel, offsetIndex);
		if(startingPoint == currentPixel)
		{
			backAtStart = true;
		}
	}
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
	const std::vector<std::vector<cv::Point>>& contourPoints, const int pixelVal)
{
	// set points to a value
	for(auto const& blob: contourPoints)
	{
		for(auto const& point: blob)
		{
			image.at<ushort>(point) = pixelVal;
		}
	}
}


void MooreBoundaryTracer::printImageToConsole(const cv::Mat image)
{
	for (int i = 0; i < image.rows; ++i)
	{
		for (int j = 0; j < image.cols; ++j)
		{
			//const char *c = int(image.at<ushort>(i, j)) == 0 ? "0" : "1";

			ushort c = image.at<ushort>(i, j);
			if(c == 100)
				std::cout << '1';
			else if(c == 101)
				std::cout << '2';
			else if(c == -1)
				std::cout << '0';
			else
				std::cout << '0';
			
		}
		std::cout << " Row:" << i << "\n";
	}
}



int MooreBoundaryTracer::getOffSetIndex(const cv::Point& offset)
{
	return std::distance(neighbour_coordinates.begin(),std::find(neighbour_coordinates.begin(), neighbour_coordinates.end(), offset));
}
