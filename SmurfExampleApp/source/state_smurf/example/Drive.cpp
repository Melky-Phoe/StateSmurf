#include <state_smurf/example/Drive.h>

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::example {

	constexpr bringauto::logging::LoggerId logId = {.id = "sampleApp"};
	using Logger = bringauto::logging::Logger<logId, bringauto::logging::LoggerImpl>;
	
	Drive::Drive(const std::shared_ptr<state_smurf::transition::StateTransition> &transition) {
		_transitions = transition;
		
	}
	
	void Drive::increaseSpeed(int newSpeed) {
		/// Changes state
		_transitions->goToState("drive");
		
		/// Do desired stuff
		_speed = newSpeed;
		Logger::logInfo("Going in speed {}", _speed);
	}
	
	void Drive::stop() {
		_speed = 0;
		_transitions->goToState("stop");
	}
	
}