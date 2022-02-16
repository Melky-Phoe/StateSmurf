#include <state_smurf/log_evaluator/CircuitCreator.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	constexpr long NOT_FOUND = -1;
	constexpr long END_FOUND = -2;
	
	CircuitCreator::CircuitCreator(std::istream &sourceLogFile,
	                               const std::vector<std::vector<std::string>> &circuitList) {
		_transitionLogVector = Filter::createTransitionLogVector(sourceLogFile);
		_circuitList = circuitList;
	}
	
	std::ofstream CircuitCreator::createCircuitFile(const std::string& newFileName) {
		// Opening new file
		std::ofstream targetFile(newFileName);
		if (!targetFile.is_open()) {
			std::cerr << "Unable to open " << newFileName << std::endl;
			return targetFile;
		}
		
		std::vector<int> circuitFoundIndexes;
		long currentCircuit = END_FOUND;
		for (_transitionIndex = 0; _transitionIndex < _transitionLogVector.size();) {
			long nextCircuit = getCircuit();
			if (nextCircuit == END_FOUND) {
				if (LineParser::getState(_transitionLogVector[_transitionIndex]).empty()) {
					targetFile << _transitionLogVector[_transitionIndex] << std::endl;
				}
				if (!handleEnd(currentCircuit)) {
					while (_transitionIndex < _transitionLogVector.size() && !LineParser::getState(_transitionLogVector[_transitionIndex]).empty()) {
						targetFile << _transitionLogVector[_transitionIndex] << std::endl;
						_transitionIndex++;
					}
				}
				currentCircuit = END_FOUND;
			} else if (nextCircuit == NOT_FOUND) {
				targetFile << _transitionLogVector[_transitionIndex] << std::endl;
				_transitionIndex++;
				currentCircuit = NOT_FOUND;
			} else {
				if (nextCircuit != currentCircuit) {
					targetFile << "In circuit " << nextCircuit << ": [";
					for (const auto& state : _circuitList[nextCircuit]) {
						targetFile << state << ", ";
					}
					targetFile << "]" << std::endl;
				}
				_transitionIndex += _circuitList[nextCircuit].size();
				currentCircuit = nextCircuit;
			}
		}
		return targetFile;
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
		bool inCircuit = true;
		while (inCircuit && _transitionIndex < _transitionLogVector.size()) {
			_transitionIndex += circuitLength;
			for (int j = 0; j < _circuitList[circuitIndex].size() && (_transitionIndex+j) < _transitionLogVector.size(); ++j) {
				if (LineParser::getState(_transitionLogVector[_transitionIndex + j]) != _circuitList[circuitIndex][j]) {
					inCircuit = false;
					break;
				}
			}
			if (!inCircuit) {
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
				if (nextState.empty() || _transitionIndex+j == _transitionLogVector.size()-1) {
					return END_FOUND;
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
			return NOT_FOUND;
		}
	}
	
	bool CircuitCreator::handleEnd(long currentCircuit) {
		if (currentCircuit == END_FOUND || currentCircuit == NOT_FOUND) {
			_transitionIndex++;
		} else {
			bool stayInCircuit = true;
			while (_transitionIndex < _transitionLogVector.size()) {
				for (int j = 0; j < _circuitList[currentCircuit].size() &&
				                (_transitionIndex + j) < _transitionLogVector.size(); ++j) {
					auto state = LineParser::getState(_transitionLogVector[_transitionIndex + j]);
					if (state.empty()) {
						_transitionIndex += j;
						return true;
					}
					if (state != _circuitList[currentCircuit][j]) {
						stayInCircuit = false;
						break;
					}
				}
				if (!stayInCircuit) {
					// States before end doesn't follow last circuit's states
					return false;
					
				} else {
					_transitionIndex += _circuitList[currentCircuit].size();
				}
			}
			
		}
		return true;
	}
	
	
}
