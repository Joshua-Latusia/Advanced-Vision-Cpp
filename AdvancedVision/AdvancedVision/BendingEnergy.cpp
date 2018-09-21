#include "BendingEnergy.h"
#include <opencv2/flann.hpp>
#include <opencv2/videostab/ring_buffer.hpp>


BendingEnergy::BendingEnergy()
{
}


BendingEnergy::~BendingEnergy()
{
}

std::vector<std::vector<int>> BendingEnergy::getChainCode(const cv::Mat& image)
{
	std::vector<std::vector<int>> chainCodes;

	CvChain* chain = 0;
	CvMemStorage* storage = 0;
	storage = cvCreateMemStorage(0);
	cvFindContours(&IplImage(image), storage, reinterpret_cast<CvSeq**>(&chain), sizeof(*chain), CV_RETR_EXTERNAL, CV_CHAIN_CODE);
	for (; chain != NULL; chain = reinterpret_cast<CvChain*>(chain->h_next))
	{
		CvSeqReader reader;
		int i, total = chain->total;
		cvStartReadSeq(reinterpret_cast<CvSeq*>(chain), &reader, 0);

		// Constructing single chaincode of one contour
		printf("\n--------------------chain\n"); // Debugging
		std::vector<int> chainCode;
		for (i = 0; i<total; i++)
		{
			char code;
			CV_READ_SEQ_ELEM(code, reader);
			printf("%d", code); // Debugging
			chainCode.push_back(int(code));
		}
		chainCodes.push_back(chainCode);
	}
	return chainCodes;
}

std::vector<int> BendingEnergy::getBendingEnergy(const std::vector<std::vector<int>>& chainCodes)
{
	std::vector<int> bendingEnergies;

	for (std::vector<int> chain : chainCodes)
	{
		int bendingEnergy = 0;
		const size_t chainsize = chain.size();

		// Skip first element of chain
		for (int i = 0; i < chainsize; i++)
		{
			const int previous = chain.at(i == 0 ? chainsize - 1 : i - 1);
			const int current = chain.at(i);

			// get bending energy counterclockwise
			int counterClockwiseEnergy = getBendingEnergyCounterClockwise(previous, current);

			// get bending energy other clockwise ( = 8sides - counterClockwise)
			int clockwiseEnergy = 8 - counterClockwiseEnergy;
			
			// add the smallest to the list
			bendingEnergy += clockwiseEnergy <= counterClockwiseEnergy ? clockwiseEnergy : counterClockwiseEnergy;
		}
		bendingEnergies.push_back(bendingEnergy);
	}
	
	return bendingEnergies;
}

int BendingEnergy::getBendingEnergyCounterClockwise(int previous, int current)
{
	int index = 0;
	while ((previous + index) % 8 != current)
	{
		index++;
	}
	return index;
}

