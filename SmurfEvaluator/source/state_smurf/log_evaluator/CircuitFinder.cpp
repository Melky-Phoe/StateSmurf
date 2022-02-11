#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <iostream>

namespace state_smurf::log_evaluator {

    CircuitFinder::CircuitFinder(diagram::StateDiagram stateDiagram) : adjacencyMap{stateDiagram.createAdjacencyMap()} {
        numberOfVertexes = adjacencyMap.size();
        blocked = static_cast<bool *>(calloc(sizeof(bool), numberOfVertexes));
		blockMap.resize(numberOfVertexes);
        if (blocked == nullptr) {
            exit(1); // exception
        }
        for (auto & it : adjacencyMap) {
            keys.push_back(it.first);
        }
		startingVertexes = stateDiagram.getStartVertexes();
    }
	
	std::vector<std::vector<std::string>>  CircuitFinder::find() {
		for (int i = 0; i < startingVertexes.size(); ++i) {
			if (!adjacencyMap.empty()) {
				for (int j = 0; j < numberOfVertexes; j++) {
					blockMap[j].clear();
					blocked[j] = false;
				}
				startVertex = startingVertexes[i];
				circuit(startVertex);
				
				for (const auto& adjacentVertex : adjacencyMap[startVertex]) {
					startingVertexes.push_back(adjacentVertex);
				}
				
				// removing Vertex that has been completly discovered
				adjacencyMap.erase(startVertex);
				for (auto & it : adjacencyMap) {
					auto it2 = std::find(it.second.begin(), it.second.end(), startVertex);
					if (it2 != it.second.end()) {
						it.second.erase(it2);
					}
				}
			} else {
				break;
			}
		}
		return circuits;
	}
	
    void CircuitFinder::unblock(const std::shared_ptr<diagram::Vertex>& vertex) {
        long vertexIndex = getVertexIndex(keys, vertex);
        blocked[vertexIndex] = false;
	    for (auto it = blockMap[vertexIndex].begin(); it != blockMap[vertexIndex].end();) {
		    auto blockedVertexIndex = it - blockMap[vertexIndex].begin();
		    it = blockMap[vertexIndex].erase(it);
		    if (blocked[blockedVertexIndex]) {
			    unblock(blockMap[vertexIndex][blockedVertexIndex]);
		    }
	    }
    }

    long CircuitFinder::getVertexIndex(const std::vector<std::shared_ptr<diagram::Vertex>>& vertexVector,
                                      const std::shared_ptr<diagram::Vertex>& vertex) {
        auto it = std::find(vertexVector.begin(), vertexVector.end(), vertex);
        if (it != vertexVector.end()) {
            long index = it - vertexVector.begin();
            return index;
        } else {
            return -2;
        }
    }

    bool CircuitFinder::circuit(const std::shared_ptr<diagram::Vertex>& vertex) {
        bool found = false;
        long currVertexIndex = getVertexIndex(keys, vertex);
        visitedVertexes.push_back(vertex);
        blocked[currVertexIndex] = true;

        for (const auto& nextVertex : adjacencyMap[vertex]) {
            if (nextVertex == startVertex) {
				std::vector<std::string> newCircuit;
                for (const auto& circuitVertex : visitedVertexes) {
                    // save circuit
                    //std::cout << circuitVertex->getName() << " ";
					newCircuit.push_back(circuitVertex->getName());
                }
                //std::cout << std::endl; // oddelovac DEBUG
				circuits.push_back(newCircuit);
                found = true;
            } else if (!blocked[getVertexIndex(keys, nextVertex)]) {
                if (circuit(nextVertex)) {
                    found = true;
                }
            }
        }
        if (found) {
            unblock(vertex);
        } else {
            for (auto blockVertex : adjacencyMap[vertex]) {
				auto blockedVector = blockMap[getVertexIndex(keys, blockVertex)];
	            if (std::find(blockedVector.begin(), blockedVector.end(), vertex) == blockedVector.end()) {
		            blockMap[getVertexIndex(keys, blockVertex)].push_back(vertex);
				}
            }
        }
		visitedVertexes.pop_back();
        return found;
    }
}
