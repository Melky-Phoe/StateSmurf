#pragma once

#include <StateTransition.h>

namespace bringauto {
/**
 * Example class using StateTransition
 */
class Drive {
public:
    /**
     * Constructor for class Drive, needs StateTransition for moving on StateGraph
     * @param transition inicialized StateTransition
     */
	explicit Drive(const std::shared_ptr<bringauto::StateTransition>& transition);

    /**
     * Changes speed, call transition to state Drive
     * @param newSpeed
     */
	void increaseSpeed(int newSpeed);

    /**
     * Changes speed to 0, call transition to state inDrive
     */
	void stop();

private:
	std::shared_ptr<bringauto::StateTransition> _transitions;
	int _speed = {0};
};
}
