#include "BoundingBoxer.h"
#include "FileIO.h"
#include <windows.h>
#include <stdio.h>
#include <iostream>

BoundingBoxer::BoundingBoxer()
{
}


BoundingBoxer::~BoundingBoxer()
{
}

int BoundingBoxer::getBoundingBoxes(const std::vector<std::vector<cv::Point>>& contours,
	std::vector<std::vector<cv::Point>>& bbs, std::vector<boundingBoxStruct>& boundingBoxStructs)
{
	for(std::vector<cv::Point> contour: contours)
	{
		// Get extremes
		boundingBoxStruct extremes = getContourExtremes(contour);
		boundingBoxStructs.push_back(extremes);
		// Construct bounding box vector
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

	// left top to right top and  right bot to left bot
	for(int i = extremes.min.x; i < extremes.max.x; ++i)
	{
		points.emplace_back(i, extremes.min.y);
		points.emplace_back(i, extremes.max.y);
	}
	// right top to right bot and left bot to right top
	for(int i = extremes.min.y; i < extremes.max.y; ++i)
	{
		points.emplace_back(extremes.max.x, i);
		points.emplace_back(extremes.min.x, i);
	}

	// generate and return box
	return points;
}

double BoundingBoxer::getBoxRatio(boundingBoxStruct& extremes)
{
	return getBoxRatio(extremes.max.x - extremes.min.x, extremes.max.y - extremes.min.y);
}

double BoundingBoxer::getBoxRatio(int length, int width)
{
	if(width > length)
		return static_cast<double>(width) / static_cast<double>(length);


	return static_cast<double>(length) / static_cast<double>(width);
}

boundingBoxStruct BoundingBoxer::getContourExtremes(std::vector<cv::Point>& contour)
{
	boundingBoxStruct boxStruct;

	for (const cv::Point point : contour)
	{
		if (point.x < boxStruct.min.x)
		{
			boxStruct.min.x = point.x;
		}
		if (point.y < boxStruct.min.y)
		{ 
			boxStruct.min.y = point.y;
		}
		if (point.x > boxStruct.max.x)
		{
			boxStruct.max.x = point.x;
		}
		if (point.y > boxStruct.max.y)
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

bool BoundingBoxer::saveBoundingBoxImages(const cv::Mat& image,
	const std::vector<boundingBoxStruct>& boundingBoxStructs, const std::string& path, const std::string& filename, const std::string
	& fileExtension)
{
	bool savedSuccess = true;
	const std::string dirName = path + '\\' + filename;

	// Check if there is a dir with the filename as dirname, otherwise create it
	if (!CreateDirectory(dirName.c_str(), NULL) ||
		ERROR_ALREADY_EXISTS == GetLastError())
	{
		std::cout << "Failed to create directory: " << dirName << " Since it already exists "<< std::endl;
	}

	// Save indivual images to this dir
	for(int i = 0; i < boundingBoxStructs.size(); ++i)
	{
		auto const boundingBoxStruct = boundingBoxStructs[i];
		int width = boundingBoxStruct.max.x - boundingBoxStruct.min.x;
		int heigth = boundingBoxStruct.max.y - boundingBoxStruct.min.y;
		const cv::Mat subImage = image(cv::Rect(boundingBoxStruct.min.x, boundingBoxStruct.min.y, width, heigth));

		const bool result = FileIO::saveImage(subImage, dirName, filename + '_' + std::to_string(i) , fileExtension);
		if(!result)
		{
			savedSuccess = false;
		}

	}
	return savedSuccess;
}
