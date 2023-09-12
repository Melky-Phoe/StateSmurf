#include <state_smurf/log_evaluator/CircuitAggregator.hpp>
#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	CircuitAggregator::CircuitAggregator(std::istream &sourceLogFile) {
		state_smurf::log_evaluator::CircuitFinder CF(sourceLogFile);
		circuitList_ = CF.find();
		sourceLogFile.clear();
		sourceLogFile.seekg(std::ios::beg);
	}
	
	std::vector<std::string> CircuitAggregator::createAggregatedVector(std::istream &sourceLogFile) {
		transitionLogVector_ = Filter::createTransitionLogVector(sourceLogFile);
		if (transitionLogVector_[0].ends_with("Start of Run -- Aggregated")) {
			std::cout << "The input file is already aggregated. Skipping aggregation" << std::endl;
			return transitionLogVector_;
		}
		std::vector<std::string> aggregatedLogVector;
		
		std::vector<int> circuitFoundIndexes;
		long currentCircuit = END_FOUND;
		for (transitionIndex_ = 0; transitionIndex_ < transitionLogVector_.size();) {
			long nextCircuit = getCircuit(currentCircuit);
			if (nextCircuit == END_FOUND) {
				if (LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
					handleStart(aggregatedLogVector);
					currentCircuit = START_FOUND;
				} else {
					handleEnd(currentCircuit, aggregatedLogVector);
					currentCircuit = END_FOUND;
				}
			} else if (nextCircuit == NOT_FOUND) {
				aggregatedLogVector.push_back(transitionLogVector_[transitionIndex_]);
				transitionIndex_++;
				currentCircuit = NOT_FOUND;
			} else if (nextCircuit == NO_CIRCUITS) {
				handleNoCircuits(aggregatedLogVector);
			} else {
				if (nextCircuit != currentCircuit) {
					printCircuitLog(aggregatedLogVector, nextCircuit);
				}
				transitionIndex_ += circuitList_[nextCircuit].size();
				currentCircuit = nextCircuit;
			}
		}
		return aggregatedLogVector;
	}
	
	long CircuitAggregator::getLongestCircuitIndex(const std::vector<int> &circuitFoundIndexes) {
		long longestCircuitIndex = 0;
		u_long longestCircuitSize = 0;
		for (auto index: circuitFoundIndexes) {
			if (circuitList_[index].size() > longestCircuitSize) {
				longestCircuitIndex = index;
				longestCircuitSize = circuitList_[index].size();
			}
		}
		return longestCircuitIndex;
	}
	
	long CircuitAggregator::getCircuit(long lastCircuit) {
		if (circuitList_.empty()) {
			return NO_CIRCUITS;
		}
		std::vector<int> circuitFoundIndexes;
		for (int i = 0; i < circuitList_.size(); ++i) {
			for (int j = 0; j < circuitList_[i].size(); ++j) {
				auto nextState = LineParser::getState(transitionLogVector_[transitionIndex_ + j]);
				if (nextState.empty() ) { // Start of Run is found
					if (lastCircuit == START_FOUND) {
						break;
					} else {
						return END_FOUND;
					}
				}
				if (nextState != circuitList_[i][j]) {
					break;
				} else if (j == circuitList_[i].size() - 1) { // all elements are equal
					circuitFoundIndexes.push_back(i);
				} else if (transitionIndex_ + j == transitionLogVector_.size() - 1) {   // end of vector, any step further will raise exception
					if (lastCircuit == START_FOUND) {
						break;
					} else {
						return END_FOUND;
					}
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
	
	void CircuitAggregator::handleEnd(long currentCircuit, std::vector<std::string> &targetLogFile) {
		if (currentCircuit == NOT_FOUND || currentCircuit == START_FOUND) {
			return;
		} else {
			if (currentCircuit < 0) {
				while (transitionIndex_ < transitionLogVector_.size() &&
				       !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
					targetLogFile.push_back(transitionLogVector_[transitionIndex_]);
					transitionIndex_++;
				}
			} else {
				bool stayInCircuit = true;
				while (transitionIndex_ < transitionLogVector_.size() &&
				       !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
					for (int j = 0; j < circuitList_[currentCircuit].size() &&
					                (transitionIndex_ + j) < transitionLogVector_.size(); ++j) {
						auto state = LineParser::getState(transitionLogVector_[transitionIndex_ + j]);
						if (state.empty()) {
							transitionIndex_ += j;
							return;
						}
						if (state != circuitList_[currentCircuit][j]) {
							stayInCircuit = false;
							break;
						}
					}
					if (!stayInCircuit) {
						// States before end doesn't follow last circuit's states. Prints all states
						while (transitionIndex_ < transitionLogVector_.size() &&
						       !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
							targetLogFile.push_back(transitionLogVector_[transitionIndex_]);
							transitionIndex_++;
						}
						return;
					} else {
						if (currentCircuit < 0) { // endless loop
							transitionIndex_++;
						} else {
							transitionIndex_ += circuitList_[currentCircuit].size();
						}
					}
				}
			}
		}
	}
	
	void CircuitAggregator::handleStart(std::vector<std::string> &targetLogFile) {
		std::string newLine = transitionLogVector_[transitionIndex_];
		newLine.append(" -- Aggregated");
		targetLogFile.push_back(newLine);
		transitionIndex_++;
	}
	
	void CircuitAggregator::handleNoCircuits(std::vector<std::string> &targetLogFile) {
		while (transitionIndex_ < transitionLogVector_.size()) {
			if (LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
				handleStart(targetLogFile);
			} else {
				targetLogFile.push_back(transitionLogVector_[transitionIndex_]);
				transitionIndex_++;
			}
		}
	}
	
	void CircuitAggregator::printCircuitLog(std::vector<std::string> &targetLogFile, long circuitIndex) {
		std::string newLine = LineParser::getTime(transitionLogVector_[transitionIndex_]);
		newLine.append(" In circuit ");
		newLine.append(std::to_string(circuitIndex));
		newLine.append(": [");
		for (const auto &state: circuitList_[circuitIndex]) {
			newLine.append(state);
			newLine.append(", ");
		}
		newLine.append("]");
		targetLogFile.push_back(newLine);
	}
	
	
}
