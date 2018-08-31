#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>


Camera::Camera()
{
	WebCamCaputure = cv::VideoCapture(WEBCAM);
}


Camera::~Camera()
{
}

void Camera::CaptureImage()
{
	WebCamCaputure.read(CapturedImage);
}

void Camera::DisplayImage() const
{
	cv::imshow("camera", CapturedImage);;
}

