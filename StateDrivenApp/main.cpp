#include <StateGraph.h>
#include <StateTransition.h>

#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/ConsoleSink.hpp>
#include <bringauto/logging/FileSink.hpp>

std::shared_ptr<bringauto::StateTransition> transitions;

void connect() {
	transitions->goToState("idle");
}

int main(int argc, char **argv) {
	namespace log = bringauto::logging;
	log::Logger::addSink<log::ConsoleSink>();
	log::Logger::addSink<log::FileSink>({"./","StateTransition.log"});
	log::Logger::init({"StateTransition", log::Logger::Verbosity::Info});

	bringauto::StateGraph stateGraph;
	auto disconnected = stateGraph.addVertex("disconnected");
	auto idle = stateGraph.addVertex("idle");
	auto drive = stateGraph.addVertex("drive");
	auto inStop = stateGraph.addVertex("inStop");
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

	transitions = std::make_shared<bringauto::StateTransition >(stateGraph);

	connect();
}