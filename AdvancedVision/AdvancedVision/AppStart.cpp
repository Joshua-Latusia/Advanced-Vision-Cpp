#include "AppStart.h"
#include <iostream>
#include <opencv2/opencv.hpp>
#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>
#include "ImageLoader.h"
#include "ImageConverter.h"
#include "avansvisionlib.h"
#include "MooreBoundaryTracer.h"

#define WEBCAM 0

int main()
{
	// Read in image and diplay it
	cv::Mat image, greyImage, binaryImage;
	std::vector<std::vector<cv::Point>> contourVector;
	ImageLoader::loadImageFromPath(image,
	                               R"(c:\Programming Projects\Advanced-Vision-Cpp\AdvancedVision\AdvancedVision\Res\rummikub0.jpg)");
	if(!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		// Convert to 16 bit grey img and cutout piece
		cv::cvtColor(image, greyImage, CV_BGR2GRAY);
		greyImage.convertTo(greyImage, CV_16S);
		cv::Mat cutoutImage = greyImage(Rect(26, 26, 100, 100));

		// Threshold the number 
		cv::threshold(cutoutImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

		// Display all the images
		show16SImageStretch(image, "Normal image");
		show16SImageStretch(greyImage, "Grey image");
		show16SImageStretch(cutoutImage, "Cutout image");
		show16SImageStretch(binaryImage, "Binary image");

		// detect the first non zero pixel
		cv::Point firstPixel;
		cv::Point previousPixel;
		MooreBoundaryTracer::printImageToConsole(binaryImage);
		MooreBoundaryTracer::findFirstNonZeroPixel(binaryImage, firstPixel, previousPixel);

		MooreBoundaryTracer::getBoundaryPoints(binaryImage, firstPixel, previousPixel);

		std::cout << "First pixel coords X: " << firstPixel.x << "  y: " << firstPixel.y;
		waitKey(0);

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

	AppStart::waitKey();

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
void AppStart::waitKey()
{
	char c;
	std::cin >> c;
}
