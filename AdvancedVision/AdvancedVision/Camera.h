#pragma once
#include <opencv2/core/mat.hpp>
#include <opencv2/videoio.hpp>

#define WEBCAM 0

class Camera
{
public:
	Camera();
	~Camera();
	
	/// <summary>
	/// Captures the image.
	/// </summary>
	void captureImage();

	/// <summary>
	/// Displays the image
	/// </summary>
	void displayImage() const;


	/// <summary>
	/// The most recent captured image
	/// </summary>
	cv::Mat capturedImage;
	
	cv::VideoCapture webCamCaputure;
};

