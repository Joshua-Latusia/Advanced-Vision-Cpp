#include "BoundingBoxer.h"



BoundingBoxer::BoundingBoxer()
{
}


BoundingBoxer::~BoundingBoxer()
{
}

int BoundingBoxer::getBoundingBoxes(const std::vector<std::vector<cv::Point>>& contours,
	std::vector<std::vector<cv::Point>>& bbs)
{
	for(std::vector<cv::Point> contour: contours)
	{
		// Get extremes
		boundingBoxStruct extremes = getContourExtremes(contour);

		// Construct bounding box vectpr
		bbs.push_back(generateBoundingBox(extremes));
	}

	return 1;

}

std::vector<cv::Point> BoundingBoxer::generateBoundingBox(boundingBoxStruct& extremes)
{
	std::vector<cv::Point> points;
	// get top points
	cv::Point leftTop = cv::Point(extremes.min.x, extremes.min.y);
	cv::Point rightTop = cv::Point(extremes.max.x, extremes.min.y);
	cv::Point rightBot = cv::Point(extremes.max.x, extremes.max.y);
	cv::Point leftBot = cv::Point(extremes.min.x, extremes.max.y);

	// left top to right top

	// right top to right bot

	// right bot to left bot

	// left bot to right top

	// generate and return box
}

boundingBoxStruct BoundingBoxer::getContourExtremes(std::vector<cv::Point>& contour)
{
	boundingBoxStruct boxStruct;

	for (cv::Point point : contour)
	{
		if (boxStruct.min.x < point.x)
		{
			boxStruct.min.x = point.x;
		}
		if (boxStruct.min.y < point.y)
		{ 
			boxStruct.min.y = point.y;
		}
		if (boxStruct.max.x > point.x)
		{
			boxStruct.max.x = point.x;
		}
		if (boxStruct.max.y > point.y)
		{
			boxStruct.max.y = point.y;
		}
	}

	return boxStruct;
}

void BoundingBoxer::drawBoundingBoxes(const cv::Mat& image, cv::Mat& outputImage,
	std::vector<std::vector<cv::Point>>& bbs)
{
	image.copyTo(outputImage);
	// set points to a value
	for (auto const& box : bbs)
	{
		for (auto const& point : box)
		{
			outputImage.at<ushort>(point) = 1;
		}
	}
}
