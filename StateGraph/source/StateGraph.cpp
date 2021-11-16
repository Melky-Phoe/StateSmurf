//
// Created by josef on 05.11.21.
//

#include "StateGraph.h"

#include <utility>
#include <iostream>

void StateGraph::setEdge(std::shared_ptr<Vertex> from, std::shared_ptr<Vertex> to) {
	Edge edge{from, to};

	edges.push_back(edge);
}

std::shared_ptr<Vertex> StateGraph::addVertex(std::string name) {
	auto vertex = std::make_shared<Vertex>(std::move(name));
	vertexes.push_back(vertex);
	return vertex;
}

bool StateGraph::changeStateByName(const std::string& vertexName) {
	if (_currentState == nullptr) {
		for (const auto& vertex : vertexes) {
			if (vertex->name == vertexName) {
				_currentState = vertex;
				return true;
			}
		}
		return false;
	} else {
		for (const auto& edge : edges) {
			if (edge.from->name == _currentState->name) {
				if (edge.to->name == vertexName) {
					std::cout << "Prechazim do " << vertexName << std::endl;
					_currentState = edge.to;
					return true;
				}
			}
		}
		std::cout << "Nelze prejit do " << vertexName << std::endl;
		return false;
	}
}

bool StateGraph::changeState(const std::shared_ptr<Vertex>& vertex) {
	if (_currentState == nullptr) {
		_currentState = vertex;
		return true;
	} else {
		for (const auto& edge: edges) {
			if (edge.from == _currentState) {
				if (edge.to == vertex) {
					std::cout << "Prechazim do " << vertex->name << std::endl;
					_currentState = vertex;
					return true;
				}
			}
		}
		std::cout << "Nelze prejit do " << vertex->name << std::endl;
		return false;
	}
}
