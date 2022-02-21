#include <state_smurf/log_evaluator/Filter.hpp>

#include <boost/algorithm/string.hpp>

#include <string>

namespace state_smurf::log_evaluator {

std::string Filter::findNextTransitionLog(std::istream& srcFile) {
	std::string line;
	for (int i = 0; std::getline(srcFile, line); i++) {
		if (boost::find_first(line,"[TransitionSmurf]")) {
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
			if (boost::find_first(line,"[DiagramSmurf]")) {
				return line;
			} else if (boost::find_first(line,"[TransitionSmurf]")) {
				return "";
			}
		}
		return "";
	}
	
}