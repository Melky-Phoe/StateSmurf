#pragma once
#include "Filter.h"
#include <fstream>
#include <vector>

namespace bringauto::logEvaluator {
class LogsComparer {
public:
    static int compareFiles(std::istream& etalon, std::istream& compared);
    static int compareLines(const std::string& etalon, const std::string& compared);
    static std::vector<std::string> parseLine(const std::string& line);

private:
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