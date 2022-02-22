#include <state_smurf/example/Drive.h>

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

#include <string>

void connect(const std::shared_ptr<state_smurf::transition::StateTransition>& transitions) {
	transitions->goToState("idle");
}

state_smurf::diagram::StateDiagram createStateDiagram() {
    state_smurf::diagram::StateDiagram stateGraph;
	/// Creating Vertexes, they are accessible by name.
	auto disconnected = stateGraph.addVertex("disconnected");
	auto idle = stateGraph.addVertex("idle");
	auto drive = stateGraph.addVertex("drive");
	auto stop = stateGraph.addVertex("stop");
	
	/// Every State diagram must have at least one starting vertex
	stateGraph.setStartVertex(disconnected);

	/// Setting Edges, from each state to every state it can transition
	stateGraph.setEdge(disconnected, idle);
	stateGraph.setEdge(idle, drive);
	stateGraph.setEdge(idle, stop);
	stateGraph.setEdge(idle, disconnected);

	stateGraph.setEdge(drive, stop);
	stateGraph.setEdge(drive, drive);

	stateGraph.setEdge(stop, idle);
	stateGraph.setEdge(stop, stop);
	stateGraph.setEdge(stop, drive);

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

	// parsing arguments
	int targetSpeed = 0;
	if (argc == 3) {
		if (strcmp(argv[1], "--speed") == 0) {
			targetSpeed = std::stol(argv[2]);
		}
	}
	
	transitions->goToState("disconnected");
	
	/// Passing transitions as argument -> not recommended approach
	connect(transitions);

	state_smurf::example::Drive drive(transitions);
	
	for (int i = 0; i < targetSpeed; i++) {
		drive.increaseSpeed(i);
		sleep(1);
	}
	
	/// Invalid transition returns false
	if (!transitions->goToState("idle")) {
		bringauto::logging::Logger::logWarning("Cant go to while driving idle, stopping...");
		drive.stop();
		transitions->goToState("idle");
	}
	transitions->goToState("disconnected");
}