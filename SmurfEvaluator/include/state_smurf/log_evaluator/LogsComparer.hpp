#pragma once

#include <fstream>
#include <vector>
#include "CircuitAggregator.hpp"



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
		static bool compareFiles(std::ifstream &etalonFile, std::ifstream &comparedFile, const std::string& saveAggregatedPath);
	
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

		static std::vector<std::string>
		loadLogs(std::ifstream &file, CircuitAggregator &circuitAggregator);

	};
}