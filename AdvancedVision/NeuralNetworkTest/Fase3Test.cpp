#include "Fase3Test.h"
#include <opencv2/core/mat.hpp>
#include <opencv2/imgproc/types_c.h>
#include <opencv2/shape/hist_cost.hpp>
#include "../AdvancedVision/MooreBoundaryTracer.h"
#include "../AdvancedVision/ImageLoader.h"


Fase3Test::Fase3Test()
{
}


Fase3Test::~Fase3Test()
{
}



void Fase3Test::HuMomentsTest()
{
	cv::Mat image;

	ImageLoader::loadImageFromPath(image, R"(Res\NeuralTestSets\Hearts_0.jpg)");

	cv::Mat binaryImage;
	// Convert to 16 bit
	cvtColor(image, binaryImage, CV_BGR2GRAY);
	binaryImage.convertTo(binaryImage, CV_16S);
	std::vector<std::vector<cv::Point>> contourPoints;
	MooreBoundaryTracer::getContours(binaryImage, contourPoints);

	cv::Moments mom = cv::moments(contourPoints[0]);
	double huMoments[7];
	cv::HuMoments(mom, huMoments);



}

