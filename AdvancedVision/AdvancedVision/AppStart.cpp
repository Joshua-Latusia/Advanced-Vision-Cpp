#include "AppStart.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>
#include "ImageLoader.h"
#include "ImageConverter.h"

#define WEBCAM 0

int main()
{
	// Read in image and diplay it
	cv::Mat image;
	std::vector<std::vector<cv::Point>> contourVector;
	ImageLoader::LoadImageFromPath(image,
	                               R"(c:\Programming Projects\Advanced-Vision-Cpp\AdvancedVision\AdvancedVision\Res\monsters.jpg)");
	if(!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		namedWindow("Normal image", cv::WINDOW_AUTOSIZE);
		cv::imshow("Normal image", image);
		ImageConverter::GetContours(image, contourVector);


		// Otherwise the image will be gray
		cv::waitKey(1);
	}
	

	// Display camera
	//Camera Camera;
	//while (true)
	//{
	//	Camera.CaptureImage();
	//	Camera.DisplayImage();

	//	// escape loop
	//	if (cv::waitKey(1) == 27)
	//		break;
	//}

	AppStart::WaitKey();

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
