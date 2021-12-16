#include <Drive.h>

namespace bringauto {

Drive::Drive(const std::shared_ptr<bringauto::StateTransition>& transition) {
	_transitions = transition;

}

void Drive::increaseSpeed(int newSpeed) {
	_transitions->goToState("drive");
	_speed = newSpeed;
}

void Drive::stop() {
	_speed = 0;
	_transitions->goToState("inStop");
}

}