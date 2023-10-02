#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/helpers/Filter.hpp>
#include <state_smurf/log_evaluator/helpers/LineParser.hpp>

#include <iostream>
#include <map>

namespace state_smurf::log_evaluator {
	
	constexpr int ORIGIN_STATE_INDEX = 5;
	constexpr int DEST_STATES_INDEX = 7;
	
	CircuitFinder::CircuitFinder(std::istream &srcFile) {
		if (!createAdjacencyMatrix(srcFile)) {
			return;
		}
		blocked_.resize(numberOfVertexes_, false);
		blockMatrix_.resize(numberOfVertexes_);
		for (int i = 0; i < numberOfVertexes_; ++i) {
			blockMatrix_[i].resize(numberOfVertexes_, false);
		}
	}
	
	std::vector<std::vector<std::string>> CircuitFinder::find() {
		for (int i = 0; i < startingVertexes_.size(); ++i) {
			if (!adjacencyMatrix_.empty()) {
				for (int j = 0; j < numberOfVertexes_; j++) {
					for (int k = 0; k < numberOfVertexes_; k++) {
						blockMatrix_[j][k] = false;
					}
					blocked_[j] = false;
				}
				startVertex_ = startingVertexes_[i];
				circuit(startVertex_);
				
				// adding adjacent vertexes as starting
				for (int j = 0; j < numberOfVertexes_; ++j) {
					if (adjacencyMatrix_[startVertex_][j]) {
						startingVertexes_.push_back(j);
					}
				}
				// removing all incoming edges to completely discovered vertex
				for (int j = 0; j < numberOfVertexes_; ++j) {
					if (adjacencyMatrix_[j][startVertex_]) {
						adjacencyMatrix_[j][startVertex_] = false;
					}
				}
			} else {
				break;
			}
		}
		return circuits_;
	}
	
	void CircuitFinder::unblock(const int &vertex) {
		blocked_[vertex] = false;
		for (int i = 0; i < numberOfVertexes_; ++i) {
			if (blockMatrix_[vertex][i]) {
				blockMatrix_[vertex][i] = false;
				unblock(i);
			}
		}
	}
	
	
	bool CircuitFinder::circuit(const int &vertex) {
		bool found = false;
		visitedVertexes_.push_back(vertex);
		blocked_[vertex] = true;
		int nextVertex = -1;
		for (int i = 0; i < numberOfVertexes_; ++i) {
			if (adjacencyMatrix_[vertex][i]) {
				nextVertex = i;
			} else {
				continue;
			}
			// ^^ found next Vertex, now decide what to do next
			if (nextVertex == startVertex_) {
				std::vector<std::string> newCircuit;
				for (const auto &circuitVertex: visitedVertexes_) {
					newCircuit.push_back(stateNames_[circuitVertex]);
				}
				circuits_.push_back(newCircuit);
				found = true;
			} else if (!blocked_[nextVertex]) {
				if (circuit(nextVertex)) {
					found = true;
				}
			}
		}
		if (found) {
			unblock(vertex);
		} else {
			for (int i = 0; i < numberOfVertexes_; ++i) {
				if (adjacencyMatrix_[vertex][i]) {
					blockMatrix_[i][vertex] = true;
				}
			}
		}
		visitedVertexes_.pop_back();
		return found;
	}
	
	bool CircuitFinder::createAdjacencyMatrix(std::istream &srcFile) {
		std::string line = helpers::Filter::findDiagramSmurfLog(srcFile);
		std::map<std::string, int> namesMap;
		
		std::vector<std::vector<std::string>> adjacencyTokens;
		while (!line.empty()) {
			adjacencyTokens.push_back(helpers::LineParser::parseLine(line));
			line = helpers::Filter::findDiagramSmurfLog(srcFile);
		}
		
		numberOfVertexes_ = adjacencyTokens.size();
		if (numberOfVertexes_ < 1) {
			std::cerr << "ERROR: Invalid state diagram" << std::endl;
			return false;
		}
		adjacencyMatrix_.resize(numberOfVertexes_);
		
		for (int i = 0; i < numberOfVertexes_; ++i) {
			stateNames_.push_back(adjacencyTokens[i][ORIGIN_STATE_INDEX]);
			namesMap[adjacencyTokens[i][ORIGIN_STATE_INDEX]] = i;
		}
		for (int i = 0; i < numberOfVertexes_; ++i) {
			adjacencyMatrix_[i].resize(numberOfVertexes_, false);
			if (adjacencyTokens[i][ORIGIN_STATE_INDEX] == "__START__") {    // different approach for starting vertexes
				for (int j = DEST_STATES_INDEX; j < adjacencyTokens[i].size(); ++j) {
					startingVertexes_.push_back(namesMap[adjacencyTokens[i][j]]);
					// don't need to add to adjacency list since __START__ is never part of circuit
				}
			} else {
				for (int j = DEST_STATES_INDEX; j < adjacencyTokens[i].size(); ++j) {
					// adjacency matrix line i, index of state on index j is true
					std::string state = adjacencyTokens[i][j];
					adjacencyMatrix_[i][namesMap[state]] = true;
				}
			}
		}
		if (startingVertexes_.empty()) {
			std::cerr << "ERROR: no starting vertexes" << std::endl;
			return false;
		}
		return true;
	}
}
