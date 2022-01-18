#pragma once

#include <Vertex.hpp>
#include <Edge.hpp>

#include <vector>
#include <memory>

namespace bringauto {

class StateGraph {
public:
    /**
     * Creates edge between two vertexes given by parameters.
     * Both Vertexes must exist.
     * @param from
     * @param to
     */
	void setEdge(const std::shared_ptr<Vertex>& from, const std::shared_ptr<Vertex>& to);

    /**
     * Creates new vertex in StateGraph
     * @param name unique name of vertex
     * @return pointer to Vertex if name is unique, nullptr otherwise
     */
	std::shared_ptr<Vertex> addVertex(const std::string &name);

	/**
	 * Changes current state
	 * @param vertex
	 * @return
	 */
	bool changeState(const std::shared_ptr<Vertex> &vertex);

	bool changeStateByName(const std::string &vertexName);

	bool stateExist(const std::string &vertexName);

	std::string getCurrentStateName() { return _currentState != nullptr ? _currentState->name : ""; }

private:
	std::shared_ptr<Vertex> _currentState{nullptr};
    std::vector<Edge> edges;
    std::vector<std::shared_ptr<Vertex>> vertexes;

};

}