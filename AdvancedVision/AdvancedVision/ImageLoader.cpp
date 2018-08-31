#include "ImageLoader.h"
#include <opencv2/imgcodecs.hpp>


ImageLoader::ImageLoader()
{
}


ImageLoader::~ImageLoader()
{
}

bool ImageLoader::LoadImageFromPath(OUT cv::Mat& image, std::string path)
{
	image = cv::imread(path);
	return image.data;
}
