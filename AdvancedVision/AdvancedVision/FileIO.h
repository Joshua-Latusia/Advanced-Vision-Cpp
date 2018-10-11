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

	/// <summary>
	/// Gets all the files from a directory and retrieve only the extension in the extension filter.
	/// </summary>
	/// <param name="path">The path of the folder.</param>
	/// <param name="filenames">The filenames in the folder.</param>
	/// <param name="extensionFilter">The extension filter of the file extensions to get.</param>
	static void getFilesFromDir(std::string path, std::vector<std::string>& filenames,
		std::vector<std::string> extensionFilter = std::vector<std::string>());

	/// <summary>
	/// Filters the by extension, only get the extension in the extension filter..
	/// </summary>
	/// <param name="filenames">The filenames to pass a filter on.</param>
	/// <param name="extensionFilter">The extension filter of filextension to get.</param>
	static void filterByExtension(std::vector<std::string>& filenames, std::vector<std::string> extensionFilter);
	
	/// <summary>
	/// Loads file from path into math object.
	/// </summary>
	/// <param name="path">The path of where to load image from.</param>
	/// <param name="image">The image.</param>
	/// <returns>If the image contains data</returns>
	static bool loadFromFile(std::string path, cv::Mat image);
};

