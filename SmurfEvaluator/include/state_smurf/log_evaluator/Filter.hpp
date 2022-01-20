#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
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
     * Filters .log file and creates vector of [TransitionSmurf] logs
     * @param srcFile .log file
     * @return filtered out logs containing only TranstionSmurf logs
     */
    static std::vector<std::string> createTransitionLogVector(std::istream& srcFile);
};
}
