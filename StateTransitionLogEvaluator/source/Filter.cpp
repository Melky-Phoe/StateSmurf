#include <Filter.h>

#include <boost/algorithm/string.hpp>

#include <string>
#include <sstream>
#include <iostream>

namespace bringauto {

std::string Filter::findNextTransitionLog(std::istream& srcFile) {
	//std::stringstream filteredLogs;
	std::string line;
	for (int i = 0; std::getline(srcFile, line); i++) {
		// boost::iterator_range<std::string::iterator> result=boost::find_first(line,"ll");
		if (boost::find_first(line,"[StateTransition]")) {
            return line;
		}
	}
    return "";
}

}