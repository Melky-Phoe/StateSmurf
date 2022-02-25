#pragma once

#include <state_smurf/diagram/Vertex.hpp>

#include <vector>
#include <memory>
#include <map>

namespace state_smurf::diagram {

	/**
	 * Class used for creating State Diagrams
	 * State Diagram is defined by set of Vertexes (states) and Edges (transitions)
	 * StateDiagram implements logic of valid moving on the diagram (changing current states)
	 */
	class StateDiagram {
	public:
		StateDiagram() { currentState_ = addVertex("__START__"); };
		
		/**
		 * Sets edge directed edge between two vertexes. Saves it into Transition matrix
		 * Both Vertexes must exist.
		 * @param from
		 * @param to
		 */
		void setEdge(const std::shared_ptr<Vertex> &from, const std::shared_ptr<Vertex> &to);
		
		/**
		 * Creates new vertex in StateDiagram
		 * @param name unique name of vertex
		 * @return pointer to Vertex
		 */
		std::shared_ptr<Vertex> addVertex(const std::string &name);
		
		/**
		 * Sets given Vertex as StartVertex. Multiple Starting Vertexes are possible
		 * At least one start vertex must be starting
		 * @param shared_ptr to Vertex
		 */
		void setStartVertex(const std::shared_ptr<Vertex> &vertex);
		
		/**
		 * Changes current state if there is valid Edge between currentState and destinationVertex in parameter
		 * @param destinationVertex as a pointer
		 * @return true if change is possible
		 */
		bool changeState(const std::shared_ptr<Vertex> &destinationVertex);
		
		/**
		 * Changes current state if there is valid Edge between currentState and vertex in parameter
		 * Finds destination vertex based on Name, not pointer
		 * @param vertexName
		 * @return true if change is possible
		 */
		bool changeStateByName(const std::string &vertexName);
		
		/**
		 * Check if Vertex is defined in StateDiagram
		 * @param vertexName
		 * @return true if Vertex exists
		 */
		bool stateExist(const std::string &vertexName);
		
		/**
		 * Finds pointer to Vertex by name
		 * @param vertexName searched vertex
		 * @return shared_ptr
		 */
		std::shared_ptr<Vertex> findStateByName(const std::string &vertexName);
		
		/**
		 * @return adjacency list implemented by std::map
		 */
		std::map<std::shared_ptr<Vertex>, std::vector<std::shared_ptr<Vertex>>>
		getAdjacencyList() { return adjacencyList_; };
		
		/**
		 * Returns name of current state
		 * @return name (string)
		 */
		std::string getCurrentStateName() { return currentState_ != nullptr ? currentState_->getName() : ""; }

	private:
		std::shared_ptr<Vertex> currentState_{nullptr};
		/**
		 * adjacency list of current diagram
		 * implemented by std::map
		 * every vertex in StateDiagram is key for its set of destination vertexes
		 * https://en.wikipedia.org/wiki/Adjacency_list
		 */
		std::map<std::shared_ptr<diagram::Vertex>, std::vector<std::shared_ptr<diagram::Vertex>>> adjacencyList_{};
	};
	
}