#include <bringauto/logEvaluator/Filter.h>

#include <boost/algorithm/string.hpp>

#include <string>

namespace bringauto::logEvaluator {

std::string Filter::findNextTransitionLog(std::istream& srcFile) {
	std::string line;
	for (int i = 0; std::getline(srcFile, line); i++) {
		if (boost::find_first(line,"[StateTransition]")) {
            return line;
		}
	}
    return "";
}

}