#pragma once

#include <bringauto/logging/Logger.hpp>

namespace state_smurf::transition {

	constexpr bringauto::logging::LoggerId logId = {.id = "StateTransition"};
	using Logger = bringauto::logging::Logger<logId, bringauto::logging::LoggerImpl>;

}
