#pragma once

#include <fstream>

namespace bringauto {

class Filter {
public:
	static std::string  findNextTransitionLog(std::istream& srcFile);
};
}
