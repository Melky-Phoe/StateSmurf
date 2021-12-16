#include <StateGraph.h>
#include <StateTransition.h>

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>
#include "Drive.h"

void connect(std::shared_ptr<bringauto::StateTransition> transitions) {
	transitions->goToState("idle");
}

bringauto::StateGraph createStateGraph() {
	bringauto::StateGraph stateGraph;
	/// Creating Vertexes, they are accessible by name.
	auto disconnected = stateGraph.addVertex("disconnected");
	auto idle = stateGraph.addVertex("idle");
	auto drive = stateGraph.addVertex("drive");
	auto inStop = stateGraph.addVertex("inStop");

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
	log::Logger::addSink<log::FileSink>({"./","StateTransition.log"});
	log::Logger::init({"StateTransition", log::Logger::Verbosity::Info});

	/// Creating State Graph. More in function
	bringauto::StateGraph stateGraph = createStateGraph();

	/// Transition class takes StateGraph in constructor, on which it is working on
	std::shared_ptr<bringauto::StateTransition> transitions = std::make_shared<bringauto::StateTransition>(stateGraph);

	/// Passing transitions as argument
	connect(transitions);

	bringauto::Drive drive(transitions);

	drive.increaseSpeed(10);
	drive.stop();

	/// Invalid transition
	transitions->goToState("disconnected");

}