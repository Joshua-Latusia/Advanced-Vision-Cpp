#pragma once
#include <opencv2/core/cvstd.hpp>

class Main
{
public:
	Main();
	~Main();
	void trainNetwork();
	void setTestData(cv::String name, int outputValue);
};

