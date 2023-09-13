#pragma once

#include <vector>
#include <string>

namespace state_smurf::log_evaluator::helpers {

	class FileHelper {
	public:
		/**
		 * Creates directories in path, where aggregated files will be created.
		 * etalonLogs are written to file 'etalon' and comparedLogs to file 'compared'
		 * @param aggregatedLogs vector of aggregated logs
		 * @param fileName file name
		 * @param path directory, where the file fileName will be created
		 * @return true if successful
		 */
		static bool createAggregatedFiles(const std::vector<std::string>& aggregatedLogs, const std::string &fileName, const std::string& path);


		static void writeVectorToFile(const std::vector<std::string>& sourceLogs, std::ofstream* outputStream);


		static bool isFileAggregated(std::istream &file);

		static std::vector<std::string> createVectorFromFile(std::istream &file);
	};
}
