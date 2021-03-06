#include <state_smurf/diagram/StateDiagram.hpp>
#include <iostream>

namespace state_smurf::diagram {
	void StateDiagram::setEdge(const std::shared_ptr<Vertex> &from, const std::shared_ptr<Vertex> &to) {
		if (to->getName() == startName_ || from->getName() == startName_) {
			std::cerr << "Can't create edge with reserved vertex " << startName_ << std::endl;
			return;
		}
		bool vertexesFound = false;
		
		if (adjacencyList_.find(from) != adjacencyList_.end() && adjacencyList_.find(to) != adjacencyList_.end()) {
			vertexesFound = true;
		}
		
		if (vertexesFound) {
			adjacencyList_[from].push_back(to);
		} else {
			std::cerr << "Can't create edge, vertex doesn't exist" << from->getName() << " or " << to->getName()
			          << std::endl;
		}
		
	}
	
	std::shared_ptr<Vertex> StateDiagram::addVertex(const std::string &name) {
		if (name.empty()) {
			std::cerr << "Vertex can't have empty name, give valid string to addVertex";
			return nullptr;
		}
		if (!stateExist(name)) {
			auto vertex = std::make_shared<Vertex>(name);
			adjacencyList_[vertex]; // creating new key in map
			return vertex;
		} else {
			if (name == startName_) {
				std::cerr << "Vertex \"" << startName_ << "\" is reserved!" << std::endl;
			} else {
				std::cerr << "Vertex with name \"" << name << "\" already exists" << std::endl;
			}
			return nullptr;
		}
	}
	
	bool StateDiagram::changeState(const std::string &vertexName) {
		for (const auto &vertex: adjacencyList_[currentState_]) {
			if (vertex->getName() == vertexName) {
				currentState_ = vertex;
				return true;
			}
		}
		return false;
	}
	
	bool StateDiagram::changeState(const std::shared_ptr<Vertex> &destinationVertex) {
		for (const auto &vertex: adjacencyList_[currentState_]) {
			if (destinationVertex == vertex) {
				currentState_ = destinationVertex;
				return true;
			}
		}
		return false;
	}
	
	bool StateDiagram::stateExist(const std::string &vertexName) {
		for (const auto &it: adjacencyList_) {
			if (it.first->getName() == vertexName) {
				return true;
			}
		}
		return false;
	}
	
	std::shared_ptr<Vertex> StateDiagram::findStateByName(const std::string &vertexName) {
		for (const auto &it: adjacencyList_) {
			if (it.first->getName() == vertexName) {
				return it.first;
			}
		}
		return nullptr;
	}
	
	void StateDiagram::setStartVertex(const std::shared_ptr<Vertex> &vertex) {
		if (adjacencyList_.find(vertex) != adjacencyList_.end()) {
			// Possible to just push to vector
			adjacencyList_[findStateByName(startName_)].push_back(vertex);
		} else {
			std::cerr << "ERROR in setStartVertex: given Vertex must be existing Vertex of StateDiagram" << std::endl;
		}
	}
	
	bool StateDiagram::startVertexesEmpty() {
		if (adjacencyList_[findStateByName(startName_)].empty()) {
			return true;
		} else {
			return false;
		}
	}
}