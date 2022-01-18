#pragma once

#include <Vertex.hpp>
#include <Edge.hpp>

#include <vector>
#include <memory>

namespace bringauto {

class StateGraph {
public:
	std::vector<Edge> edges;
	std::vector<std::shared_ptr<Vertex>> vertexes;

	void setEdge(const std::shared_ptr<Vertex>& from, const std::shared_ptr<Vertex>& to);

	std::shared_ptr<Vertex> addVertex(const std::string &name);

	// Logic
	bool changeState(const std::shared_ptr<Vertex> &vertex);

	bool changeStateByName(const std::string &vertexName);

	bool stateExist(const std::string &vertexName);

	std::string getCurrentStateName() { return _currentState != nullptr ? _currentState->name : ""; }

private:
	std::shared_ptr<Vertex> _currentState{nullptr};
};

}