#include "Camera.h"
#include <opencv2/highgui/highgui.hpp>

/// <summary>
/// The most recent captured image
/// </summary>
cv::Mat capturedImage;

/// <summary>
/// The web cam caputure
/// </summary>
cv::VideoCapture webCamCaputure;

Camera::Camera(int cameraId)
{
	webCamCaputure = cv::VideoCapture(cameraId);
}


Camera::~Camera()
{
}

void Camera::captureImage()
{
	webCamCaputure >> capturedImage;
}

void Camera::displayImage() const
{
	imshow("camera", capturedImage);
}

