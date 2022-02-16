#pragma once

#include <string>
#include <vector>

namespace state_smurf::log_evaluator {

class LineParser {
public:
	/**
	 * Compares strings given by parameters word by word. Doesn't compare date and time in logs
	 * @param etalon line from etalon
	 * @param compared line compared with etalon
	 * @return true if same
	 */
	static bool compareLines(const std::string &etalon, const std::string &compared);
	
	/**
	 * Splits line into Tokens (words)
	 * @param line string
	 * @return vector containing tokens
	 */
	static std::vector<std::string> parseLine(const std::string &line);

	/**
	 * Gets destination state from TransitionSmurf Log
	 * @param line string containing TransitionSmurf log
	 * @return name of state
	 */
	static std::string getState(const std::string &line);
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