#include <state_smurf/log_evaluator/CircuitAggregator.hpp>
#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {
	
	constexpr long NOT_FOUND = -1;
	constexpr long END_FOUND = -2;
	
	CircuitAggregator::CircuitAggregator(std::istream &sourceLogFile) {
		transitionLogVector_ = Filter::createTransitionLogVector(sourceLogFile);
		sourceLogFile.clear();
		sourceLogFile.seekg(std::ios::beg);
		state_smurf::log_evaluator::CircuitFinder CF(sourceLogFile);
		circuitList_ = CF.find();
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
		for (transitionIndex_ = 0; transitionIndex_ < transitionLogVector_.size();) {
			long nextCircuit = getCircuit();
			if (nextCircuit == END_FOUND) {
				if (LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
					targetFile << transitionLogVector_[transitionIndex_] << " -- Aggregated" << std::endl;
				}
				if (!handleEnd(currentCircuit)) {
					while (transitionIndex_ < transitionLogVector_.size() &&
					       !LineParser::getState(transitionLogVector_[transitionIndex_]).empty()) {
						targetFile << transitionLogVector_[transitionIndex_] << std::endl;
						transitionIndex_++;
					}
				}
				currentCircuit = END_FOUND;
			} else if (nextCircuit == NOT_FOUND) {
				targetFile << transitionLogVector_[transitionIndex_] << std::endl;
				transitionIndex_++;
				currentCircuit = NOT_FOUND;
			} else {
				if (nextCircuit != currentCircuit) {
					targetFile << "In circuit " << nextCircuit << ": [";
					for (const auto &state: circuitList_[nextCircuit]) {
						targetFile << state << ", ";
					}
					targetFile << "]" << std::endl;
				}
				transitionIndex_ += circuitList_[nextCircuit].size();
				currentCircuit = nextCircuit;
			}
		}
		targetFile.close();
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
	
	long CircuitAggregator::getCircuit() {
		std::vector<int> circuitFoundIndexes;
		for (int i = 0; i < circuitList_.size(); ++i) {
			for (int j = 0; j < circuitList_[i].size(); ++j) {
				auto nextState = LineParser::getState(transitionLogVector_[transitionIndex_ + j]);
				if (nextState.empty() || transitionIndex_ + j == transitionLogVector_.size() - 1) {
					return END_FOUND;
				}

				if (nextState != circuitList_[i][j]) {
					break;
				} else if (j == circuitList_[i].size() - 1) { // all elements are equal
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
		if (currentCircuit == END_FOUND) {
			transitionIndex_++;
		} else if (currentCircuit == NOT_FOUND) {
			return false;
		} else {
			bool stayInCircuit = true;
			while (transitionIndex_ < transitionLogVector_.size()) {
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
