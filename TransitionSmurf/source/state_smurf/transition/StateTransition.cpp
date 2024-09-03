#include <state_smurf/transition/StateTransition.hpp>
#include <state_smurf/transition/StateTransitionLoggerId.hpp>

namespace state_smurf::transition {

	StateTransition::StateTransition(diagram::StateDiagram stateDiagram) {
		stateDiagram_ = std::move(stateDiagram);
		if (stateDiagram_.startVertexesEmpty()) {
			// In later version, this check will be part of creating diagram in compile-time
			Logger::logError("[TransitionSmurf] No starting vertex was set, TransitionSmurf is unable to work!");
			return;
		}
		printAdjacencyList();
		Logger::logDebug("[TransitionSmurf] Start of Run");
	}
	
	bool StateTransition::goToState(const std::string &stateName) {
		if (stateDiagram_.changeState(stateName)) {
			Logger::logDebug("[TransitionSmurf] Going to state {}", stateName);
			return inState(stateName);
		} else {
			if (stateDiagram_.stateExist(stateName)) {
				if (stateDiagram_.getCurrentStateName().empty()) {
					Logger::logWarning(
						R"([TransitionSmurf] Invalid Starting state {{ "state":{} }})",
						stateName);
				} else {
					Logger::logWarning(
						R"([TransitionSmurf] Couldn't change state {{ "to":{} "from": {} }})",
						stateName, stateDiagram_.getCurrentStateName());
				}
			} else {
				Logger::logWarning("[TransitionSmurf] State doesnt exist {}", stateName);
			}
			return false;
		}
	}
	
	bool StateTransition::inState(const std::string &stateName) {
		if (isInState(stateName)) {
			return true;
		} else {
				Logger::logError("[TransitionSmurf] Not in state {}, current state is {}",
					stateName, stateDiagram_.getCurrentStateName());

			return false;
		}
	}

	bool StateTransition::isInState(const std::string &stateName) {
		return (stateDiagram_.getCurrentStateName() == stateName);
	}
	
	void StateTransition::printAdjacencyList() {
		auto adjacencyList = stateDiagram_.getAdjacencyList();
		
		for (const auto &it: adjacencyList) {
			std::string states = {};
			for (const auto &state: it.second) {
				states.append(state->getName());
				states.append(" ");
			}
			Logger::logDebug("[DiagramSmurf] {} :\t{}", it.first->getName(), states);
		}
		
	}
}