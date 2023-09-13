#include "state_smurf/log_evaluator/helpers/Filter.hpp"

#include <string>

namespace state_smurf::log_evaluator::helpers {
	
	std::string Filter::findNextTransitionLog(std::istream &srcFile) {
		std::string line;
		for (int i = 0; std::getline(srcFile, line); i++) {
			std::size_t found = line.find("[TransitionSmurf]");
			if (found != std::string::npos) {
				return line;
			}
		}
		return "";
	}
	
	std::vector<std::string> Filter::createTransitionLogVector(std::istream &srcFile) {
		std::vector<std::string> filteredLogs;
		std::string line = findNextTransitionLog(srcFile);
		while (!line.empty()) {
			filteredLogs.push_back(line);
			line = findNextTransitionLog(srcFile);
		}
		return filteredLogs;
	}
	
	std::string Filter::findDiagramSmurfLog(std::istream &srcFile) {
		std::string line;
		for (int i = 0; std::getline(srcFile, line); i++) {
			std::size_t found = line.find("[DiagramSmurf]");
			if (found != std::string::npos) {
				return line;
			} else if (line.find("[TransitionSmurf]") != std::string::npos) {
				return "";
			}
		}
		return "";
	}
	
}