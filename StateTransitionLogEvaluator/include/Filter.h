#pragma once

#include <fstream>

namespace bringauto {

class Filter {
public:
	static void filterStateTransitionLogs(std::istream& etalonFile);
};
}
