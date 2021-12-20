#include <iostream>
#include <StateGraph.h>

int main() {
	bringauto::StateGraph stateGraph;
	auto a = stateGraph.addVertex("A");
	auto b = stateGraph.addVertex("B");
	auto c = stateGraph.addVertex("C");
	stateGraph.addVertex("D");
	stateGraph.setEdge(a, b);
	stateGraph.setEdge(b, c);
	stateGraph.changeStateByName("A");
	stateGraph.changeStateByName("B");
	stateGraph.changeState(b);
	stateGraph.changeState(c);
	return 0;
}
