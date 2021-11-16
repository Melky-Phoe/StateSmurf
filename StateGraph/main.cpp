#include <iostream>
#include <StateGraph.h>

int main() {
	StateGraph stateGraph;
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
	std::cout << stateGraph.getCurrentStateName() << std::endl;
	return 0;
}
