#pragma once

#define WEBCAM 0
#define EXTERNAL_CAM 1

class Camera
{
public:
	Camera(int cameraId = 0);
	~Camera();
	
	/// <summary>
	/// Captures the image.
	/// </summary>
	static void captureImage();

	/// <summary>
	/// Displays the image
	/// </summary>
	void displayImage() const;

};

