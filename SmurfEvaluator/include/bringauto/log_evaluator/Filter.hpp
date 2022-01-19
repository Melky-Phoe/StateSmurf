#pragma once

#include <fstream>
#include <vector>

namespace bringauto::log_evaluator {
/**
 * Class implementing filtering of .log files
 */
class Filter {
public:
    /**
     * Finds log containing prefix [TransitionSmurf] and returns whole line
     * all other logs are filtered out
     * @param srcFile .log file
     * @return line as string
     */
	static std::string findNextTransitionLog(std::istream& srcFile);

    /**
     * Filters .log file
     * @param srcFile .log file
     * @return
     */
    static std::vector<std::string> createTransitionLogVector(std::istream& srcFile)
};
}
