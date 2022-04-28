#include <state_smurf/diagram/StateDiagram.hpp>

int main() {
	state_smurf::diagram::StateDiagram stateGraph;
	auto a = stateGraph.addVertex("A");
	auto b = stateGraph.addVertex("B");
	auto c = stateGraph.addVertex("C");
	// This will add Vertex, but lose pointer to it therefore we can't use it later
	stateGraph.addVertex("D");

	stateGraph.setStartVertex(a);
	stateGraph.setEdge(a, b);
	stateGraph.setEdge(b, c);
	// Changing states by name
	stateGraph.changeState("A");
	stateGraph.changeState("B");
	// Changing states by pointer
	stateGraph.changeState(b);
	stateGraph.changeState(c);
	return 0;
}