#include <state_smurf/log_evaluator/CircuitCreator.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	CircuitCreator::CircuitCreator(std::istream &sourceLogFile,
	                               const std::vector<std::vector<std::string>> &circuitList) {
		_transitionLogVector = Filter::createTransitionLogVector(sourceLogFile);
		_circuitList = circuitList;
	}
	
	void CircuitCreator::createCircuit(const std::string& targetFileName) {
		// Opening new file
		std::ofstream targetFile(targetFileName);
		if (!targetFile.is_open()) {
			std::cerr << "Unable to open " << targetFileName << std::endl;
			return;
		}
		
		bool newRun = false;
		
		std::vector<int> circuitFoundIndexes;
		
		for (_transitionIndex = 0; _transitionIndex < _transitionLogVector.size();) {
			for (int i = 0; i < _circuitList.size(); ++i) {
				for (int j = 0; j < _circuitList[i].size() && _transitionIndex+j < _transitionLogVector.size(); ++j) {
					auto nextState = LineParser::getState(_transitionLogVector[_transitionIndex + j]);
					if (nextState.empty()) {
						newRun = true;
						break;
					}
					if (nextState != _circuitList[i][j]) {
						break;
					} else if (j == _circuitList[i].size() - 1) { // all elements are equal
						circuitFoundIndexes.push_back(i);
					}
				}
			}
			// I have all possible circuits
			if (!circuitFoundIndexes.empty()) {
				u_long index = getLongestCircuitIndex(circuitFoundIndexes);
				targetFile << "In loop " << index << ": [";
				for (const auto& state : _circuitList[index]) {
					targetFile << state << ", ";
				}
				targetFile << "]" << std::endl;
				aggregateCircuit(index);
				
				circuitFoundIndexes.clear();
			} else {
				if (newRun) {
					for (; _transitionIndex < _transitionLogVector.size() && !LineParser::getState(_transitionLogVector[_transitionIndex]).empty(); _transitionIndex++) {
						targetFile << _transitionLogVector[_transitionIndex] << std::endl;
					}

					newRun = false;
				}
				targetFile << _transitionLogVector[_transitionIndex] << std::endl;
				_transitionIndex++;
			}
		}
		std::cout << "Circuits are written in file: " << targetFileName << std::endl;
	}
	
	long CircuitCreator::getLongestCircuitIndex(const std::vector<int>& circuitFoundIndexes) {
		long max = 0;
		for (auto index : circuitFoundIndexes) {
			if (_circuitList[index].size() > max) {
				max = index;
			}
		}
		return max;
	}
	
	
	
	int CircuitCreator::aggregateCircuit(const u_long &circuitIndex) {
		u_long circuitLength = _circuitList[circuitIndex].size();
		bool inLoop = true;
		while (inLoop && _transitionIndex < _transitionLogVector.size()) {
			_transitionIndex += circuitLength;
			for (int j = 0; j < _circuitList[circuitIndex].size() && (_transitionIndex+j) < _transitionLogVector.size(); ++j) {
				if (LineParser::getState(_transitionLogVector[_transitionIndex + j]) != _circuitList[circuitIndex][j]) {
					inLoop = false;
					break;
				}
			}
			if (!inLoop) {
				return 0;
			}
		}
		return 0;
	}
	
	long CircuitCreator::getCircuit() {
		std::vector<int> circuitFoundIndexes;
		for (int i = 0; i < _circuitList.size(); ++i) {
			for (int j = 0; j < _circuitList[i].size(); ++j) {
				auto nextState = LineParser::getState(_transitionLogVector[_transitionIndex + j]);
				if (nextState.empty() || _transitionIndex+j < _transitionLogVector.size()) {
					// TODO Handle end
					break;
				}
				if (nextState != _circuitList[i][j]) {
					break;
				} else if (j == _circuitList[i].size() - 1) { // all elements are equal
					circuitFoundIndexes.push_back(i);
				}
			}
		}
		if (!circuitFoundIndexes.empty()) {
			long index = getLongestCircuitIndex(circuitFoundIndexes);
			return index;
		} else {
			return -1;
		}
	}
	
	
}
