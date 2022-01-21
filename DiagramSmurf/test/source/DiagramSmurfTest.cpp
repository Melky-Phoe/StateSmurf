#include <state_smurf/diagram/Vertex.hpp>
#include <DiagramSmurfTest.hpp>

namespace state_smurf::diagram {

TEST_F(DiagramSmurfTest, allValid) {
	EXPECT_TRUE(stateDiagram.changeStateByName("A")); //start
	EXPECT_TRUE(stateDiagram.changeStateByName("A"));
	EXPECT_TRUE(stateDiagram.changeStateByName("B"));
	EXPECT_TRUE(stateDiagram.changeStateByName("C"));
	EXPECT_TRUE(stateDiagram.changeStateByName("D"));
	EXPECT_TRUE(stateDiagram.changeStateByName("A"));
	EXPECT_TRUE(stateDiagram.changeStateByName("D"));
}

TEST_F(DiagramSmurfTest, goToSelf) {
	EXPECT_TRUE(stateDiagram.changeStateByName("A")); //start
	EXPECT_TRUE(stateDiagram.changeStateByName("A")); //valid
	EXPECT_TRUE(stateDiagram.changeStateByName("B")); // just going elsewhere
	EXPECT_FALSE(stateDiagram.changeStateByName("B"));    //invalid
}

TEST_F(DiagramSmurfTest, notExistingState) {
	EXPECT_FALSE(stateDiagram.changeStateByName("Invalid"));
}

TEST_F(DiagramSmurfTest, invalidEdge) {
	EXPECT_TRUE(stateDiagram.changeStateByName("B"));
	EXPECT_FALSE(stateDiagram.changeStateByName("A"));    // edge in Oposite dirrection
	EXPECT_FALSE(stateDiagram.changeStateByName("D"));    // No Edge at all
}

TEST_F(DiagramSmurfTest, null) {
	EXPECT_FALSE(stateDiagram.changeState(nullptr));
}

TEST_F(DiagramSmurfTest, stateExist) {
	EXPECT_TRUE(stateDiagram.stateExist("A"));
	EXPECT_TRUE(stateDiagram.stateExist("B"));
	EXPECT_FALSE(stateDiagram.stateExist("Invalid"));
}

/*
TEST_F(SGTest, createInvalidEdge) {
    // New vertex that isn't added to StateGraph
	auto newVertex = std::make_shared<Vertex>("newVertex");
	int count = stateGraph.edges.size();
	stateGraph.setEdge(newVertex, newVertex);
	EXPECT_EQ(stateGraph.edges.size(), count);
}
*/
}
