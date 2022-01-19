#include <state_smurf/example/Drive.h>

#include <bringauto/logging/Logger.hpp>

namespace bringauto {

Drive::Drive(const std::shared_ptr<bringauto::StateTransition>& transition) {
	_transitions = transition;

}

void Drive::increaseSpeed(int newSpeed) {
	/// Changes state
	_transitions->goToState("drive");

	/// Do desired stuff
	_speed = newSpeed;
	logging::Logger::logInfo("Going in speed {}", _speed);
}

void Drive::stop() {
	_speed = 0;
	_transitions->goToState("inStop");
}

}