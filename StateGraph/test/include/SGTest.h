#pragma once
#include <StateGraph.h>
#include <gtest/gtest.h>

class SGTest : public ::testing::Test {
protected:
	void SetUp() override {
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
	}

	StateGraph stateGraph;
};

