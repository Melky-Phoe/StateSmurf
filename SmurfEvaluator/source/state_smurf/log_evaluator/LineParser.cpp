#include <state_smurf/log_evaluator/LineParser.hpp>

#include <boost/algorithm/string.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {

std::vector<std::string> LineParser::parseLine(std::string line) {
	std::vector<std::string> tokens;
	boost::trim(line);
	boost::split(tokens, line, boost::is_any_of(" \t\n"));
	return tokens;
}

bool LineParser::compareLines(const std::string &etalon, const std::string &compared) {
	std::vector<std::string> etalonTokens = parseLine(etalon);
	std::vector<std::string> comparedTokens = parseLine(compared);
	
	bool logsAreSame = true;
	if (etalonTokens.size() > 3 && etalonTokens[1] == "circuit") { // circuit log
		for (int i = 0; i < 3; ++i) {
			if (etalonTokens[i] != comparedTokens[i]) {
				logsAreSame = false;
			}
		}
	} else {    // normal StateTransition log
		for (int i = static_cast<int>(LogTokensIndexes::appName);
		     i < std::max(etalonTokens.size(), comparedTokens.size()); ++i) {
			if (etalonTokens[i] != comparedTokens[i]) {
				logsAreSame = false;
			}
		}
	}
	if (!logsAreSame) {
		std::cout << "Logs aren't equal:\n"
		             "  Etalon: " << etalon << std::endl <<
					 "  Compared: " << compared << std::endl;
		return false;
	} else {
		return true;
	}
}
	
	std::string LineParser::getState(const std::string &line) {
		std::vector<std::string> tokens = parseLine(line);
		if (tokens[static_cast<unsigned long>(LogTokensIndexes::inOrGoing)] != "Start") {
			return tokens[static_cast<unsigned long>(LogTokensIndexes::goingState)];
		} else {
			return "";
		}
	}
}