#include <state_smurf/example/Drive.h>

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>
#include <state_smurf/transition/StateTransitionLoggerId.hpp>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

#include <cstring>
#include <thread>

void connect(const std::shared_ptr<state_smurf::transition::StateTransition> &transitions) {
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
	log::FileSink::Params fsParams = {"./", "sampleApp.log"};
	state_smurf::example::Logger::addSink<log::ConsoleSink>();
	//LoggerSample::addSink<log::FileSink>(fsParams); // Using 2 file sinks on one file does not work properly
	state_smurf::example::Logger::init({"sampleApp", log::LoggerVerbosity::Debug});
	state_smurf::transition::Logger::addSink<log::ConsoleSink>();
	state_smurf::transition::Logger::addSink<log::FileSink>(fsParams);
	state_smurf::transition::Logger::init({"StateTransition", log::LoggerVerbosity::Debug});
	
	/// Creating State Graph. More in function
	state_smurf::diagram::StateDiagram stateGraph = createStateDiagram();
	
	/// Transition class takes StateGraph in constructor, on which it is working on
	std::shared_ptr<state_smurf::transition::StateTransition> transitions = std::make_shared<state_smurf::transition::StateTransition>(
		stateGraph);
	
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
		std::this_thread::sleep_for(std::chrono::duration<double>(1));
	}
	
	/// Invalid transition returns false
	if (!transitions->goToState("idle")) {
		state_smurf::example::Logger::logWarning("Cant go to idle while driving, stopping...");
		drive.stop();
		transitions->goToState("idle");
	}
	transitions->goToState("disconnected");
}