#pragma once

#include <StateGraph.h>
#include <string>

namespace bringauto {

class StateTransition {
public:
	explicit StateTransition(StateGraph stateGraph);

	/**
	 * Changes current state
	 * @param stateName Name of state
	 * @return true if transition is possible
	 */
	bool goToState(const std::string &stateName);

	/**
	 * Compares if current state is same as param stateName, is called by goToState after change
	 * @param stateName
	 * @return true is same
	 */
	bool inState(const std::string &stateName);

	//static void setStateGraph(StateGraph stateGraph);

private:
	StateGraph _stateGraph;
};

}