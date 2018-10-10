#include "AppStart.h"
#include <iostream>
#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>
#include "TestClass.h"

int main()
{
	/*Camera cam(EXTERNAL_CAM);
	while(1)
	{
		cam.captureImage();
		cam.displayImage();
		cv::waitKey();
	}*/

	//TestClass::testChainCode();
	//TestClass::testBoundingBoxes();
	//std::cout << "press enter on the contour image" << std::endl; // todo remove after presentation
	//TestClass::testBoundingBoxesTraining();
	TestClass::testFloodFill();

	
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