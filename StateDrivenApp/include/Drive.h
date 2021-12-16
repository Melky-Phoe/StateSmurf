#pragma once

#include <StateTransition.h>

namespace bringauto {

class Drive {
public:
	explicit Drive(const std::shared_ptr<bringauto::StateTransition>& transition);

	void increaseSpeed(int newSpeed);
	void stop();

private:
	std::shared_ptr<bringauto::StateTransition> _transitions;
	int _speed = {0};
};
}
