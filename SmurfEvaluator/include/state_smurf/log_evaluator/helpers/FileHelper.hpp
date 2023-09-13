#pragma once

#include <vector>
#include <string>

namespace state_smurf::log_evaluator::helpers {

	class FileHelper {
	public:
		/**
		 * Creates directories in path, where aggregated files will be created. Then create file and fill it with aggregatedLogs
		 * @param aggregatedLogs vector of aggregated logs
		 * @param fileName file name
		 * @param path directory, where the file fileName will be created
		 * @return true if successful
		 */
		static bool createAggregatedFiles(const std::vector<std::string>& aggregatedLogs, const std::string &fileName, const std::string& path);

		/**
		 * @brief Puts all logs into file
		 * @param sourceLogs logs that will be saved
		 * @param outputStream output file stream
		 */
		static void writeVectorToFile(const std::vector<std::string>& sourceLogs, std::ofstream* outputStream);

		/**
		 * @brief Reads whole file and puts each line into vector
		 * @param file input stream
		 * @return vector of lines
		 */
		static std::vector<std::string> createVectorFromFile(std::istream &file);

		/**
		 * @brief Check if a file is already aggregated.
		 * @param file
		 * @return true if aggregated, false otherwise
		 */
		static bool isFileAggregated(std::istream &file);
	};
}
