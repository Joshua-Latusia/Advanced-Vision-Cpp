#include "MooreBoundaryTracer.h"
#include <opencv2/core/mat.hpp>
#include <iostream>

const std::array<cv::Point, 8> NeighbourCoordinates
{
	cv::Point{ 0, -1 },		// left
	cv::Point{ -1, -1 }, 	// top left
	cv::Point{ -1, 0 }, 	// top
	cv::Point{ -1, 1 }, 	// top right
	cv::Point{ 0, 1 },		// right
	cv::Point{ 1, 1 },		// bottom right
	cv::Point{ 1, 0 },		// bottom
	cv::Point{ 1 , -1 }, 	// bottom left
};

MooreBoundaryTracer::MooreBoundaryTracer()
{
}


MooreBoundaryTracer::~MooreBoundaryTracer()
{
}

void MooreBoundaryTracer::FindFirstNonZeroPixel(const cv::Mat & image, cv::Point& nonZeroPixel, cv::Point& previousNeighbour)
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
					previousNeighbour.x = -1;
					previousNeighbour.y = y;

				}
				else
				{
					previousNeighbour.x = x;
					previousNeighbour.y = y -1;
				}
				return;
			}
		}
	}

	throw std::invalid_argument("No nonzero pixel found in the image");
}

void MooreBoundaryTracer::PrintImageToConsole(const cv::Mat image)
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