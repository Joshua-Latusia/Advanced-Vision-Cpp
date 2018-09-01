#include "ImageLoader.h"
#include <opencv2/imgcodecs.hpp>
#include <opencv2/imgcodecs/imgcodecs_c.h>


ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

bool ImageLoader::LoadImageFromPath(OUT cv::Mat& image, const std::string& path)
{
	image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
	return image.data;
}
