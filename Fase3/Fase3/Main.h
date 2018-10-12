#pragma once
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/base.hpp>

class Main
{
public:
	Main();
	~Main();
	void TestHeart();
	void TestLightning();
	void runCamera();
	void createEmptyBorder(cv::Mat& image, int borderSize);
	void trainNetwork();
	void setTestData(cv::String name, cv::Mat classification);
};

