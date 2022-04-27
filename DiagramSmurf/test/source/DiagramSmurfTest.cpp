#include <state_smurf/diagram/Vertex.hpp>
#include <DiagramSmurfTest.hpp>

namespace state_smurf::diagram {

TEST_F(DiagramSmurfTest, allValid) {
	EXPECT_TRUE(stateDiagram.changeState("A")); //start
	EXPECT_TRUE(stateDiagram.changeState("A"));
	EXPECT_TRUE(stateDiagram.changeState("B"));
	EXPECT_TRUE(stateDiagram.changeState("C"));
	EXPECT_TRUE(stateDiagram.changeState("D"));
	EXPECT_TRUE(stateDiagram.changeState("A"));
	EXPECT_TRUE(stateDiagram.changeState("D"));
}

TEST_F(DiagramSmurfTest, goToSelf) {
	EXPECT_TRUE(stateDiagram.changeState("A")); //start
	EXPECT_TRUE(stateDiagram.changeState("A")); //valid
	EXPECT_TRUE(stateDiagram.changeState("B")); // just going elsewhere
	EXPECT_FALSE(stateDiagram.changeState("B"));    //invalid
}

TEST_F(DiagramSmurfTest, notExistingState) {
	EXPECT_FALSE(stateDiagram.changeState("Invalid"));
}

TEST_F(DiagramSmurfTest, StartingVertex) {
	EXPECT_FALSE(stateDiagram.changeState("D"));
	stateDiagram.setStartVertex(stateDiagram.findStateByName("D"));
	EXPECT_TRUE(stateDiagram.changeState("D"));
}

TEST_F(DiagramSmurfTest, invalidEdge) {
	EXPECT_TRUE(stateDiagram.changeState("B"));
	EXPECT_FALSE(stateDiagram.changeState("A"));    // edge in Oposite dirrection
	EXPECT_FALSE(stateDiagram.changeState("D"));    // No Edge at all
}

TEST_F(DiagramSmurfTest, stateExist) {
	EXPECT_TRUE(stateDiagram.stateExist("A"));
	EXPECT_TRUE(stateDiagram.stateExist("B"));
	EXPECT_FALSE(stateDiagram.stateExist("Invalid"));
}

}
