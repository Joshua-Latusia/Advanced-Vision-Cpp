#pragma once
#include <string>
#include <opencv2/core/mat.hpp>

/*Defines for the banknotes CSV*/
#define VARIANCE_INDEX 0
#define SKEWNESS_INDEX 1
#define CURTOSIS_INDEX 2
#define ENTROPY_INDEX 3
#define CLASS_INDEX 4

namespace cv {
	class Mat;
}

/// <summary>
/// Struct for storing Csv specific data
/// </summary>
struct csvColums
{	
	/// <summary>
	/// The column names of the csv
	/// </summary>
	std::vector<std::string> columnNames;
		
	/// <summary>
	/// The amount of feature colums
	/// </summary>
	int featureColums;
	
	/// <summary>
	/// The amount of output colums
	/// </summary>
	int outputColumns;
	
	/// <summary>
	/// Determines whether the colums are valid by checking amount of columnames with the gives feature and output colums.
	/// </summary>
	/// <returns>
	///   <c>true</c> if this instance is valid; otherwise, <c>false</c>.
	/// </returns>
	bool isValid() const
	{
		return static_cast<int>(columnNames.size()) == featureColums + outputColumns && featureColums > 0 && outputColumns > 0;
	}

};

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

class CsvParser
{
public:
	CsvParser();
	~CsvParser();
	
	/// <summary>
	/// Converts a banknotes csv file to a trainingset
	/// Converts the all the values that are not the last value in a row to the inputSet row
	/// Sets the last value to the output set row
	/// </summary>
	/// <param name="path">The path of the file.</param>
	/// <param name="inputSet">The input set.</param>
	/// <param name="outputSet">The output set.</param>
	static void bankNotesCsvtoTrainingSet(const std::string& path, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet);
	
	/// <summary>
	/// Converts a csv file to a trainingset.
	/// In the csv the first row has to be the names of 
	/// </summary>
	/// <param name="path">The path of the file.</param>
	/// <param name="featureColums"> Amount of colums that are a feature.</param>
	/// <param name="inputSet">The input set.</param>
	/// <param name="outputSet">The output set.</param>
	static csvColums csvToTrainingSet(const std::string& path, const int featureColumns, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet);
	
	/// <summary>
	/// Writes set to CSV.
	/// Will display error if file is already existing but trying to overwrite
	/// </summary>
	/// <param name="path">The path of where the file will be saved and created.</param>
	/// <param name="csvColums">The CSV colums struct.</param>
	/// <param name="completeSet">The whole set of data to write.</param>
	/// <param name="removeBias">if set to <c>true</c> will remove bias column from the set (first col) . default = false</param>
	static void writeToCsv(const std::string& path, csvColums& csvColums, cv::Mat_<double>& completeSet, bool overWriteExisting = false, bool removeBias = false);
	
	/// <summary>
	/// Writes set to CSV by callin.
	/// Will display error if file is already existing but trying to overwrite
	/// <param name="path">The path of where the file will be saved and created.</param>
	/// <param name="csvColums">The CSV colums struct.</param>
	/// <param name="inputSet">The input set.</param>
	/// <param name="outputSet">The output set.</param>
	/// <param name="overWriteExisting">if set to <c>true</c> will overwrite existing files. default = false</param>
	/// <param name="removeBias">if set to <c>true</c> will remove bias column from the inputset. default = false</param>
	static void writeToCsv(const std::string& path, csvColums & csvColums, cv::Mat_<double>& inputSet, cv::Mat_<double>& outputSet, bool overWriteExisting = false, bool removeBias = false);
};

