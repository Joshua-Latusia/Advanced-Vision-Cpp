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
	Mat image, greyImage, binaryImage;
	std::vector<std::vector<Point>> contourVector;
	ImageLoader::loadImageFromPath(image,
	                               R"(c:\Programming Projects\Advanced-Vision-Cpp\AdvancedVision\AdvancedVision\Res\monsters.jpg)");
	if(!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		// Convert to 16 bit grey img and cutout piece
		cvtColor(image, greyImage, CV_BGR2GRAY);
		greyImage.convertTo(greyImage, CV_16S);

		// Threshold the number 
		threshold(greyImage, binaryImage, 100, 1, THRESH_BINARY_INV);

		// Display all the images
		show16SImageStretch(greyImage, "Grey image");
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		Mat contourImage = Mat::zeros(binaryImage.rows,binaryImage.cols, binaryImage.type());
		std::vector<std::vector<Point>> contourPoints;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints, 10);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		show16SImageStretch(contourImage, "Contour image");

		waitKey(0);

	}

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
