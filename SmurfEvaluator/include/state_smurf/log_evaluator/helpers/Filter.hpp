#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator::helpers {
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
		static std::string findNextTransitionLog(std::istream &srcFile);
		
		/**
		 * Finds log containing prefix [DiagramSmurf] and returns whole line
		 * all other logs are filtered out. If [TransitionSmurf] log is found, returns empty string
		 * @param srcFile .log file
		 * @return line as string
		 */
		static std::string findDiagramSmurfLog(std::istream &srcFile);
		
		/**
		 * Filters .log file and creates vector of [TransitionSmurf] logs
		 * @param srcFile .log file
		 * @return filtered out logs containing only TransitionSmurf logs
		 */
		static std::vector<std::string> createTransitionLogVector(std::istream &srcFile);
	};
}
