#include <state_smurf/diagram/StateDiagram.hpp>
#include <iostream>

namespace state_smurf::diagram {

void StateDiagram::setEdge(const std::shared_ptr<Vertex>& from, const std::shared_ptr<Vertex>& to) {
	bool fromFound = false;
	bool toFound = false;
	for (const auto &vertex: vertexes) {
		if (vertex == from) {
			fromFound = true;
		}
		if (vertex == to) {
			toFound = true;
		}
	}
	if (fromFound && toFound) {
		Edge edge{from, to};
		edges.push_back(edge);
	} else {
		std::cerr << "Cant create edge, vertex doesn't exist" << std::endl;
	}

}

std::shared_ptr<Vertex> StateDiagram::addVertex(const std::string &name) {
	if (name.empty()) {
		std::cerr << "Vertex can't have empty name, give valid string to addVertex";
	}
    if (!stateExist(name)) {
        auto vertex = std::make_shared<Vertex>(name);
        vertexes.push_back(vertex);
        return vertex;
    } else {
        std::cerr << "Vertex with name \"" << name <<"\" already exists" << std::endl;
        return nullptr;
    }
}

bool StateDiagram::changeStateByName(const std::string &vertexName) {
	if (_currentState == nullptr) {
		for (const auto &vertex: startVertexes) {
			if (vertex->getName() == vertexName) {
				_currentState = vertex;
				return true;
			}
		}
		if (startVertexes.empty()) {
			std::cerr << "List of Starting Vertexes is empty, add Starting Vertex:\n"
						 " setStartVertex(std::shared_ptr<Vertex> vertex)" << std::endl;
		}
		return false;
	} else {
		for (auto edge: edges) {
			if (edge.getFrom() == _currentState) {
				if (edge.getTo()->getName() == vertexName) {
					_currentState = edge.getTo();
					return true;
				}
			}
		}
		return false;
	}
}

bool StateDiagram::changeState(const std::shared_ptr<Vertex> &vertex) {
	if (vertex == nullptr) {
		return false;
	}
	if (_currentState == nullptr) {
		for (const auto &startVertex: startVertexes) {
			if (vertex == startVertex) {
				return true;
			}
		}
		return false;
	} else {
		for (auto edge: edges) {
			if (edge.getFrom() == _currentState) {
				if (edge.getTo() == vertex) {
					_currentState = vertex;
					return true;
				}
			}
		}
		return false;
	}
}

std::map<std::shared_ptr<Vertex>, std::vector<std::shared_ptr<Vertex>>> StateDiagram::createAdjacencyList() {
    std::map<std::shared_ptr<Vertex>, std::vector<std::shared_ptr<Vertex>>> adjacencyMap;
    for (auto edge: edges) {
        adjacencyMap[edge.getFrom()].push_back(edge.getTo());
    }
    /* Printing adjacency
    for (const auto & [key, vec] : adjacencyMap) {
        std::cout << key->getName() << std::endl;
        for (auto v : vec) {
            std::cout << v->getName() << " ";
        }
        std::cout << std::endl;

    }*/
    return adjacencyMap;
}

bool StateDiagram::stateExist(const std::string &vertexName) {
	for (const auto &vertex: vertexes) {
		if (vertex->getName() == vertexName) {
			return true;
		}
	}
	return false;
}

void StateDiagram::setStartVertex(std::shared_ptr<Vertex> vertex) {
	if (std::find(vertexes.begin(), vertexes.end(), vertex) != vertexes.end()) {
		startVertexes.push_back(vertex);
	} else {
		std::cerr << "ERROR in setStartVertex: given Vertex must be existing Vertex of StateDiagram" << std::endl;
	}
}
/*
bool StateDiagram::isOptimized() {
	auto adjacencyMap = createAdjacencyMap();
	bool vertexRemovable = false;
	for (auto it : adjacencyMap) {
		if (it.second.size() == 1) {
			for (auto it2 : adjacencyMap) {
				if (std::find(it2.second.begin(), it2.second.end(), it.first) != it2.second.end()) {
					if (std::find(it2.second.begin(), it2.second.end(), it.second.front()) != it2.second.end()) {
						vertexRemovable = false;
						continue;
					} else {
						vertexRemovable = true;
					}
					
				}
			}
			if (vertexRemovable) {
				std::cout << "removable " << it.first->getName();
			}
		}
	}
	return true;
}*/

}