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
    static bool isStartOfRunLog(const std::string& line);

    static bool validateEtalon(std::vector<std::string> etalonLogs);
	
};
}