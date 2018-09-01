#include "MooreBoundaryTracer.h"
#include <opencv2/core/mat.hpp>

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
	for(int i = 0; i < image.rows; ++i)
	{
		for(int j = 0; j < image.cols; ++j)
		{
			// Check if image is nonzero
			cv::Point point(i, j);
			if(image.at<point> != 0)
			{
				nonZeroPixel.x = i;
				nonZeroPixel.y = j;

				// If there is no pixel on the left go upward
				if(j == 0)
				{
					// if its the top left pixel
					if(i == 0)
					{
						throw std::invalid_argument("The topleft pixel is the first non zero pixel");
					}
					previousNeighbour.x = -1;
					previousNeighbour.y = j;

				}
				else
				{
					previousNeighbour.x = i;
					previousNeighbour.y = j -1;
				}
				return;
			}
		}
	}

	throw std::invalid_argument("No nonzero pixel found in the image");
}
