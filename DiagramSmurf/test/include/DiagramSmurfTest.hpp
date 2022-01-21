#pragma once
#include <state_smurf/diagram/StateDiagram.hpp>
#include <gtest/gtest.h>

namespace state_smurf::diagram {

class DiagramSmurfTest : public ::testing::Test {
protected:
	void SetUp() override {
		/// This State Graph as is drawn in test/StateGraph.jpg
		auto a = stateDiagram.addVertex("A");
		auto b = stateDiagram.addVertex("B");
		auto c = stateDiagram.addVertex("C");
		auto d = stateDiagram.addVertex("D");
		stateDiagram.setEdge(a, b);
		stateDiagram.setEdge(b, c);
		stateDiagram.setEdge(c, d);
		stateDiagram.setEdge(a, d);
		stateDiagram.setEdge(a, a);
		stateDiagram.setEdge(d, a);
	}

	StateDiagram stateDiagram;
};

}