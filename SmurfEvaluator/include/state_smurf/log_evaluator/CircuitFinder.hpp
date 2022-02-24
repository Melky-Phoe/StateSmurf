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
	explicit CircuitFinder(std::istream& srcFile);
	~CircuitFinder();
	
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
    void unblock(const int& vertex);

	/**
	 * Checks given vertex's adjacent Vertexes, if any is startOfCircuit (bottom on stack), returns true
	 * recursively calls for all adjacent Vertexes
	 * @param vertex
	 * @return true if circuit found
	 */
    bool circuit(const int& vertex);

	/**
	 * Creates adjacency matrix from [DiagramSmurf] logs in srcFile
	 * @param srcFile .log file input stream
	 */
	bool createAdjacencyMatrix(std::istream& srcFile);

	/**
	 * Vector of state names according to their indexes
	 */
	std::vector<std::string> stateNames;
	
	/**
	 * Adjacency matrix is set of possible destination vertexes for each vertex
	 */
	std::vector<bool*> adjacencyMatrix;
	
	/**
	 * matrix of vertexes that will be unblocked if a vertex is unblocked
	 * For more info check Johnson's algorithm principle
	 */
	std::vector<bool*> blockMatrix;

	/**
	 * Vector of visited vertexes by circuit()
	 * has stack-like behavior, but has to be iterated-over, therefore is implemented as vector
	 * If circuit() finds startVertex, all vertexes in vector are part of a new 4circuit
	 */
    std::vector<int> visitedVertexes;
	
	/**
	 * total vertexes in state diagram
	 */
    int numberOfVertexes {0};
	
	/**
	 * list of all startingVertexes of state diagram
	 * find() starts searching for cycles vertexes of this vector
	 * after every circuit is found, adjacent vertexes are appended
	 */
	std::vector<int> startingVertexes;
	
	/**
	 * Vertex that circuit() was originally called on, serve for comparing to find end of circuit
	 */
    int startVertex {};
	
	/**
	 * Array indicating if Vertex on given index is blocked.
	 * Vertexes are blocked if they were already visited and They are unblocked when circuit is found
	 */
    bool *blocked {nullptr};
	
	/**
	 * Vector of circuits found. Is returned by find() when all circuits were found
	 */
	std::vector<std::vector<std::string>> circuits;
};

}