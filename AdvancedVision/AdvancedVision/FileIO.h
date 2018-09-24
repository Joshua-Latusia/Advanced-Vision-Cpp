#pragma once
#include <string>
#include <opencv2/core/mat.hpp>

class FileIO
{
public:
	FileIO();
	~FileIO();

	/// <summary>
	/// Saves the file to given path.
	/// </summary>
	/// <param name="image">The image object.</param>
	/// <param name="path">The path where to save the image to.</param>
	/// <param name="filename"> Name of the file.</param>
	/// <param name="fileExtension"> Extension of the file. </param>
	/// <returns>Wheter ot not the image was saved succesfully</returns>
	static bool saveImage(const cv::Mat& image, const std::string& path, const std::string& filename, const std::string&
		fileExtension);
};

