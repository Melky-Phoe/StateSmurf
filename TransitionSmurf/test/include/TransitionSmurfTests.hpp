#pragma once

#include <state_smurf/diagram/StateDiagram.hpp>
#include <state_smurf/transition/StateTransition.hpp>

#include <gtest/gtest.h>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

namespace state_smurf::transition {
	
	class StateTransitionTest : public ::testing::Test {
	protected:
		void SetUp() override {
			bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
			bringauto::logging::Logger::addSink<bringauto::logging::FileSink>({"./", "StateTransitionTest.log"});
			bringauto::logging::Logger::init({"StateTransitionTest", bringauto::logging::Logger::Verbosity::Debug});
			
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
			bringauto::logging::Logger::destroy();
		}
		
		std::shared_ptr<StateTransition> transition;
	};
	
}