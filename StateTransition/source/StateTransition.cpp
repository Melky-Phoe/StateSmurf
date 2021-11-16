#include <StateTransition.h>

#include <bringauto/logging/Logger.hpp>

StateTransition::StateTransition(StateGraph stateGraph) {
	_stateGraph = stateGraph;
}

void StateTransition::goToState(std::string stateName) {
	bringauto::logging::Logger::logInfo("Going to state {}", stateName);
	_stateGraph.changeStateByName(stateName);
	inState();
}

void StateTransition::inState() {
	bringauto::logging::Logger::logInfo("In state {}", _stateGraph.getCurrentStateName());
}
