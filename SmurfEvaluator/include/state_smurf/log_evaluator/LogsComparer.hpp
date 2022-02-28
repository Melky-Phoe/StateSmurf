#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
	class LogsComparer {
	public:
		/**
		 * Compares logs of files containing TransitionSmurf logs given by param, writes differences on stdout
		 * @param etalonFile file, that we presume as correct one
		 * @param comparedFile compared file
		 * @param saveAggregatedPath path to directory where aggregated files will be saved, if empty, files are not saved
		 * @return true if StateTransition logs are valid and same, false otherwise
		 */
		static bool compareFiles(std::istream &etalonFile, std::istream &comparedFile, std::string saveAggregatedPath);
	
	private:
		/**
		 * Check if line is Start of Run - Aggregated
		 * @param line
		 * @return
		 */
		static bool isStartOfRunLog(const std::string &line);
		
		/**
		 * Check if all logs of etalon are valid. Checks count of runs, unsuccessful transitions
		 * @param etalonLogs filtered vector
		 * @return
		 */
		static bool validateEtalon(std::vector<std::string> etalonLogs);
		
		/**
		 * Creates directories in path, where aggregated files will be created.
		 * etalonLogs are written to file 'etalon' and comparedLogs to file 'compared'
		 * @param etalonLogs vector of aggregated etalon logs
		 * @param comparedLogs vector of aggregated comparedFile logs
		 * @param path directory, where files `etalon` and `compared` will be created
		 * @return true if successful
		 */
		static bool saveAggregatedFiles(const std::vector<std::string>& etalonLogs, const std::vector<std::string>& comparedLogs, const std::string& path);
		
	};
}