#pragma once
#include <string>
#include <vector>
#include <opencv2/core/mat.hpp>
#include "CsvToTrainingSet.h"

class TrainingSetToCsv
{
public:
	TrainingSetToCsv();
	~TrainingSetToCsv();

	static void setToCsv(const std::string path, const csvColums csvColums, cv::Mat_<double> inputSet);
};

