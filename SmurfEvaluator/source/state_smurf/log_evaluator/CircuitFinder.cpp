#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <iostream>
#include "state_smurf/log_evaluator/Filter.hpp"
#include "state_smurf/log_evaluator/LineParser.hpp"

namespace state_smurf::log_evaluator {
	
	constexpr int ORIGIN_STATE_INDEX = 5;
	constexpr int DEST_STATES_INDEX = 7;

    CircuitFinder::CircuitFinder(std::istream& srcFile) {
	    createAdjacencyMatrix(srcFile);
        blocked = static_cast<bool *>(calloc(sizeof(bool), numberOfVertexes));
		blockMatrix.resize(numberOfVertexes);
	    for (int i = 0; i < numberOfVertexes; ++i) {
		    blockMatrix[i] = static_cast<bool *>(calloc(sizeof(bool), numberOfVertexes));
		    if (blockMatrix[i] == nullptr) {
			    std::cerr << "ERROR: bad allocation" << std::endl;
			    exit(1);
		    }
	    }
		
        if (blocked == nullptr) {
	        std::cerr << "ERROR: bad allocation" << std::endl;
            exit(1); // exception
        }
    }
	
	std::vector<std::vector<std::string>>  CircuitFinder::find() {
		for (int i = 0; i < startingVertexes.size(); ++i) {
			if (!adjacencyMatrix.empty()) {
				for (int j = 0; j < numberOfVertexes; j++) {
					for (int k = 0; k < numberOfVertexes; k++) {
						blockMatrix[j][k] = false;
					}
					blocked[j] = false;
				}
				startVertex = startingVertexes[i];
				circuit(startVertex);
				
				for (int j = 0; j < numberOfVertexes; ++j) {
					if (adjacencyMatrix[startVertex][j]) {
						startingVertexes.push_back(j);
					}
				}
				// removing all incoming edges to completely discovered vertex
				for (int j = 0; j < numberOfVertexes; ++j) {
					if (adjacencyMatrix[j][startVertex]) {
						adjacencyMatrix[j][startVertex] = false;
					}
				}
			} else {
				break;
			}
		}
		return circuits;
	}
	
    void CircuitFinder::unblock(const int& vertex) {
        blocked[vertex] = false;
	    for (int i = 0; i < numberOfVertexes; ++i) {
		    if (blockMatrix[vertex][i]) {
			    blockMatrix[vertex][i] = false;
			    unblock(i);
			}
	    }
    }


    bool CircuitFinder::circuit(const int& vertex) {
        bool found = false;
        visitedVertexes.push_back(vertex);
        blocked[vertex] = true;
		int nextVertex = -1;
		for (int i = 0; i < numberOfVertexes; ++i) {
			if (adjacencyMatrix[vertex][i]) {
				nextVertex = i;
			} else {
				continue;
			}
			// ^^ mam najity dalsi vertex, ted se rozhodnu co s nim
			if (nextVertex == startVertex) {
				std::vector<std::string> newCircuit;
				for (const auto &circuitVertex: visitedVertexes) {
					// save circuit
					//std::cout << circuitVertex->getName() << " ";
					newCircuit.push_back(stateNames[circuitVertex]);
				}
				//std::cout << std::endl; // oddelovac DEBUG
				circuits.push_back(newCircuit);
				found = true;
			} else if (!blocked[nextVertex]) {
				if (circuit(nextVertex)) {
					found = true;
				}
			}
		}
		if (found) {
			unblock(vertex);
		} else {
			for (int i = 0; i < numberOfVertexes; ++i) {
				if (adjacencyMatrix[vertex][i]) {
					blockMatrix[i][vertex] = true;
				}
			}
		}
		visitedVertexes.pop_back();
		return found;
    }
	
	void CircuitFinder::createAdjacencyMatrix(std::istream& srcFile) {
		std::string line = Filter::findDiagramSmurfLog(srcFile);
		std::map<std::string, int> namesMap;
		
		std::vector<std::vector<std::string>> adjacencyTokens;
		while (!line.empty()) {
			adjacencyTokens.push_back(LineParser::parseLine(line));
			line = Filter::findDiagramSmurfLog(srcFile);
		}
		
		numberOfVertexes = adjacencyTokens.size();
		if (numberOfVertexes < 1) {
			std::cerr << "ERROR: Invalid state diagram" << std::endl;
			exit(1);
		}
		adjacencyMatrix.resize(numberOfVertexes);
		
		for (int i = 0; i < numberOfVertexes; ++i) {
			stateNames.push_back(adjacencyTokens[i][ORIGIN_STATE_INDEX]);
			namesMap[adjacencyTokens[i][ORIGIN_STATE_INDEX]] = i;
		}
		for (int i = 0; i < numberOfVertexes; ++i) {
			if (adjacencyTokens[i][ORIGIN_STATE_INDEX] == "__START__") {    // different approach for starting vertexes
				for (int j = DEST_STATES_INDEX; j < adjacencyTokens[i].size(); ++j) {
					// adjacency matrix line i, index of state on index j is true
					startingVertexes.push_back(namesMap[adjacencyTokens[i][j]]);
				}
			}
			adjacencyMatrix[i] = static_cast<bool *>(calloc(sizeof(bool), numberOfVertexes));
			for (int j = DEST_STATES_INDEX; j < adjacencyTokens[i].size(); ++j) {
				// adjacency matrix line i, index of state on index j is true
				std::string state = adjacencyTokens[i][j];
				adjacencyMatrix[i][namesMap[state]] = true;
			}
		}
		if (startingVertexes.empty()) {
			std::cerr << "ERROR: no starting vertexes" << std::endl;
		}

	}
}
