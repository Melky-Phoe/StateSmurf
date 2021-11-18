#pragma once

#include <Vertex.h>
#include <Edge.h>

#include <vector>
#include <memory>

namespace bringauto {

class StateGraph {
public:
	std::vector<Edge> edges;
	std::vector<std::shared_ptr<Vertex>> vertexes;

	void setEdge(std::shared_ptr<Vertex> from, std::shared_ptr<Vertex> to);

	std::shared_ptr<Vertex> addVertex(std::string name);

	// Logic
	bool changeState(const std::shared_ptr<Vertex> &vertex);

	bool changeStateByName(const std::string &vertexName);

	bool stateExist(const std::string &vertexName);

	std::string getCurrentStateName() { return _currentState != nullptr ? _currentState->name : ""; }

private:
	std::shared_ptr<Vertex> _currentState{nullptr};
};

}