#pragma once

#include <fstream>

namespace bringauto::log_evaluator {

class Filter {
public:
	static std::string  findNextTransitionLog(std::istream& srcFile);
};
}
