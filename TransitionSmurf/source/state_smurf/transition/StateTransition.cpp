#include <state_smurf/transition/StateTransition.hpp>

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::transition {
StateTransition::StateTransition(diagram::StateDiagram stateGraph) {
	_stateGraph = std::move(stateGraph);
	printAdjacencyList();
    bringauto::logging::Logger::logDebug("[TransitionSmurf] Start of Run");
}

bool StateTransition::goToState(const std::string &stateName) {
	if (_stateGraph.changeStateByName(stateName)) {
		bringauto::logging::Logger::logDebug("[TransitionSmurf] Going to state {}", stateName);
		return inState(stateName);
	} else {
		if (_stateGraph.stateExist(stateName)) {
			if (_stateGraph.getCurrentStateName().empty()) {
				bringauto::logging::Logger::logWarning(R"([TransitionSmurf] Invalid Starting state {{ "state":{} }})",
													   stateName);
			} else {
				bringauto::logging::Logger::logWarning(
						R"([TransitionSmurf] Couldn't change state {{ "to":{} "from": {} }})",
						stateName, _stateGraph.getCurrentStateName());
			}
		} else {
			bringauto::logging::Logger::logWarning("[TransitionSmurf] State doesnt exist {}", stateName);
		}
		return false;
	}
}

bool StateTransition::inState(const std::string &stateName) {
	if (_stateGraph.getCurrentStateName() == stateName) {
		//bringauto::logging::Logger::logInfo("[TransitionSmurf] In state {}", _stateGraph.getCurrentStateName());
		return true;
	} else {
		bringauto::logging::Logger::logError("[TransitionSmurf] Not in state {}, current state is {}",
		                                     stateName, _stateGraph.getCurrentStateName());
		return false;
	}
}

void StateTransition::printAdjacencyList() {
	auto adjacencyList = _stateGraph.getAdjacencyList();
	
	for (const auto &it : adjacencyList) {
		std::string states = {};
		for (const auto &state : it.second) {
			states.append(state->getName());
			states.append(" ");
		}
		bringauto::logging::Logger::logDebug("[DiagramSmurf] {} :\t{}", it.first->getName(), states);
	}
	
}
}