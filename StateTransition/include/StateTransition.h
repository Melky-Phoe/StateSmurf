#pragma once

#include <StateGraph.h>
#include <string>


class StateTransition {
public:
	StateTransition(StateGraph stateGraph);
	void goToState(std::string stateName);
	void inState();

private:
	StateGraph _stateGraph;
};

