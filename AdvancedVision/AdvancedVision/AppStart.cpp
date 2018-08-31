#include "AppStart.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>

#define WEBCAM 0

int main()
{
	Camera Camera;

	while (true)
	{
		Camera.CaptureImage();
		Camera.DisplayImage();

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
