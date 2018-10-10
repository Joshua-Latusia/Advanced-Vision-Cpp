#include "TestClass.h"
#include <opencv2/highgui/highgui.hpp>
#include "avansvisionlib.h"
#include "MooreBoundaryTracer.h"
#include <opencv2/imgproc.hpp>
#include "ImageLoader.h"
#include "BendingEnergy.h"
#include "BoundingBoxer.h"
#include "FloodFill.h"


TestClass::TestClass()
{
}


TestClass::~TestClass()
{
}


void TestClass::testBoundaryPoints()
{
	// Read in image and diplay it
	cv::Mat image, greyImage, binaryImage;
	std::vector<std::vector<cv::Point>> contourVector;
	ImageLoader::loadImageFromPath(image,
		R"(Res\monsters.jpg)");
	if (!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		// Convert to 16 bit grey img and cutout piece
		cvtColor(image, greyImage, CV_BGR2GRAY);
		greyImage.convertTo(greyImage, CV_16S);

		// Threshold the number 
		threshold(greyImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

		// Display all the images
		show16SImageStretch(greyImage, "Grey image");
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints, 10);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		show16SImageStretch(contourImage, "Contour image");

		cv::waitKey(0);

	}
}

void TestClass::testChainCode()
{
	// Read in image and diplay it
	cv::Mat image, greyImage, binaryImage;
	std::vector<std::vector<cv::Point>> contourVector;
	ImageLoader::loadImageFromPath(image,
		R"(Res\monsters.jpg)");
	if (!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		// Convert to 16 bit grey img and cutout piece
		cvtColor(image, greyImage, CV_BGR2GRAY);
		greyImage.convertTo(greyImage, CV_16S);

		// Threshold the number 
		threshold(greyImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);

		// Display all the images
		show16SImageStretch(greyImage, "Grey image");
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints, 10);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		show16SImageStretch(contourImage, "Contour image");

		// Convert to 8 bit
		contourImage.convertTo(contourImage, CV_8U);

		// Get chaincodes
		const std::vector<std::vector<int>> chaincodes = BendingEnergy::getChainCode(contourImage);

		// Calculate bendingenergy
		std::vector<int> bendingEnergies = BendingEnergy::getBendingEnergy(chaincodes);
		for(double energy: bendingEnergies)
		{
			std::cout << "The bending energy is: " << energy << std::endl;
		}

		cv::waitKey(0);

	}
}

void TestClass::testBoundingBoxes()
{
	// Read in image and diplay it
	cv::Mat image;
	std::vector<std::vector<cv::Point>> contourVector;
	ImageLoader::loadImageFromPath(image,
		R"(Res\rummikubbin.bmp)");
	if (!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		
		cv::Mat binaryImage;
		// Convert to 16 bit
		cvtColor(image, binaryImage, CV_BGR2GRAY);
		binaryImage.convertTo(binaryImage, CV_16S);

		// Invert the image since the source is inverted
		threshold(binaryImage, binaryImage, 1, 1, cv::THRESH_BINARY_INV);
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		cv::Mat bbsImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		std::vector<std::vector<cv::Point>> bbsPoints;
		std::vector<boundingBoxStruct> boundingboxStructs;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		BoundingBoxer::getBoundingBoxes(contourPoints, bbsPoints, boundingboxStructs);
		BoundingBoxer::drawBoundingBoxes(contourImage, bbsImage, bbsPoints);
		show16SImageStretch(contourImage, "Contour image");
		show16SImageStretch(bbsImage, "Bbs image");

		// Save bounding boxed images to dir.
		BoundingBoxer::saveBoundingBoxImages(image, boundingboxStructs, R"(Res)", "rummikubbin");

		cv::waitKey(0);
	}
}

void TestClass::testBoundingBoxesTraining()
{
	// Read in image and diplay it
	cv::Mat image;
	std::vector<std::vector<cv::Point>> contourVector;
	/*ImageLoader::loadImageFromPath(image,
		R"(Res\harten.png)");*/
	ImageLoader::loadImageFromPath(image,
		R"(Res\harten.png)");
	if (!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		cv::imshow( "original image", image);
		cv::Mat binaryImage;
		// Convert to 16 bit
		cvtColor(image, binaryImage, CV_BGR2GRAY);
		binaryImage.convertTo(binaryImage, CV_16S);

		// Invert the image since the source is inverted
		threshold(binaryImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		cv::Mat bbsImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		std::vector<std::vector<cv::Point>> bbsPoints;
		std::vector<boundingBoxStruct> boundingboxStructs;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		BoundingBoxer::getBoundingBoxes(contourPoints, bbsPoints, boundingboxStructs);
		BoundingBoxer::drawBoundingBoxes(contourImage, bbsImage, bbsPoints);
		show16SImageStretch(contourImage, "Contour image");
		show16SImageStretch(bbsImage, "Bbs image");

		// Save bounding boxed images to dir.
		BoundingBoxer::saveBoundingBoxImages(image, boundingboxStructs, R"(Res)", "Harten");

		cv::waitKey(0);
	}
}

void TestClass::testFloodFill()
{
	// Read in image and diplay it
	cv::Mat image;
	std::vector<std::vector<cv::Point>> contourVector;
	/*ImageLoader::loadImageFromPath(image,
		R"(Res\testImg.png)");*/
	ImageLoader::loadImageFromPath(image,
		R"(Res\mix2.png)");
	if (!image.data)
	{
		std::cout << "Could not open file" << std::endl;

	}
	else
	{
		
		cv::Mat binaryImage;
		// Convert to 16 bit
		cvtColor(image, binaryImage, CV_BGR2GRAY);
		binaryImage.convertTo(binaryImage, CV_16S);

		// Invert the image since the source is inverted
		//threshold(binaryImage, binaryImage, 1, 1, cv::THRESH_BINARY_INV);

		threshold(binaryImage, binaryImage, 100, 1, cv::THRESH_BINARY_INV);
		show16SImageStretch(binaryImage, "Binary image");

		// Get contours and display them :D
		cv::Mat contourImage = cv::Mat::zeros(binaryImage.rows, binaryImage.cols, binaryImage.type());
		std::vector<std::vector<cv::Point>> contourPoints;
		MooreBoundaryTracer::getContours(binaryImage, contourPoints);
		MooreBoundaryTracer::generateBoundaryImage(contourImage, contourPoints);
		show16SImageStretch(contourImage, "Contour image");

		std::vector<std::vector<cv::Point>> allBlobsRegionPixels;

		for (int contours = 0; contours < contourPoints.size(); contours++)
		{
			std::vector<cv::Point> regionPixels;
			FloodFill::getEnclosedPixels(binaryImage, contourPoints[contours], regionPixels, true);
			allBlobsRegionPixels.push_back(regionPixels);
			regionPixels.clear();
		}

		FloodFill::saveEnclosedPixelsImages(image, allBlobsRegionPixels, R"(Res)", "mixedClosedPixels");
		//BoundingBoxer::saveBoundingBoxImages(image, boundingboxStructs, R"(Res)", "harten");
		// Test boundary fill
		
	}
}



