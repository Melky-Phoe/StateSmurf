#include <state_smurf/transition/StateTransition.hpp>

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::transition {
StateTransition::StateTransition(diagram::StateDiagram stateGraph) {
	_stateGraph = std::move(stateGraph);
    bringauto::logging::Logger::logDebug("[TransitionSmurf] Start of Run");
}

bool StateTransition::goToState(const std::string &stateName) {
	if (_stateGraph.changeStateByName(stateName)) {
		bringauto::logging::Logger::logDebug("[TransitionSmurf] Going to state {}", stateName);
		return inState(stateName);
	} else {
		if (_stateGraph.stateExist(stateName)) {
			bringauto::logging::Logger::logWarning(R"([TransitionSmurf] Couldn't change state {{ "to":{} "from": {} }})",
			                                       stateName, _stateGraph.getCurrentStateName());
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
}