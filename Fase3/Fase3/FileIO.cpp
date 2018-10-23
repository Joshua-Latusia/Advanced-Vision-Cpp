#include "FileIO.h"
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgcodecs.hpp>


FileIO::FileIO()
{
}


FileIO::~FileIO()
{
}

bool FileIO::saveImage(const cv::Mat& image, const std::string& path = "", const std::string& filename = "", const std::string
	& fileExtension = ".jpg")
{
	cv::imwrite(path + '\\' +filename + fileExtension, image);

	// Check if file can be read after saving
	cv::Mat img = cv::imread(path, CV_LOAD_IMAGE_COLOR);
	return img.data;
}