#pragma once

#include <state_smurf/diagram/Vertex.hpp>
#include <state_smurf/diagram/Edge.hpp>

#include <vector>
#include <memory>

namespace state_smurf::diagram {

/**
 * Class used for creating StateGraphs
 * State Graph is defined by its set of Vertexes (states) and Edges (transitions)
 * StateGraph implements logic of valid moving on the graph (changing current states)
 */
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
	 * Changes current state if there is valid Edge between currentState and vertex in parameter
	 * This method is rather proof of concept
	 * @param vertex as a pointer
	 * @return true if change is possible
	 */
	bool changeState(const std::shared_ptr<Vertex> &vertex);

    /**
	 * Changes current state if there is valid Edge between currentState and vertex in parameter
     * Finds destination vertex based on Name, not pointer
	 * @param vertexName
	 * @return true if change is possible
	 */
	bool changeStateByName(const std::string &vertexName);

    /**
     * Check if Vertex is defined in State Graph
     * @param vertexName
     * @return true if Vertex exists
     */
	bool stateExist(const std::string &vertexName);

    /**
     * Returns name of current state
     * @return name (string)
     */
	std::string getCurrentStateName() { return _currentState != nullptr ? _currentState->getName() : ""; }

private:
	std::shared_ptr<Vertex> _currentState{nullptr};
    std::vector<Edge> edges;
    std::vector<std::shared_ptr<Vertex>> vertexes;

};

}