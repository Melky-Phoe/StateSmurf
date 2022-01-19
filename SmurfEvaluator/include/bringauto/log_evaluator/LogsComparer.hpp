#pragma once
#include <fstream>
#include <vector>

namespace bringauto::log_evaluator {
class LogsComparer {
public:
    /**
     * Compares [StateTransition] logs of files given by param, writes differences on stdout
     * @param etalon log file, that we take as correct one
     * @param compared tested log file
     * @return true if StateTransition logs are same, false otherwise
     */
    static bool compareFiles(std::istream& etalon, std::istream& compared);

private:
    static bool compareLines(const std::string& etalon, const std::string& compared);
    static std::vector<std::string> parseLine(const std::string& line);
    enum class LogTokensIndexes {
        date = 0,
        time = 1,
        appName = 2,
        verbosity = 3,
        stateTransition = 4,
        inOrGoing = 5,
        inState = 7,
        goingState = 8
    };
};
}