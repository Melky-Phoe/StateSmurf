#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {

class CircuitCreator {
public:
	CircuitCreator(std::istream& sourceLogFile, const std::vector<std::vector<std::string>>& circuitList);
	std::ofstream createCircuitFile(const std::string& newFileName);

private:
	int aggregateCircuit(const u_long &circuitIndex);
	
	long getCircuit();
	bool handleEnd(long currentCircuit);
	long getLongestCircuitIndex(const std::vector<int>& circuitFoundIndexes);
	
	std::vector<std::vector<std::string>> _circuitList;
	std::vector<std::string> _transitionLogVector;
	u_long _transitionIndex {0};
};

}


