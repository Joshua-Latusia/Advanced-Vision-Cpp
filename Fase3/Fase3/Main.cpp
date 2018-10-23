#include "Main.h"
#include <opencv2/core/types.hpp>
#include <opencv2/highgui/highgui.hpp>
#include "MooreBoundaryTracer.h"
#include "avansvisionlib20.h"
#include "ImageLoader.h"
#include <opencv2/imgproc.hpp>
#include <vector>

#include <iomanip>
#include <math.h>

Main::Main()
{
}


Main::~Main()
{
}

cv::Mat_<double> ITset, OTset;
cv::Mat V0, W0, dW0, dV0;

int hiddenNeurons = 7;

// Maximale fout die toegestaan wordt in de output voor de training input
const double MAX_OUTPUT_ERROR = 1E-10;

// maximaal aantal runs dat uitgevoerd wordt bij het trainen
const int MAXRUNS = 10000;

/// <summary>
/// Starting point of the application.
/// </summary>
/// <param name="argc">The argc.</param>
/// <param name="argv">The argv.</param>
/// <returns></returns>
int main(int argc, char* argv[])
{
	Main main;
	//main.setTestData("Key", 0);
	main.setTestData("Hearts", (cv::Mat_<double>(1, 1) << 0 ));
	//main.setTestData("Circles", (cv::Mat_<double>(1, 2) << 1, 0));
	//main.setTestData("Squares", (cv::Mat_<double>(1, 2) << 0, 1));
	main.setTestData("Lightningbolt", (cv::Mat_<double>(1, 1) << 1));

	std::cout << "Training Input " << std::endl << std::endl;
	std::cout << ITset << std::endl << std::endl;
	std::cout << "Training Output " << std::endl << std::endl;
	std::cout << OTset << std::endl << std::endl;

	main.trainNetwork();
	//main.TestLightning();
	main.runCamera();
	getchar();
}


void Main::CheckContour(std::vector<cv::Point> contourPoints)
{
	//calculates circularity
	double perimeter = cv::arcLength(contourPoints, true);
	double area = cv::contourArea(contourPoints);
	double circularity = 4 * CV_PI *(area / (perimeter*perimeter));
	std::vector<cv::Point> approx;

	//calculate if contour is convex


	std::vector<cv::Point> convexHull;
	cv::convexHull(contourPoints, convexHull);
	bool isConvex = cv::isContourConvex(convexHull);

	std::vector<int> convexHullI;
	cv::convexHull(contourPoints, convexHullI);
	std::vector<cv::Vec4i> defectPoints(convexHullI.size());
	cv::convexityDefects(contourPoints, convexHullI, defectPoints);

	auto hull_area = cv::contourArea(convexHull);

	// calculate solidity
	auto solidity = area / hull_area;

	cv::Mat	IVec = (cv::Mat_<double>(1, 3) <<
		circularity, (double)defectPoints.size() / 100, solidity);

	std::cout << "circularity: " << circularity << std::endl;
	std::cout << "isCOnvex: " << isConvex << std::endl;
	std::cout << "convex defects: " << (double)defectPoints.size() / 100 << std::endl;
	std::cout << "solidity: " << solidity << std::endl;

	auto outputVectorBPN = BPN(IVec, V0, W0);

	// druk de output vector van het BPN af in dezelfde regel afgesloten met |
	for (int r = 0; r < outputVectorBPN.rows; r++)
		std::cout << std::setw(8) << round(getEntry(outputVectorBPN, r, 0));

	std::cout<<std::endl;
}

void Main::TestLightning()
{
	cv::Mat image;
	std::string path = R"(Res\NeuralTestSets\Lightningbolt\Lightningbolt_0.jpg)";

	ImageLoader::loadImageFromPath(image, path);


	cv::Mat binaryImage;
	// Convert to 16 bit
	cvtColor(image, binaryImage, CV_BGR2GRAY);
	binaryImage.convertTo(binaryImage, CV_16S);
	threshold(binaryImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

	cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
	std::vector<std::vector<cv::Point>> contourPoints;
	MooreBoundaryTracer::getContours(binaryImage, contourPoints);

	//calculates circularity
	double perimeter = cv::arcLength(contourPoints[0], true);
	double area = cv::contourArea(contourPoints[0]);
	double circularity = 4 * CV_PI *(area / (perimeter*perimeter));
	std::vector<cv::Point> approx;

	//calculate if contour is convex
	std::vector<cv::Point> convexHull;
	cv::convexHull(contourPoints[0], convexHull);
	bool isConvex = cv::isContourConvex(convexHull);

	std::vector<int> convexHullI;
	cv::convexHull(contourPoints[0], convexHullI);
	std::vector<cv::Vec4i> defectPoints(convexHullI.size());
	cv::convexityDefects(contourPoints[0], convexHullI, defectPoints);

	auto hull_area = cv::contourArea(convexHull);

	// calculate solidity
	auto solidity = area / hull_area;

	cv::Mat	IVec = (cv::Mat_<double>(1, 3) <<
		circularity, (double)defectPoints.size() / 100, solidity);
	
	auto outputVectorBPN = BPN(IVec, V0, W0);

	// druk de output vector van het BPN af in dezelfde regel afgesloten met |
	for (int r = 0; r < outputVectorBPN.rows; r++)
		std::cout << std::setw(8) << round(getEntry(outputVectorBPN, r, 0));
}



void Main::runCamera()
{
	cv::VideoCapture cap(1); // open the default camera
	if (!cap.isOpened())  // check if we succeeded
		return;

	cv::Mat edges;
	//v::namedWindow("liveFeed", 1);
	for (;;)
	{
		cv::Mat frame;
		cap >> frame; // get a new frame from camera
		cv::Mat binaryImage;
		cv::cvtColor(frame, binaryImage, CV_BGR2GRAY);
		binaryImage.convertTo(binaryImage, CV_16S);

		threshold(binaryImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

		createEmptyBorder(binaryImage, 10);
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		show16SImageClip(binaryImage, "feed");

		MooreBoundaryTracer::getContours(binaryImage, contourPoints, 100);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		show16SImageClip(contourImage *255, "feed");
		for(int contours = 0; contours < contourPoints.size(); contours++)
			CheckContour(contourPoints[contours]);
		//imshow("edges", binaryImage * 255);
		if (cv::waitKey(1000) >= 0) break;
	}
	// the camera will be deinitialized automatically in VideoCapture destructor
}

void Main::createEmptyBorder(cv::Mat& image, int borderSize)
{
	cv::Mat borderedImage = cv::Mat::zeros(image.rows + borderSize* 2, image.cols + borderSize * 2, image.type());

	for(int y = 0; y < image.rows; ++y)
	{
		for(int x = 0; x < image.cols; ++x)
		{
			borderedImage.at<ushort>(cv::Point(x + borderSize , y + borderSize )) = image.at<ushort>(cv::Point(x,y));
		}
	}
	image = borderedImage;
}


void Main::trainNetwork()
{
	initializeBPN(ITset.cols, hiddenNeurons, OTset.cols, V0, dV0, W0, dW0);

	cv::Mat IT, OT, OH, OO;
	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
	cv::Mat V1, W1;

	int runs = 0;


	while ((sumSqrDiffError > MAX_OUTPUT_ERROR) && (runs < MAXRUNS)) 
	{

		sumSqrDiffError = 0;

		for (int inputSetRowNr = 0; inputSetRowNr < ITset.rows; inputSetRowNr++) {
			IT = transpose(getRow(ITset, inputSetRowNr));

			OT = transpose(getRow(OTset, inputSetRowNr));

			calculateOutputHiddenLayer(IT, V0, OH);

			calculateOutputBPN(OH, W0, OO);

			adaptVW(OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1);

			calculateOutputBPNError(OO, OT, outputError0);

			calculateOutputBPNError(BPN(IT, V1, W1), OT, outputError1);

			sumSqrDiffError += (outputError1 - outputError0) * (outputError1 - outputError0);

			V0 = V1;
			W0 = W1;
		}
		std::cout << "sumSqrDiffError = " << sumSqrDiffError << std::endl;
		runs++;
	}

	std::cout << "BPN Training is ready!" << std::endl << std::endl;
	std::cout << "Runs = " << runs << std::endl << std::endl;

	cv::Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;

	// druk voor elke input vector uit de trainingset de output vector uit trainingset af 
	// tezamen met de output vector die het getrainde BPN (zie V0, W0) genereerd bij de 
	// betreffende input vector.
	std::cout << std::setw(16) << " " << "Training Input" << std::setw(12) << "|" << " Expected Output "
		<< std::setw(1) << "|" << " Output BPN " << std::setw(6) << "|" << std::endl << std::endl;
	for (int row = 0; row < ITset.rows; row++) {

		// haal volgende inputvector op uit de training set
		inputVectorTrainingSet = transpose(getRow(ITset, row));

		// druk de inputvector af in een regel afgesloten met | 
		for (int r = 0; r < inputVectorTrainingSet.rows; r++)
			std::cout << std::setw(8) << getEntry(inputVectorTrainingSet, r, 0);
		std::cout << std::setw(2) << "|";

		// haal bijbehorende outputvector op uit de training set
		outputVectorTrainingSet = transpose(getRow(OTset, row));

		// druk de outputvector van de training set af in dezelfde regel afgesloten met | 
		for (int r = 0; r < outputVectorTrainingSet.rows; r++)
			std::cout << std::setw(8) << round(getEntry(outputVectorTrainingSet, r, 0));
		std::cout << std::setw(2) << "|";

		// bepaal de outputvector die het getrainde BPN oplevert 
		// bij de inputvector uit de trainingset  
		outputVectorBPN = BPN(inputVectorTrainingSet, V0, W0);

		// druk de output vector van het BPN af in dezelfde regel afgesloten met |
		for (int r = 0; r < outputVectorBPN.rows; r++)
			std::cout << std::setw(8) << round(getEntry(outputVectorBPN, r, 0));
		std::cout << std::setw(2) << "|";

		std::cout << std::endl;
	}

}


void Main::setTestData(cv::String name, cv::Mat classification)
{
	for(int imageIndex = 0; imageIndex < 9; imageIndex++)
	{
		cv::Mat image;

		std::string path = "Res\\NeuralTestSets\\" + name + "\\" + name + "_" + std::to_string(imageIndex) + ".jpg";

		ImageLoader::loadImageFromPath(image, path);

		
		cv::Mat binaryImage;
		// Convert to 16 bit
		cvtColor(image, binaryImage, CV_BGR2GRAY);
		binaryImage.convertTo(binaryImage, CV_16S);
		threshold(binaryImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints);
		
	


		//calculates circularity
		double perimeter = cv::arcLength(contourPoints[0], true);
		double area = cv::contourArea(contourPoints[0]);
		double circularity = 4 * CV_PI *(area / (perimeter*perimeter));
		std::vector<cv::Point> approx;

		//calculate if contour is convex
		

		std::vector<cv::Point> convexHull;
		cv::convexHull(contourPoints[0], convexHull);
		bool isConvex = cv::isContourConvex(convexHull);

		std::vector<int> convexHullI;
		cv::convexHull(contourPoints[0], convexHullI);
		std::vector<cv::Vec4i> defectPoints(convexHullI.size());
		cv::convexityDefects(contourPoints[0], convexHullI, defectPoints);

		auto hull_area = cv::contourArea(convexHull);

		// calculate solidity
		auto solidity = area / hull_area;


		std::cout << name << "_" << imageIndex << "values" << std::endl;
		std::cout << "circularity: " << circularity << std::endl;
	//	std::cout << "isCOnvex: " << isConvex << std::endl;
		std::cout << "convex defects: " << (double)defectPoints.size() / 100 << std::endl;
		std::cout << "solidity: " << solidity << std::endl;
		

		cv::Mat	IVec = (cv::Mat_<double>(1, 3) <<
			circularity, (double)defectPoints.size()/100, solidity);
		ITset.push_back(IVec);


		//cv::Mat OVec = (cv::Mat_<double>(1, 2) << outputValue);
		OTset.push_back(classification);
	}
}
