#pragma once
#include <StateGraph.h>
#include <StateTransition.h>

#include <gtest/gtest.h>
#include <bringauto/logging/Logger.hpp>
#include <bringauto/logging/FileSink.hpp>
#include <bringauto/logging/ConsoleSink.hpp>

namespace bringauto {

class StateTransitionTest : public ::testing::Test {
protected:
	void SetUp() override {
		bringauto::logging::Logger::addSink<bringauto::logging::ConsoleSink>();
		bringauto::logging::Logger::addSink<bringauto::logging::FileSink>({"./", "StateTransitionTest.log"});
		bringauto::logging::Logger::init({"StateTransitionTest", bringauto::logging::Logger::Verbosity::Info});

		StateGraph stateGraph;
		auto a = stateGraph.addVertex("A");
		auto b = stateGraph.addVertex("B");
		auto c = stateGraph.addVertex("C");
		auto d = stateGraph.addVertex("D");
		stateGraph.setEdge(a, b);
		stateGraph.setEdge(b, c);
		stateGraph.setEdge(c, d);
		stateGraph.setEdge(a, d);
		stateGraph.setEdge(a, a);
		stateGraph.setEdge(d, a);
		transition = std::make_shared<StateTransition>(stateGraph);
	}

	void TearDown() override {
		bringauto::logging::Logger::destroy();
	}

	std::shared_ptr<StateTransition> transition;
};

}