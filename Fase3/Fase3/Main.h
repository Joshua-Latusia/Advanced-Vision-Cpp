#pragma once
#include <opencv2/core/cvstd.hpp>
#include <opencv2/core/base.hpp>
#include "BendingEnergy.h"

class Main
{
public:
	Main();
	~Main();
	void TestHeart();
	void CheckContour(std::vector<cv::Point> contourPoints);
	void TestLightning();
	void runCamera();
	void createEmptyBorder(cv::Mat& image, int borderSize);
	void trainNetwork();
	void setTestData(cv::String name, cv::Mat classification);
};

