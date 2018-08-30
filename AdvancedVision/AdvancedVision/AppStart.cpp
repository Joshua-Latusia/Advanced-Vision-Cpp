#include "AppStart.h"
#include <iostream>
#include <opencv2/opencv.hpp>


AppStart::AppStart()
{

}


AppStart::~AppStart()
{
}

/// <summary>
/// Waits for key input so app doesn't close.
/// </summary>
void AppStart::WaitKey() const
{
	char c;
	std::cin >> c;
}
