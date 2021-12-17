#include <Filter.h>

#include <boost/algorithm/string.hpp>

#include <string>
#include <sstream>
#include <iostream>

namespace bringauto {

void Filter::filterStateTransitionLogs(std::istream& srcFile) {
	std::stringstream filteredLogs;
	std::string line;
	for (int i = 0; std::getline(srcFile, line); i++) {
		// boost::iterator_range<std::string::iterator> result=boost::find_first(line,"ll");
		if (boost::find_first(line,"[StateTransition]")) {
			filteredLogs << i << ":\t" << line << std::endl;
		}
	}
	std::cout << filteredLogs.str();
}

}