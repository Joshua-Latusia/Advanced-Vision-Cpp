#include "AppStart.h"
#include <iostream>
#include <opencv2/opencv.hpp>

#define WEBCAM 0

int main()
{
	cv::Mat img;

	cv::VideoCapture cam(WEBCAM);

	cv::namedWindow("Cam screen", CV_WINDOW_NORMAL);

	while (true)
	{
		cam >> img;
		cv::imshow("camera", img);

		// escape loop
		if (cv::waitKey(1) == 27)
			break;
	}

}

AppStart::AppStart()
{
	
}


AppStart::~AppStart()
{
}

/// <summary>
/// Waits for key input so app doesn't close.
/// </summary>
void AppStart::WaitKey()
{
	char c;
	std::cin >> c;
}
