
	/// <summary>
	/// Gets the pixels inside of a single contour.
	/// </summary>
	/// <param name="image">The original image for the img dimensions.</param>
	/// <param name="boundaryVec">The boundary vec.</param>
	/// <param name="regionPixels">The region pixels.</param>
	/// <param name="animation">if set to <c>true</c> [animation].</param>
	/// <returns></returns>
	static int getEnclosedPixels(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels, bool animation = false);
	static void fillImageEightConnected(const cv::Mat& image, const std::vector<cv::Point>& boundaryVec, std::vector<cv::Point>& regionPixels, bool animation = false);


	/// <summary>
	/// Saves the enclosed pixels images.
	/// </summary>
	/// <param name="image">The image.</param>
	/// <param name="enclosedPixels">The enclosed pixels.</param>
	/// <param name="path">The path.</param>
	/// <param name="filename">The filename.</param>
	/// <param name="fileExtension">The file extension.</param>
	/// <returns></returns>
	static bool saveEnclosedPixelsImages(const cv::Mat & image, std::vector<std::vector<cv::Point>>& enclosedPixels, const std::string & path,
		const std::string & filename, const std::string & fileExtension = ".jpg");


	/// <summary>
	/// Gets the extremes.
	/// </summary>
	/// <param name="enclosedPixels">The enclosed pixels.</param>
	/// <param name="extremesStruct">The extremes structure.</param>
	static void getExtremes(std::vector<cv::Point>& enclosedPixels, FloodFillExtremesStruct & extremesStruct);