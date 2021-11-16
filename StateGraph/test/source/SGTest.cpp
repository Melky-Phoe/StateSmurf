#include "SGTest.h"

TEST_F(SGTest, allValid) {
	EXPECT_TRUE(stateGraph.changeStateByName("A")); //start
	EXPECT_TRUE(stateGraph.changeStateByName("A"));
	EXPECT_TRUE(stateGraph.changeStateByName("B"));
	EXPECT_TRUE(stateGraph.changeStateByName("C"));
	EXPECT_TRUE(stateGraph.changeStateByName("D"));
	EXPECT_TRUE(stateGraph.changeStateByName("A"));
	EXPECT_TRUE(stateGraph.changeStateByName("D"));
}

TEST_F(SGTest, goToSelf) {
	EXPECT_TRUE(stateGraph.changeStateByName("A")); //start
	EXPECT_TRUE(stateGraph.changeStateByName("A")); //valid
	EXPECT_TRUE(stateGraph.changeStateByName("B")); // just going elsewhere
	EXPECT_FALSE(stateGraph.changeStateByName("B"));    //invalid
}

TEST_F(SGTest, notExistingState) {
	EXPECT_FALSE(stateGraph.changeStateByName("Invalid"));
}

TEST_F(SGTest, invalidEdge) {
	EXPECT_TRUE(stateGraph.changeStateByName("B"));
	EXPECT_FALSE(stateGraph.changeStateByName("A"));    // edge in Oposite dirrection
	EXPECT_FALSE(stateGraph.changeStateByName("D"));    // No Edge at all
}

TEST_F(SGTest, null) {
	EXPECT_FALSE(stateGraph.changeState(nullptr));
}

TEST_F(SGTest, stateExist) {
	EXPECT_TRUE(stateGraph.stateExist("A"));
	EXPECT_TRUE(stateGraph.stateExist("B"));
	EXPECT_FALSE(stateGraph.stateExist("Invalid"));
}