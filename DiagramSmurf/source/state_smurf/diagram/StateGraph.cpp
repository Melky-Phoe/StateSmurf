#include <state_smurf/diagram/StateGraph.hpp>
#include <iostream>

namespace state_smurf::diagram {

void StateGraph::setEdge(const std::shared_ptr<Vertex>& from, const std::shared_ptr<Vertex>& to) {
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

std::shared_ptr<Vertex> StateGraph::addVertex(const std::string &name) {
    if (!stateExist(name)) {
        auto vertex = std::make_shared<Vertex>(name);
        vertexes.push_back(vertex);
        return vertex;
    } else {
        std::cerr << "Vertex with name \"" << name <<"\" already exists" << std::endl;
        return nullptr;
    }
}

bool StateGraph::changeStateByName(const std::string &vertexName) {
	if (_currentState == nullptr) {
		for (const auto &vertex: vertexes) {
			if (vertex->getName() == vertexName) {
				_currentState = vertex;
				return true;
			}
		}
		return false;
	} else {
		for (auto edge: edges) {
			if (edge.getFrom() == _currentState) {
				if (edge.getTo()->getName() == vertexName) {
					// std::cout << "Going to " << vertexName << std::endl; // Debug print
					_currentState = edge.getTo();
					return true;
				}
			}
		}
		// std::cout << "Can't go to " << vertexName << std::endl; // Debug print
		return false;
	}
}

bool StateGraph::changeState(const std::shared_ptr<Vertex> &vertex) {
	if (vertex == nullptr) {
		return false;
	}
	if (_currentState == nullptr) {
		_currentState = vertex;
		return true;
	} else {
		for (auto edge: edges) {
			if (edge.getFrom() == _currentState) {
				if (edge.getTo() == vertex) {
					// std::cout << "Prechazim do " << vertex->name << std::endl;
					_currentState = vertex;
					return true;
				}
			}
		}
		// std::cout << "Nelze prejit do " << vertex->name << std::endl;
		return false;
	}
}

bool StateGraph::stateExist(const std::string &vertexName) {
	for (const auto &vertex: vertexes) {
		if (vertex->getName() == vertexName) {
			return true;
		}
	}
	return false;
}
}