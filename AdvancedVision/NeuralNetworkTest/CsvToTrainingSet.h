#pragma once
#include <string>
#include <opencv2/core/mat.hpp>

#define VARIANCE_INDEX 0
#define SKEWNESS_INDEX 1
#define CURTOSIS_INDEX 2
#define ENTROPY_INDEX 3
#define CLASS_INDEX 4

namespace cv {
	class Mat;
}

/// <summary>
/// Struct for spliting strings with commas into vectors
/// </summary>
struct tokens : std::ctype<char>
{
	tokens() : std::ctype<char>(get_table()) {}

	static std::ctype_base::mask const* get_table()
	{
		typedef std::ctype<char> cctype;
		static const cctype::mask *const_rc = cctype::classic_table();

		static cctype::mask rc[cctype::table_size];
		std::memcpy(rc, const_rc, cctype::table_size * sizeof(cctype::mask));

		rc[','] = std::ctype_base::space;
		return &rc[0];
	}
};

class CsvToTrainingSet
{
public:
	CsvToTrainingSet();
	~CsvToTrainingSet();
	
	/// <summary>
	/// Converts a csv file to a trainingset
	/// Converts the all the values that are not the last value in a row to the inputSet row
	/// Sets the last value to the output set row
	/// </summary>
	/// <param name="path">The path of the file.</param>
	/// <param name="inputSet">The input set.</param>
	/// <param name="outputSet">The output set.</param>
	static void toTrainingSet(const std::string path, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet);
};

