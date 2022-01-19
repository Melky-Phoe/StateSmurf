#include <StateTransition.h>

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::transition_smurf {
StateTransition::StateTransition(StateGraph stateGraph) {
	_stateGraph = std::move(stateGraph);
}

bool StateTransition::goToState(const std::string &stateName) {
	if (_stateGraph.changeStateByName(stateName)) {
		bringauto::logging::Logger::logInfo("[StateTransition] Going to state {}", stateName);
		return inState(stateName);
	} else {
		if (_stateGraph.stateExist(stateName)) {
			bringauto::logging::Logger::logWarning(R"([StateTransition] Couldn't change state {{ "to":{} "from": {} }})",
			                                       stateName, _stateGraph.getCurrentStateName());
		} else {
			bringauto::logging::Logger::logWarning("[StateTransition] State doesnt exist {}", stateName);
		}
		return false;
	}
}

bool StateTransition::inState(const std::string &stateName) {
	if (_stateGraph.getCurrentStateName() == stateName) {
		bringauto::logging::Logger::logInfo("[StateTransition] In state {}", _stateGraph.getCurrentStateName());
		return true;
	} else {
		bringauto::logging::Logger::logError("[StateTransition] Not in state {}, current state is {}",
		                                     stateName, _stateGraph.getCurrentStateName());
		return false;
	}
}
}