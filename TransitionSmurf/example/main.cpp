#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>
#include <state_smurf/transition/StateTransitionLoggerId.hpp>

#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>


int main() {
    /// Creating State Graph
	state_smurf::diagram::StateDiagram stateDiagram;
	/// Creating Vertexes, they are accessible by name.
	auto connected = stateDiagram.addVertex("Connected");
	auto carStatusSent = stateDiagram.addVertex("CarStatusSent");
	auto carCommandReceived = stateDiagram.addVertex("CarCommandReceived");
	auto startReceived = stateDiagram.addVertex("StartReceived");
	auto stopReceived = stateDiagram.addVertex("StopReceived");
	auto idle = stateDiagram.addVertex("Idle");
	auto drive = stateDiagram.addVertex("Drive");
	auto inStop = stateDiagram.addVertex("InStop");
	
	stateDiagram.setStartVertex(idle);
	/// Setting Edges, from each state to every state it can transition
	stateDiagram.setEdge(connected, carStatusSent);
	
	stateDiagram.setEdge(carStatusSent, carCommandReceived);
	stateDiagram.setEdge(carCommandReceived, stopReceived);
	stateDiagram.setEdge(carCommandReceived, startReceived);
	
	stateDiagram.setEdge(stopReceived, idle);
	stateDiagram.setEdge(startReceived, idle);
	stateDiagram.setEdge(startReceived, drive);
	stateDiagram.setEdge(startReceived, inStop);
	
	stateDiagram.setEdge(idle, connected);
	stateDiagram.setEdge(idle, carStatusSent);
	stateDiagram.setEdge(drive, carStatusSent);
	stateDiagram.setEdge(drive, inStop);
	stateDiagram.setEdge(inStop, carStatusSent);

    /// Logger initialization
	namespace log = bringauto::logging;
	state_smurf::transition::Logger::addSink<log::ConsoleSink>();
	state_smurf::transition::Logger::addSink<log::FileSink>({"./","StateTransition.log"});
	state_smurf::transition::Logger::init({"StateTransition", log::LoggerVerbosity::Debug});

    /// StateTransition Init
    state_smurf::transition::StateTransition transitions(stateDiagram);
    /// Moving on StateGraph
	transitions.goToState("Invalid");
	transitions.goToState("Idle");
	transitions.goToState("CarStatusSent");
	transitions.goToState("CarCommandReceived");
	transitions.goToState("StartReceived");
	transitions.goToState("Drive");
	transitions.goToState("InStop");
	

	return 0;
}
