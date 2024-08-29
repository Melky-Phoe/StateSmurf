#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>

#include <gtest/gtest.h>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

namespace state_smurf::transition {

	constexpr bringauto::logging::LoggerId logId = {.id = "StateTransition"};
	using Logger = bringauto::logging::Logger<logId, bringauto::logging::LoggerImpl>;
	
	class StateTransitionTest : public ::testing::Test {
	protected:
		void SetUp() override {
			Logger::addSink<bringauto::logging::ConsoleSink>();
			Logger::addSink<bringauto::logging::FileSink>({"./", "StateTransitionTest.log"});
			Logger::init({"StateTransition", bringauto::logging::LoggerVerbosity::Debug});
			
			diagram::StateDiagram stateDiagram;
			auto a = stateDiagram.addVertex("A");
			auto b = stateDiagram.addVertex("B");
			auto c = stateDiagram.addVertex("C");
			auto d = stateDiagram.addVertex("D");
			stateDiagram.setStartVertex(a);
			stateDiagram.setStartVertex(b);
			stateDiagram.setEdge(a, b);
			stateDiagram.setEdge(b, c);
			stateDiagram.setEdge(c, d);
			stateDiagram.setEdge(a, d);
			stateDiagram.setEdge(a, a);
			stateDiagram.setEdge(d, a);
			transition = std::make_shared<StateTransition>(stateDiagram);
		}
		
		void TearDown() override {
			Logger::destroy();
		}
		
		std::shared_ptr<StateTransition> transition;
	};
	
}