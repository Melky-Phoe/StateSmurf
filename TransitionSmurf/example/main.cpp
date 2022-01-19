#include <StateGraph.h>
#include <StateTransition.h>

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

int main() {
    /// Creating State Graph
    state_smurf::graph_smurf::StateGraph stateGraph;
	auto a = stateGraph.addVertex("A");
	auto b = stateGraph.addVertex("B");
	auto c = stateGraph.addVertex("C");
	auto d = stateGraph.addVertex("D");
	stateGraph.setEdge(a, b);
	stateGraph.setEdge(b, c);
	stateGraph.setEdge(c, d);
	stateGraph.setEdge(a, d);
	stateGraph.setEdge(a, a);
	stateGraph.setEdge(d, a);

    /// Logger initialization
	namespace log = bringauto::logging;
	log::Logger::addSink<log::ConsoleSink>();
	log::Logger::addSink<log::FileSink>({"./","StateTransition.log"});
	log::Logger::init({"StateTransition", log::Logger::Verbosity::Info});

    /// StateTransition Init
    state_smurf::transition_smurf::StateTransition transitions(stateGraph);
    /// Moving on StateGraph
	transitions.goToState("Invalid");
	transitions.goToState("D");
	transitions.goToState("A");
	transitions.goToState("A");
	transitions.goToState("C");
	transitions.goToState("B");

	return 0;
}
