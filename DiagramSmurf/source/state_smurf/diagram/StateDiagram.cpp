#include <state_smurf/diagram/StateDiagram.hpp>
#include <iostream>

namespace state_smurf::diagram {

void StateDiagram::setEdge(const std::shared_ptr<Vertex>& from, const std::shared_ptr<Vertex>& to) {
	bool vertexesFound = false;
	
	if (adjacencyList.find(from) != adjacencyList.end() && adjacencyList.find(to) != adjacencyList.end()) {
		vertexesFound = true;
	}

	if (vertexesFound) {
		adjacencyList[from].push_back(to);
	} else {
		std::cerr << "Cant create edge, vertex doesn't exist" << from->getName() << " or " << to->getName() << std::endl;
	}

}

std::shared_ptr<Vertex> StateDiagram::addVertex(const std::string &name) {
	if (name.empty()) {
		std::cerr << "Vertex can't have empty name, give valid string to addVertex";
	}
    if (!stateExist(name)) {
        auto vertex = std::make_shared<Vertex>(name);
        adjacencyList[vertex]; // creating new key in map
        return vertex;
    } else {
        std::cerr << "Vertex with name \"" << name <<"\" already exists" << std::endl;
        return nullptr;
    }
}

bool StateDiagram::changeStateByName(const std::string &vertexName) {
	for (const auto& vertex : adjacencyList[_currentState]) {
		if (vertex->getName() == vertexName) {
			_currentState = vertex;
			return true;
		}
	}
	return false;
}

bool StateDiagram::changeState(const std::shared_ptr<Vertex> &destinationVertex) {
	for (const auto& vertex : adjacencyList[_currentState]) {
		if (destinationVertex == vertex) {
			_currentState = destinationVertex;
			return true;
		}
	}
	return false;
}

bool StateDiagram::stateExist(const std::string &vertexName) {
	for (const auto &it: adjacencyList) {
		if (it.first->getName() == vertexName) {
			return true;
		}
	}
	return false;
}

std::shared_ptr<Vertex> StateDiagram::findStateByName(const std::string &vertexName) {
	for (const auto &it: adjacencyList) {
		if (it.first->getName() == vertexName) {
			return it.first;
		}
	}
	return nullptr;
}

void StateDiagram::setStartVertex(const std::shared_ptr<Vertex>& vertex) {
	if (adjacencyList.find(vertex) != adjacencyList.end()) {
		// Possible to just push to vector
		adjacencyList[findStateByName("__START__")].push_back(vertex);
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