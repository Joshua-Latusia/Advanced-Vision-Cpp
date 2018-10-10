#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "CsvParser.h"


CsvParser::CsvParser()
{
}


CsvParser::~CsvParser()
{
}

void CsvParser::bankNotesCsvtoTrainingSet(const std::string path, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet)
{
	// Open filestream
	std::ifstream fs;
	fs.open(path);
	
	// Read file line by line
	std::string line;
	// skip first line
	fs >> line;

	while (!fs.eof()) 
	{
		fs >> line;

		// Splitting line into seperated string with the comma delimiter
		std::stringstream ss(line);
		ss.imbue(std::locale(std::locale(), new tokens()));
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vstrings(begin, end);

		// Add input to inputmatrix, the first element in the row should be a 1
		cv::Mat inputRow = (cv::Mat_<double>(1, vstrings.size() ) <<
			1, std::stod(vstrings[VARIANCE_INDEX]), std::stod(vstrings[SKEWNESS_INDEX]), std::stod(vstrings[CURTOSIS_INDEX]), std::stod(vstrings[ENTROPY_INDEX]));
		inputSet.push_back(inputRow);

		// Add output to outputmatrix
		cv::Mat outputRow = (cv::Mat_<double>(1, 1) << std::stod(vstrings[CLASS_INDEX]));
		outputSet.push_back(outputRow);
	}

	// Close the File
	fs.close();
}

csvColums CsvParser::csvToTrainingSet(const std::string path, const int featureColumns, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet)
{
	// Open filestream
	std::ifstream fs;
	fs.open(path);

	// Read file line by line
	std::string line;

	// Get the csv colums object
	fs >> line;
	std::stringstream ss(line);
	ss.imbue(std::locale(std::locale(), new tokens()));
	std::istream_iterator<std::string> begin(ss);
	std::istream_iterator<std::string> end;
	std::vector<std::string> columnNames(begin, end);

	csvColums csvColums = {
		columnNames,
		featureColumns,
		static_cast<int>(columnNames.size()) - featureColumns
	};

	if(!csvColums.isValid())
	{
		// TODO return error if the struct is not valid
		return csvColums;
	}

	// parse the actual file
	while (!fs.eof())
	{
		fs >> line;

		// Splitting line into seperated string with the comma delimiter
		std::stringstream ss(line);
		ss.imbue(std::locale(std::locale(), new tokens()));
		std::istream_iterator<std::string> begin(ss);
		std::istream_iterator<std::string> end;
		std::vector<std::string> vstrings(begin, end);

		// Add input to inputmatrix, the first element in the row should be a 1
		cv::Mat inputRow = cv::Mat_<double>(1, csvColums.featureColums + 1);
		inputRow.at<double>(cv::Point(0, 0)) = 1;

		for(int i = 0; i < csvColums.featureColums; ++i)
		{
			// always take i + 1 because of skipping the bias (first element)
			inputRow.at<double>(cv::Point(i + 1, 0)) = std::stod(vstrings[i]);

		}
		std::cout << std::endl;
		inputSet.push_back(inputRow);

		// Add output to outputmatrix
		cv::Mat outputRow = cv::Mat_<double>(1, csvColums.outputColumns);
		for(int i = 0; i < csvColums.outputColumns; ++i)
		{
			outputRow.at<double>(cv::Point(i, 0)) = std::stod(vstrings[i + csvColums.featureColums]);
		}
		outputSet.push_back(outputRow);
	}
	return csvColums;
}
	

