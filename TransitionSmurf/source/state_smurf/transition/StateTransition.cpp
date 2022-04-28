#include <state_smurf/transition/StateTransition.hpp>

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::transition {
	StateTransition::StateTransition(diagram::StateDiagram stateDiagram) {
		stateDiagram_ = std::move(stateDiagram);
		if (stateDiagram_.startVertexesEmpty()) {
			// In later version, this check will be part of creating diagram in compile-time
			bringauto::logging::Logger::logError("[TransitionSmurf] No starting vertex was set, TransitionSmurf is unable to work!");
			return;
		}
		printAdjacencyList();
		bringauto::logging::Logger::logDebug("[TransitionSmurf] Start of Run");
	}
	
	bool StateTransition::goToState(const std::string &stateName) {
		if (stateDiagram_.changeState(stateName)) {
			bringauto::logging::Logger::logDebug("[TransitionSmurf] Going to state {}", stateName);
			return inState(stateName);
		} else {
			if (stateDiagram_.stateExist(stateName)) {
				if (stateDiagram_.getCurrentStateName().empty()) {
					bringauto::logging::Logger::logWarning(
							R"([TransitionSmurf] Invalid Starting state {{ "state":{} }})",
							stateName);
				} else {
					bringauto::logging::Logger::logWarning(
							R"([TransitionSmurf] Couldn't change state {{ "to":{} "from": {} }})",
							stateName, stateDiagram_.getCurrentStateName());
				}
			} else {
				bringauto::logging::Logger::logWarning("[TransitionSmurf] State doesnt exist {}", stateName);
			}
			return false;
		}
	}
	
	bool StateTransition::inState(const std::string &stateName) {
		if (stateDiagram_.getCurrentStateName() == stateName) {
			return true;
		} else {
			bringauto::logging::Logger::logError("[TransitionSmurf] Not in state {}, current state is {}",
			                                     stateName, stateDiagram_.getCurrentStateName());
			return false;
		}
	}
	
	void StateTransition::printAdjacencyList() {
		auto adjacencyList = stateDiagram_.getAdjacencyList();
		
		for (const auto &it: adjacencyList) {
			std::string states = {};
			for (const auto &state: it.second) {
				states.append(state->getName());
				states.append(" ");
			}
			bringauto::logging::Logger::logDebug("[DiagramSmurf] {} :\t{}", it.first->getName(), states);
		}
		
	}
}