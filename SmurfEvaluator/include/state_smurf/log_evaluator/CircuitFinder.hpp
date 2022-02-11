#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/diagram/Vertex.hpp>

#include <stack>

/// Algorithms for finding circuits in SmurfDiagram based on Johnson's algorithm
namespace state_smurf::log_evaluator {

class CircuitFinder {
public:
	explicit CircuitFinder(diagram::StateDiagram stateDiagram);
	~CircuitFinder() { free(blocked); blocked = nullptr;};
	void find();

private:
    void unblock(const std::shared_ptr<diagram::Vertex>& vertex);

    bool circuit(const std::shared_ptr<diagram::Vertex>& vertex);

    static long getVertexIndex(const std::vector<std::shared_ptr<diagram::Vertex>>& vertexVector, const std::shared_ptr<diagram::Vertex>& vertex);
	
    std::map<std::shared_ptr<diagram::Vertex>, std::vector<std::shared_ptr<diagram::Vertex>>> adjacencyMap;
    std::vector<std::shared_ptr<diagram::Vertex>> keys;
    std::vector<std::vector<std::shared_ptr<diagram::Vertex>>> blockMap;
    std::vector<std::shared_ptr<diagram::Vertex>> stack;
    ulong numberOfVertexes;
	std::vector<std::shared_ptr<diagram::Vertex>> startingVertexes;
    std::shared_ptr<diagram::Vertex> startVertex;
    bool *blocked;
};

}