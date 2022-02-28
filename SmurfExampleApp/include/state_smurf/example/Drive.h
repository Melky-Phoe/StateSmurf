#pragma once

#include <state_smurf/transition/StateTransition.hpp>
#include <memory>

namespace state_smurf::example {
	/**
	 * Example class using TransitionSmurf
	 */
	class Drive {
	public:
		/**
		 * Constructor for class Drive, needs TransitionSmurf for moving on StateGraph
		 * @param transition inicialized StateTransition
		 */
		explicit Drive(const std::shared_ptr<state_smurf::transition::StateTransition> &transition);
		
		/**
		 * Changes speed, call transition to state Drive
		 * @param newSpeed
		 */
		void increaseSpeed(int newSpeed);
		
		/**
		 * Changes speed to 0, call transition to state inStop
		 */
		void stop();
	
	private:
		std::shared_ptr<state_smurf::transition::StateTransition> _transitions;
		int _speed = {0};
	};
}
