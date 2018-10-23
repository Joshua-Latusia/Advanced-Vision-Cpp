#pragma once
#define OUT
#include <string>

class ImageLoader
{
public:
	ImageLoader();
	~ImageLoader();
	
	/// <summary>
	/// Loads the image from path.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="path">The path.</param>
	/// <returns>false if loading fails, true if loading succeeds</returns>
	static bool loadImageFromPath(OUT cv::Mat& image, const std::string& path);
};

