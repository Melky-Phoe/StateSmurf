#pragma once
#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
class LogsComparer {
public:
    /**
     * Compares [TransitionSmurf] logs of files given by param, writes differences on stdout
     * @param etalonFile log file, that we take as correct one
     * @param comparedFile tested log file
     * @return true if StateTransition logs are same, false otherwise
     */
    static bool compareFiles(std::istream& etalonFile, std::istream& comparedFile);

private:
	/**
	 * Check if line is Start of Run in aggregated file
	 * @param line
	 * @return
	 */
    static bool isStartOfRunLog(const std::string& line);

	/**
	 * Check if all logs of etalon are valid. Checks count of runs, unsuccessful transitions
	 * @param etalonLogs filtered vector
	 * @return
	 */
    static bool validateEtalon(std::vector<std::string> etalonLogs);
	
};
}