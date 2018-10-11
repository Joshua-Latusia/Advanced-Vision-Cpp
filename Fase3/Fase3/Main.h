#pragma once
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/base.hpp>

class Main
{
public:
	Main();
	~Main();
	void trainNetwork();
	void setTestData(cv::String name, cv::Mat classification);
};

