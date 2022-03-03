#include <state_smurf/log_evaluator/CircuitAggregator.hpp>
#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	constexpr long NOT_FOUND = -1;
	constexpr long END_FOUND = -2;
	constexpr long START_FOUND = -3;
	constexpr long NO_CIRCUITS = -4;
	
	CircuitAggregator::CircuitAggregator(std::istream &sourceLogFile) {
		state_smurf::log_evaluator::CircuitFinder CF(sourceLogFile);
		circuitList_ = CF.find();
		sourceLogFile.clear();
		sourceLogFile.seekg(std::ios::beg);
	}
	
	std::vector<std::string> CircuitAggregator::createAggregatedVector(std::istream &sourceLogFile) {
		transitionLogVector_ = Filter::createTransitionLogVector(sourceLogFile);
		std::vector<std::string> aggregatedLogVector;
		
		std::vector<int> circuitFoundIndexes;
		long currentCircuit = END_FOUND;
		for (transitionIndex_ = 0; transitionIndex_ < transitionLogVector_.size();) {
			long nextCircuit = getCircuit(currentCircuit);
			if (nextCircuit == END_FOUND) {
				if (LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
					std::string newLine = transitionLogVector_[transitionIndex_];
					newLine.append(" -- Aggregated");
					aggregatedLogVector.push_back(newLine);
					
					currentCircuit = START_FOUND;
					transitionIndex_++;
				} else {
					if (!handleEnd(currentCircuit)) {
						while (transitionIndex_ < transitionLogVector_.size() &&
						       !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
							aggregatedLogVector.push_back(transitionLogVector_[transitionIndex_]);
							transitionIndex_++;
						}
					}
					currentCircuit = END_FOUND;
				}
			} else if (nextCircuit == NOT_FOUND) {
				aggregatedLogVector.push_back(transitionLogVector_[transitionIndex_]);
				transitionIndex_++;
				currentCircuit = NOT_FOUND;
			} else if (nextCircuit == NO_CIRCUITS) {
				while (transitionIndex_ < transitionLogVector_.size()) {
					if (LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
						std::string newLine = transitionLogVector_[transitionIndex_];
						newLine.append(" -- Aggregated");
						aggregatedLogVector.push_back(newLine);
					} else {
						aggregatedLogVector.push_back(transitionLogVector_[transitionIndex_]);
					}
					transitionIndex_++;
				}
			} else {
				if (nextCircuit != currentCircuit) {
					std::string newLine = LineParser::getTime(transitionLogVector_[transitionIndex_]);
					newLine.append(" In circuit ");
					newLine.append(std::to_string(nextCircuit));
					newLine.append(": [");
					for (const auto &state: circuitList_[nextCircuit]) {
						newLine.append(state);
						newLine.append(", ");
					}
					newLine.append("]");
					aggregatedLogVector.push_back(newLine);
				}
				transitionIndex_ += circuitList_[nextCircuit].size();
				currentCircuit = nextCircuit;
			}
		}
		return aggregatedLogVector;
	}
	
	long CircuitAggregator::getLongestCircuitIndex(const std::vector<int> &circuitFoundIndexes) {
		long max = 0;
		for (auto index: circuitFoundIndexes) {
			if (circuitList_[index].size() > max) {
				max = index;
			}
		}
		return max;
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
	
	bool CircuitAggregator::handleEnd(long currentCircuit) {
		if (currentCircuit == NOT_FOUND || currentCircuit == START_FOUND) {
			return false;
		} else {
			bool stayInCircuit = true;
			while (transitionIndex_ < transitionLogVector_.size() && !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
				for (int j = 0; j < circuitList_[currentCircuit].size() &&
				                (transitionIndex_ + j) < transitionLogVector_.size(); ++j) {
					auto state = LineParser::getState(transitionLogVector_[transitionIndex_ + j]);
					if (state.empty()) {
						transitionIndex_ += j;
						return true;
					}
					if (state != circuitList_[currentCircuit][j]) {
						stayInCircuit = false;
						break;
					}
				}
				if (!stayInCircuit) {
					// States before end doesn't follow last circuit's states
					return false;
					
				} else {
					transitionIndex_ += circuitList_[currentCircuit].size();
				}
			}
			
		}
		return true;
	}
	
	
}
