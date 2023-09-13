#include "state_smurf/log_evaluator/helpers/LineParser.hpp"

#include <iostream>
#include <sstream>

namespace state_smurf::log_evaluator::helpers {
	
	constexpr int MINIMAL_CIRCUIT_LOG_SIZE = 5; // [date time] In circuit N:
	constexpr int CIRCUIT_WORD_INDEX = 3;
	
	std::vector<std::string> LineParser::parseLine(std::string line) {
		std::vector<std::string> tokens;
		splitString(tokens, line);
		return tokens;
	}
	
	bool LineParser::compareLines(const std::string &etalon, const std::string &compared) {
		std::vector<std::string> etalonTokens = parseLine(etalon);
		std::vector<std::string> comparedTokens = parseLine(compared);
		
		bool logsAreSame = true;
		
		if (etalonTokens.size() != comparedTokens.size()) {
			logsAreSame = false;
		} else if (etalonTokens.size() > MINIMAL_CIRCUIT_LOG_SIZE &&
		    etalonTokens[CIRCUIT_WORD_INDEX] == "circuit") { // circuit log
			for (int i = MINIMAL_CIRCUIT_LOG_SIZE; i < std::max(etalonTokens.size(), comparedTokens.size()); ++i) {
				/// Circuits can have different numbers for same application, because of transition table is implemented
				/// as map, therefore its printed in different order and circuits are found in different order
				/// -> Must compare states in circuit, not numbers
				if (etalonTokens[i] != comparedTokens[i]) {
					logsAreSame = false;
				}
			}
		} else {    // normal StateTransition log
			for (int i = static_cast<int>(LogTokensIndexes::APP_NAME);
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
		if (tokens[static_cast<unsigned long>(LogTokensIndexes::START_OR_GOING)] != "Start") {
			return tokens[static_cast<unsigned long>(LogTokensIndexes::STATE)];
		} else {
			return "";
		}
	}
	
	std::string LineParser::getTime(const std::string &line) {
		std::vector<std::string> tokens = parseLine(line);
		std::string timeString = tokens[static_cast<unsigned long>(LogTokensIndexes::DATE)];
		timeString.append(" ");
		timeString.append(tokens[static_cast<unsigned long>(LogTokensIndexes::TIME)]);
		return timeString;
	}
	
	void LineParser::splitString(std::vector<std::string> &tokens, const std::string &line) {
		std::stringstream tmpStream( line );

		for (std::string tmp; tmpStream >> tmp; ) {
			tokens.push_back(tmp);
		}
	}
}