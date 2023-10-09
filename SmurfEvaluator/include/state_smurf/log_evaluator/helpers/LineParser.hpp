#pragma once

#include <string>
#include <vector>

namespace state_smurf::log_evaluator::helpers {
	/**
	 * Helper functions for parsing lines into tokens. Uses Boost library
	 */
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
		static std::vector<std::string> parseLine(std::string line);
		
		/**
		 * Gets destination state from TransitionSmurf Log
		 * @param line string containing TransitionSmurf log
		 * @return name of state
		 */
		static std::string getState(const std::string &line);
		
		/**
		 * Gets date and time from log line
		 * @param line log
		 * @return time in format: [YYYY-MM-DD hh:mm:ss.ms]
		 */
		static std::string getTime(const std::string &line);
		
		/**
		 * Split line by whitespaces
		 * @param tokens pointer to vector where words will be saved
		 * @param line string that will be split
		 */
		static void splitString(std::vector<std::string> &tokens, const std::string &line);
		
		/**
		 * Enum for addressing tokens created by parseLine
		 */
		enum class LogTokensIndexes {
			DATE = 0,
			TIME = 1,
			APP_NAME = 2,
			VERBOSITY = 3,
			TRANSITION_SMURF_PREFIX = 4,
			START_OR_GOING = 5,
			STATE = 8
		};
	};
	
}