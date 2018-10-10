#include "CsvToTrainingSet.h"
#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <opencv2/core/mat.hpp>
#include <iostream>


CsvToTrainingSet::CsvToTrainingSet()
{
}


CsvToTrainingSet::~CsvToTrainingSet()
{
}

void CsvToTrainingSet::toTrainingSet(const std::string path, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet)
{
	int index = 0;

	// Open filestream
	std::ifstream fs;
	fs.open(path);

	// Read file line by line
	std::string line;
	while (!fs.eof()) 
	{
		fs >> line;

		// Skip first line and lines with text
		if (index == 0)
		{
			index++;
			continue;;
		}
		

		// Splitting line into seperated string with the comma delimiter
		std::stringstream ss(line);
		ss.imbue(std::locale(std::locale(), new tokens()));
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vstrings(begin, end);

		double d = std::stod(vstrings[VARIANCE_INDEX]);

		// Add input to inputmatrix, the first element in the row should be a 1
		cv::Mat inputRow = (cv::Mat_<double>(1, vstrings.size()) <<
			1, std::stod(vstrings[VARIANCE_INDEX]), std::stod(vstrings[SKEWNESS_INDEX]), std::stod(vstrings[CURTOSIS_INDEX]), std::stod(vstrings[ENTROPY_INDEX]));
		inputSet.push_back(inputRow);

		// Add output to outputmatrix
		cv::Mat outputRow = (cv::Mat_<double>(1, 1) << std::stod(vstrings[CLASS_INDEX]));
		outputSet.push_back(outputRow);

	}

	// Close the File
	fs.close();
}
	

