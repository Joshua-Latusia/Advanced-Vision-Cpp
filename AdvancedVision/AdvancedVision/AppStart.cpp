#include "AppStart.h"
#include <iostream>
#include "Camera.h"
#include "TestClass.h"

#define WEBCAM 0

int main()
{
	
	TestClass::testChainCode();
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
