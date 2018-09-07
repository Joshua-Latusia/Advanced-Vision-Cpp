#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>


Camera::Camera()
{
	webCamCaputure = cv::VideoCapture(WEBCAM);
}


Camera::~Camera()
{
}

void Camera::captureImage()
{
	webCamCaputure.read(capturedImage);
}

void Camera::displayImage() const
{
	cv::imshow("camera", capturedImage);
}

