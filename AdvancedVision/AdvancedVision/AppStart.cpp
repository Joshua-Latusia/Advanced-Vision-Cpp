#include "AppStart.h"
#include <iostream>
#include <opencv2/highgui/highgui.hpp>
#include "TestClass.h"

/// <summary>
/// Starting point of the application
/// </summary>
/// <returns></returns>
int main()
{
	AppStart::printIntro();
	AppStart::askForTest();
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

void AppStart::printIntro()
{
	std::cout << "Hello and welcome to the Advanced C++ project made by:\n"
		"Joshua Latusia and Cezan von Meijenfeldt.\n"
		"This project was made during our minor Vision and Robotics\n"
		"during our fourth year of Computer Science at Avans Hogeschool Breda.\n"
		"\n"
		"This application is capable of multiple things like: calculate bending energy, generating and drawing bounding boxes, drawing contours and so on.\n"
		"\n"
		"For simplicity sake, we set up 5 test functions to display these functionalities.\n"
		"\n"
		"1: Testing of getting a boundary from an image and displaying it.\n"
		"2: Testing calculation of the chaincode of a contour.\n"
		"3: Testing of generating and displaying bounding boxes.\n"
		"4: Extracting the bounding boxed images to seperate images for late use in combination with a neural network.\n"
		"5: Testing of our own written FloodFill algorithm.\n"
		"\n"
		"To see the result of these test type in the test number and press enter." <<
		std::endl;
}

void AppStart::askForTest()
{
	std::string input;
	std::cout << "\n\nWhich test do you want to see? (1-5) " << std::endl;
	std::cin >> input;

	// Convert the number into a string.
	const int caseNumber = stoi(input);
	displayTest(caseNumber);

}

void AppStart::displayTest(const int caseNumber)
{
	switch (caseNumber)
	{
	case 1:
		TestClass::testBoundaryPoints();
		break;
	case 2:
		TestClass::testChainCode();
		break;
	case 3:
		TestClass::testBoundingBoxes();
		break;
	case 4:
		TestClass::testBoundingBoxesTraining();
		break;
	case 5:
		TestClass::testFloodFill();
		break;

	default:
		std::cout << "Input was invalid, please try again" << std::endl;
		break;
	}
	askForTest();
}