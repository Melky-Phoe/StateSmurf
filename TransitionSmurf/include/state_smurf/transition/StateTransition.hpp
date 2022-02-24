#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <string>

namespace state_smurf::transition {
/**
 * Class controlling state transition logic of predefined State diagram implemented by DiagramSmurf
 * StateDiagram is passed to StateTransition in constructor
 * StateTransition is using Bringauto Logger library, that has to be initialized before use
 */
class StateTransition {
public:

	explicit StateTransition(diagram::StateDiagram stateGraph);

	/**
	 * Method used for moving around state diagram. Changes current state if possible
	 * If transition is possible, logs info: Going to state stateName
	 * @param stateName Name of state
	 * @return true if transition is possible
	 */
	bool goToState(const std::string &stateName);
private:
	/**
	 * Compares if current state is same as in param stateName, is called by goToState after change
	 * Serves as guard for multithreading applications
	 * @param stateName
	 * @return true is same, meaning transition was successful
	 */
	bool inState(const std::string &stateName);

	/**
	 * Logs all transitions in specific parsable format
	 */
	void printAdjacencyList();
	
    diagram::StateDiagram _stateGraph;
};

}