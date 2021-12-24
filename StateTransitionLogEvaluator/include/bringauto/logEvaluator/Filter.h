#pragma once

#include <fstream>

namespace bringauto::logEvaluator {

class Filter {
public:
	static std::string  findNextTransitionLog(std::istream& srcFile);
};
}
