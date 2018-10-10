// Demo: Training of a Neural Network / Back-Propagation algorithm 
// Jan Oostindie, Avans Hogeschool, dd 6-12-2016
// email: jac.oostindie@avans.nl

//#include <opencv2/opencv.hpp>
#include <opencv2/core/core.hpp>
#include <opencv2/highgui/highgui.hpp>
#include <opencv/cv.h>
#include <iostream>
#include <string>
#include <iomanip>
#include "avansvisionlib20.h" // versie 2.0 (!)
#include "CsvParser.h"


// Maximale fout die toegestaan wordt in de output voor de training input
const double MAX_OUTPUT_ERROR = 1E-10;

// maximaal aantal runs dat uitgevoerd wordt bij het trainen
const int MAXRUNS = 10000;

int main(int argc, char** argv)
{
	// IT, OT: input trainingset, output trainingset
	cv::Mat_<double> ITset, OTset;
	csvColums csvColums = CsvParser::csvToTrainingSet(R"(c:\Programming Projects\Advanced-Vision-Cpp\AdvancedVision\NeuralNetworkTest\Res\banknotes.csv)", 4, ITset, OTset);

	// Example of how to use write to Csv
	CsvParser::writeToCsv(R"(c:\Programming Projects\Advanced-Vision-Cpp\AdvancedVision\NeuralNetworkTest\Res\banknotesResult.csv)", csvColums, ITset, OTset,true, true);
	// V0, W0   : weightfactor matrices
	// dV0, dW0 : weightfactor correction matrices
	cv::Mat V0, W0, dW0, dV0;

	// default number of hiddenNeurons. The definite number is user input  
	// inputNeurons and outputNeurons are implicitly determined via
	// the trainingset, i.e.: inputNeurons = ITset.cols ; outputNeurons = OTset.cols;
	int hiddenNeurons = 2;

	std::cout << std::endl << "Load testtrainingset..." << std::endl << std::endl;

	//loadTrainingSet1(ITset, OTset);
	//loadBinaryTrainingSet1(ITset, OTset);

	std::cout << "Training Input " << std::endl << std::endl;
	std::cout << ITset << std::endl << std::endl;
	std::cout << "Training Output " << std::endl << std::endl;
	std::cout << OTset << std::endl << std::endl;

	std::cout << " ===> BPN format: " << std::endl <<
		"BPN Inputlayer  = " << ITset.cols << "  neurons" << std::endl <<
		"BPN Outputlayer = " << OTset.cols << "  neurons" << std::endl << std::endl;
	std::cout << "Please choose a number of hidden neurons: ";
	std::cin >> hiddenNeurons;
	std::cout << "Thank you!" << std::endl << std::endl << std::endl;

	std::cout << "Initialize BPN ..." << std::endl;
	initializeBPN(ITset.cols, hiddenNeurons, OTset.cols, V0, dV0, W0, dW0);
	//testBPN(ITset, OTset, V0, dV0, W0, dW0);

	std::cout << "initial values of weight matrices V0 and W0" << std::endl;
	std::cout << "*******************************************" << std::endl;
	std::cout << V0 << std::endl << std::endl << W0 << std::endl << std::endl;
	std::cout << "Press ENTER => ";
	std::string dummy;
	getline(std::cin, dummy);
	getline(std::cin, dummy);

	// IT: current training input of the inputlayer 
	// OT: desired training output of the BPN
	// OH: output of the hiddenlayer
	// OO: output of the outputlayer
	cv::Mat IT, OT, OH, OO;

	// outputError0: error on output for the current input and weighfactors V0, W0
	// outputError1: error on output for the current input and new calculated 
	//               weighfactors, i.e. V1, W1
	double outputError0, outputError1, sumSqrDiffError = MAX_OUTPUT_ERROR + 1;
	cv::Mat V1, W1;

	int runs = 0;
	while ((sumSqrDiffError > MAX_OUTPUT_ERROR) && (runs < MAXRUNS)) {

		sumSqrDiffError = 0;

		for (int inputSetRowNr = 0; inputSetRowNr < ITset.rows; inputSetRowNr++) {

			IT = transpose(getRow(ITset, inputSetRowNr));

			OT = transpose(getRow(OTset, inputSetRowNr));

			calculateOutputHiddenLayer(IT, V0, OH);

			calculateOutputBPN(OH, W0, OO);

			adaptVW(OT, OO, OH, IT, W0, dW0, V0, dV0, W1, V1);

			calculateOutputBPNError(OO, OT, outputError0);

			calculateOutputBPNError(BPN(IT, V1, W1), OT, outputError1);

			sumSqrDiffError += (outputError1 - outputError0) * (outputError1 - outputError0);

			V0 = V1;
			W0 = W1;
		}
		std::cout << "sumSqrDiffError = " << sumSqrDiffError << std::endl;
		runs++;
	}

	std::cout << "BPN Training is ready!" << std::endl << std::endl;
	std::cout << "Runs = " << runs << std::endl << std::endl;

	cv::Mat inputVectorTrainingSet, outputVectorTrainingSet, outputVectorBPN;

	// druk voor elke input vector uit de trainingset de output vector uit trainingset af 
	// tezamen met de output vector die het getrainde BPN (zie V0, W0) genereerd bij de 
	// betreffende input vector.
	std::cout << std::setw(16) << " " << "Training Input" << std::setw(12) << "|" << " Expected Output "
		<< std::setw(1) << "|" << " Output BPN " << std::setw(6) << "|" << std::endl << std::endl;
	for (int row = 0; row < ITset.rows; row++) {

		// haal volgende inputvector op uit de training set
		inputVectorTrainingSet = transpose(getRow(ITset, row));

		// druk de inputvector af in een regel afgesloten met | 
		for (int r = 0; r < inputVectorTrainingSet.rows; r++)
			std::cout << std::setw(8) << getEntry(inputVectorTrainingSet, r, 0);
		std::cout << std::setw(2) << "|";

		// haal bijbehorende outputvector op uit de training set
		outputVectorTrainingSet = transpose(getRow(OTset, row));

		// druk de outputvector van de training set af in dezelfde regel afgesloten met | 
		for (int r = 0; r < outputVectorTrainingSet.rows; r++)
			std::cout << std::setw(8) << round(getEntry(outputVectorTrainingSet, r, 0));
		std::cout << std::setw(2) << "|";

		// bepaal de outputvector die het getrainde BPN oplevert 
		// bij de inputvector uit de trainingset  
		outputVectorBPN = BPN(inputVectorTrainingSet, V0, W0);

		// druk de output vector van het BPN af in dezelfde regel afgesloten met |
		for (int r = 0; r < outputVectorBPN.rows; r++)
			std::cout << std::setw(8) << round(getEntry(outputVectorBPN, r, 0));
		std::cout << std::setw(2) << "|";

		std::cout << std::endl;
	}

	std::cout << std::endl << std::endl << "Press ENTER for exit";
	getline(std::cin, dummy);
	getline(std::cin, dummy);

	return 0;
}

