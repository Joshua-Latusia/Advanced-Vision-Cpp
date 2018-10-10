#include <vector>
#include <fstream>
#include <sstream>
#include <iterator>
#include <opencv2/core/mat.hpp>
#include <iostream>
#include "CsvParser.h"
#include "csvfile.h"
#include <experimental/filesystem>
#include <opencv2/core.hpp>


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
		cv::Mat inputRow = (cv::Mat_<double>(1, static_cast<int>(vstrings.size()) ) <<
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
		std::cout << "The Csv colums are invalid for file: " << path << std::endl;
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
	
void CsvParser::writeToCsv(const std::string path, csvColums& csvColums, cv::Mat_<double>& completeSet, bool overWriteExisting, bool removeBias)
{
	// If not overwritable check if file exists
	if(!overWriteExisting)
	{
		if (std::experimental::filesystem::exists(path))
		{
			// TODO return error and exit function
			std::cout << "Trying to overwrite file on " << path << " while this is not permitted";
			return;
		}
	}

	if (removeBias)
	{
		// remove bias
		completeSet = completeSet.colRange(1, completeSet.cols);
	}

	// Creates new csv or overwrite old one
	csvfile csv(path,",");

	// Add Column names
	for (const auto columnName : csvColums.columnNames)
	{
		csv << columnName;
	}
	csv << endrow;
	
	// Add Data from left top to right bottom
	for(int y = 0; y < completeSet.rows; ++y)
	{
		for(int x = 0; x < completeSet.cols; ++x)
		{
			csv << completeSet.at<double>(cv::Point(x, y));
		}
		csv << endrow;
	}
}

void CsvParser::writeToCsv(const std::string path, csvColums& csvColums, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet, bool overWriteExisting, bool removeBias)
{
	if(removeBias)
	{
		inputSet = inputSet.colRange(1, inputSet.cols);
	}

	// append outputset tot the right of the inputset
	hconcat(inputSet, outputSet, inputSet);
	writeToCsv(path, csvColums, inputSet,overWriteExisting, false);
}
