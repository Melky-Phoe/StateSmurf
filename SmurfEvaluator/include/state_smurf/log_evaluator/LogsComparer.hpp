#pragma once
#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
class LogsComparer {
public:
    /**
     * Compares [TransitionSmurf] logs of files given by param, writes differences on stdout
     * @param etalon log file, that we take as correct one
     * @param compared tested log file
     * @return true if StateTransition logs are same, false otherwise
     */
    static bool compareFiles(std::istream& etalon, std::istream& compared);

private:
    static bool isStartOfRunLog(const std::string& line);

    static bool validateEtalon(std::vector<std::string> etalonLogs);
	
};
}