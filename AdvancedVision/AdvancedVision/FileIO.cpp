#include "FileIO.h"
#include <opencv2/imgcodecs/imgcodecs_c.h>
#include <opencv2/imgcodecs.hpp>
#include <experimental/filesystem>
#include <iostream>

namespace fs = std::experimental::filesystem;


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

void FileIO::getFilesFromDir(const std::string path, std::vector<std::string>& filenames, const std::vector<std::string> extensionFilter)
{
	// get all files in dir
	for (auto& s : fs::directory_iterator(path))
	{
		filenames.push_back(s.path().string());
	}

	// filter by extension
	if (!extensionFilter.empty())
	{
		filterByExtension(filenames, extensionFilter);
	}
}

void FileIO::filterByExtension(std::vector<std::string>& filenames, const std::vector<std::string> extensionFilter)
{
	std::vector<std::string> result;
	for (auto& filename : filenames)
	{
		fs::path filePath = filename;
		if (std::find(extensionFilter.begin(), extensionFilter.end(), filePath.extension()) != extensionFilter.end())
		{
			result.push_back(filename);
		}
	}
	filenames = result;
}

bool FileIO::loadFromFile(const std::string path, cv::Mat image)
{
	try
	{
		image = cv::imread(path, CV_LOAD_IMAGE_COLOR);
		return image.data;
	}
	catch(cv::Exception& e)
	{
		std::cout << e.msg << std::endl;
		return false;
	}
}
