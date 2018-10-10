#pragma once
#include <opencv2/core/mat.hpp>

class BendingEnergy
{
public:
	BendingEnergy();
	~BendingEnergy();
	
	/// <summary>
	/// Gets the bending energy of the contours.
	/// </summary>
	/// <param name="chainCodes">The chain codes</param>
	/// <returns></returns>
	static std::vector<int> getBendingEnergy(const std::vector<std::vector<int>>& chainCodes);

	/// <summary>
	/// Gets the bending energy index of 8 dimension freeman chaincode counter clockwise.
	/// </summary>
	/// <returns></returns>
	static int getBendingEnergyCounterClockwise(int previous, int current);

	/// <summary>
	/// Gets the chain code of all contours.
	/// </summary>
	/// <param name="">The .</param>
	/// <returns></returns>
	static std::vector<std::vector<int>> getChainCode(const cv::Mat&);
};

