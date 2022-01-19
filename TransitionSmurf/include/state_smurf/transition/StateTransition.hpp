#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <string>

namespace state_smurf::transition {
/**
 * Class controlling state transition logic of predefined StateGraph
 * StateGraph is passed to StateTransition in constructor
 * StateTransition is using Bringauto Logger library, that has to be initialized before use
 */
class StateTransition {
public:

	explicit StateTransition(diagram::StateDiagram stateGraph);

	/**
	 * Changes current state if possible
	 * If transition is possible, logs info: Going to state stateName
	 * @param stateName Name of state
	 * @return true if transition is possible
	 */
	bool goToState(const std::string &stateName);
private:
	/**
	 * Compares if current state is same as param stateName, is called by goToState after change
	 * @param stateName
	 * @return true is same, meaning transition was successful
	 */
	bool inState(const std::string &stateName);

    diagram::StateDiagram _stateGraph;
};

}