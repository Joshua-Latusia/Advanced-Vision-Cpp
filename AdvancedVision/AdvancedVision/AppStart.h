#pragma once
class AppStart
{
public:
	AppStart();
	~AppStart();
	static void waitKey();
	static void printIntro();
	static void askForTest();
	static void displayTest(int caseNumber);
};

