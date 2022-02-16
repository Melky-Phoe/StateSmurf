#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/diagram/Vertex.hpp>

#include <stack>

namespace state_smurf::log_evaluator {

/**
 * Class for discovering circuits in state diagrams defined by DiagramSmurf.
 * Circuit is repeatable set of states.
 * circuits are used by Evaluator to aggregate states transition into circuits transition
 *
 * Class uses Johnson's algorithm methods.
 * https://www.cs.tufts.edu/comp/150GA/homeworks/hw1/Johnson%2075.PDF
 */
class CircuitFinder {
public:
	/**
	 * Constructor
	 * @param stateDiagram to search circuits in
	 */
	explicit CircuitFinder(diagram::StateDiagram stateDiagram);
	~CircuitFinder() { free(blocked); blocked = nullptr;};
	
	/**
	 * Find all elementary non-duplicit circuits in state diagram
	 * @return vector of string-vectors, each representing one circuit
	 */
	std::vector<std::vector<std::string>> find();

private:
	/**
	 * Unblock vertex and recursively all vertexes that are in blockMap for given vertex
	 * Vertexes are unblocked when circuit containing them is found
	 * @param vertex
	 */
    void unblock(const std::shared_ptr<diagram::Vertex>& vertex);

	/**
	 * Checks given vertex's adjacent Vertexes, if any is startOfCircuit (bottom on stack), returns true
	 * recursively calls for all adjacent Vertexes
	 * @param vertex
	 * @return true if circuit found
	 */
    bool circuit(const std::shared_ptr<diagram::Vertex>& vertex);

	/**
	 * Returns vertex's index in given array (vector)
	 * @param vertexVector array where to look for vertex
	 * @param vertex that's index is wanted
	 * @return index
	 */
    static long getVertexIndex(const std::vector<std::shared_ptr<diagram::Vertex>>& vertexVector, const std::shared_ptr<diagram::Vertex>& vertex);
	
	/**
	 * Adjacency list, set of possible destination vertexes for each vertex
	 * TODO odkaz, rozdil mezi List a Matrix (matrix asi nerychlejsi) https://www.geeksforgeeks.org/comparison-between-adjacency-list-and-adjacency-matrix-representation-of-graph/#:~:text=A%20graph%20can%20be%20represented,which%20this%20node%20is%20connected.
	 */
    std::map<std::shared_ptr<diagram::Vertex>, std::vector<std::shared_ptr<diagram::Vertex>>> adjacencyList;
	
	/**
	 * Vector of all vertexes ordered, used for indexing in other vectors
	 */
    std::vector<std::shared_ptr<diagram::Vertex>> keys;
	
	/**
	 * List of vertexes that will be unblocked if a vertex is unblocked
	 * For more info check Johnson's algorithm principle
	 */
    std::vector<std::vector<std::shared_ptr<diagram::Vertex>>> blockMap;
	
	/**
	 * Vector of visited vertexes by circuit()
	 * has stack-like behavior, but has to be iterated-over, therefore is implemented as vector
	 * If circuit() finds startVertex, all vertexes in vector are part of a new 4circuit
	 */
    std::vector<std::shared_ptr<diagram::Vertex>> visitedVertexes;
	
	/**
	 * total vertexes in state diagram
	 */
    ulong numberOfVertexes;
	
	/**
	 * list of all startingVertexes of state diagram
	 * find() starts searching for cycles vertexes of this vector
	 * after every circuit is found, adjacent vertexes are appended
	 */
	std::vector<std::shared_ptr<diagram::Vertex>> startingVertexes;
	
	/**
	 * Vertex that circuit() was originally called on, serve for comparing to find end of circuit
	 */
    std::shared_ptr<diagram::Vertex> startVertex;
	
	/**
	 * Array indicating if Vertex on given index is blocked.
	 * Vertexes are blocked if they were already visited and They are unblocked when circuit is found
	 */
    bool *blocked;
	
	/**
	 * Vector of circuits found. Is returned by find() when all circuits were found
	 */
	std::vector<std::vector<std::string>> circuits;
};

}