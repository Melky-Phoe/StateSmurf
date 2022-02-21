#include <state_smurf/example/Drive.h>

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

void connect(const std::shared_ptr<state_smurf::transition::StateTransition>& transitions) {
	transitions->goToState("idle");
}

state_smurf::diagram::StateDiagram createStateDiagram() {
    state_smurf::diagram::StateDiagram stateGraph;
	/// Creating Vertexes, they are accessible by name.
	auto disconnected = stateGraph.addVertex("disconnected");
	auto idle = stateGraph.addVertex("idle");
	auto drive = stateGraph.addVertex("drive");
	auto inStop = stateGraph.addVertex("inStop");
	
	stateGraph.setStartVertex(disconnected);

	/// Setting Edges, from each state to every state it can transition
	stateGraph.setEdge(disconnected, idle);
	stateGraph.setEdge(idle, drive);
	stateGraph.setEdge(idle, inStop);
	stateGraph.setEdge(idle, disconnected);

	stateGraph.setEdge(drive, idle);
	stateGraph.setEdge(drive, inStop);
	stateGraph.setEdge(drive, drive);

	stateGraph.setEdge(inStop, idle);
	stateGraph.setEdge(inStop, inStop);
	stateGraph.setEdge(inStop, drive);

	return stateGraph;
}

int main(int argc, char **argv) {
	/// State Transitions are using BALogger. You have to initialize before using it,
	namespace log = bringauto::logging;
	log::Logger::addSink<log::ConsoleSink>();
	log::Logger::addSink<log::FileSink>({"./","sampleApp.log"});
	log::Logger::init({"sampleApp", log::Logger::Verbosity::Debug});

	/// Creating State Graph. More in function
    state_smurf::diagram::StateDiagram stateGraph = createStateDiagram();

	/// Transition class takes StateGraph in constructor, on which it is working on
	std::shared_ptr<state_smurf::transition::StateTransition> transitions = std::make_shared<state_smurf::transition::StateTransition>(stateGraph);

	transitions->goToState("disconnected");
	/// Passing transitions as argument
	connect(transitions);

	state_smurf::example::Drive drive(transitions);

	drive.increaseSpeed(10);
	drive.increaseSpeed(15);
	drive.stop();

	/// Invalid transition returns false
	if (!transitions->goToState("disconnected")) {
		bringauto::logging::Logger::logWarning("Unsuccessful transition");
		drive.stop();
	}

}