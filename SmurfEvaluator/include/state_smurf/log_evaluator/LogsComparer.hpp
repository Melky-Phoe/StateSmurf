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
    /**
     * Compares strings given by parameters word by word. Doesn't compare date and time in logs
     * @param etalon line from etalon
     * @param compared line compared with etalon
     * @return true if same
     */
    static bool compareLines(const std::string& etalon, const std::string& compared);

    /**
     * Splits line into Tokens (words)
     * @param line string
     * @return vector containing tokens
     */
    static std::vector<std::string> parseLine(const std::string& line);

    static bool isStartOfRunLog(const std::string& line);

    static bool validateEtalon(std::vector<std::string> etalonLogs);

    /**
     * Enum for addressing tokens created by parseLine
     */
    enum class LogTokensIndexes {
        date = 0,
        time = 1,
        appName = 2,
        verbosity = 3,
        transitionSmurf = 4,
        inOrGoing = 5,
        inState = 7,
        goingState = 8
    };
};
}