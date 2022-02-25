#include <state_smurf/log_evaluator/CircuitAggregator.hpp>
#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	constexpr long NOT_FOUND = -1;
	constexpr long END_FOUND = -2;
	constexpr long LAST_FOUND = -3;
	
	CircuitAggregator::CircuitAggregator(std::istream &sourceLogFile) {
		_transitionLogVector = Filter::createTransitionLogVector(sourceLogFile);
		sourceLogFile.clear();
		sourceLogFile.seekg(std::ios::beg);
		state_smurf::log_evaluator::CircuitFinder CF(sourceLogFile);
		_circuitList = CF.find();
	}
	
	void CircuitAggregator::createAggregatedFile(const std::string &newFileName) {
		// Opening new file
		std::ofstream targetFile(newFileName);
		if (!targetFile.is_open()) {
			std::cerr << "Unable to open " << newFileName << std::endl;
			return;
		}
		
		std::vector<int> circuitFoundIndexes;
		long currentCircuit = END_FOUND;
		for (_transitionIndex = 0; _transitionIndex < _transitionLogVector.size();) {
			long nextCircuit = getCircuit();
			if (nextCircuit == END_FOUND) {
				if (LineParser::getState(_transitionLogVector[_transitionIndex]).empty()) {
					targetFile << _transitionLogVector[_transitionIndex] << " -- Aggregated" << std::endl;
				}
				if (!handleEnd(currentCircuit)) {
					while (_transitionIndex < _transitionLogVector.size() &&
					       !LineParser::getState(_transitionLogVector[_transitionIndex]).empty()) {
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
					for (const auto &state: _circuitList[nextCircuit]) {
						targetFile << state << ", ";
					}
					targetFile << "]" << std::endl;
				}
				_transitionIndex += _circuitList[nextCircuit].size();
				currentCircuit = nextCircuit;
			}
		}
		targetFile.close();
	}
	
	long CircuitAggregator::getLongestCircuitIndex(const std::vector<int> &circuitFoundIndexes) {
		long max = 0;
		for (auto index: circuitFoundIndexes) {
			if (_circuitList[index].size() > max) {
				max = index;
			}
		}
		return max;
	}
	
	long CircuitAggregator::getCircuit() {
		std::vector<int> circuitFoundIndexes;
		for (int i = 0; i < _circuitList.size(); ++i) {
			for (int j = 0; j < _circuitList[i].size(); ++j) {
				auto nextState = LineParser::getState(_transitionLogVector[_transitionIndex + j]);
				if (nextState.empty() || _transitionIndex + j == _transitionLogVector.size() - 1) {
					return END_FOUND;
				}
				/*if (_transitionIndex+j == _transitionLogVector.size()-1) {
					return LAST_FOUND;
				}*/
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
	
	bool CircuitAggregator::handleEnd(long currentCircuit) {
		if (currentCircuit == END_FOUND) { // || currentCircuit == NOT_FOUND) {
			_transitionIndex++;
		} else if (currentCircuit == NOT_FOUND) {
			return false;
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
