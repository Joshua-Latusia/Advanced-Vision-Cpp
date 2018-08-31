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
	void CaptureImage();

	/// <summary>
	/// Displays the image
	/// </summary>
	void DisplayImage() const;


	/// <summary>
	/// The most recent captured image
	/// </summary>
	cv::Mat CapturedImage;
	
	cv::VideoCapture WebCamCaputure;
};

