#include <bringauto/log_evaluator/Filter.hpp>

#include <boost/algorithm/string.hpp>

#include <string>

namespace bringauto::log_evaluator {

std::string Filter::findNextTransitionLog(std::istream& srcFile) {
	std::string line;
	for (int i = 0; std::getline(srcFile, line); i++) {
		if (boost::find_first(line,"[TransitionSmurf]")) {
            return line;
		}
	}
    return "";
}

}