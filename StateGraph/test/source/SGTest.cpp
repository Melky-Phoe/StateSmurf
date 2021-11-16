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
	EXPECT_FALSE(stateGraph.changeStateByName("Nevalidni"));
}

TEST_F(SGTest, invalidEdge) {
	EXPECT_TRUE(stateGraph.changeStateByName("B"));
	EXPECT_FALSE(stateGraph.changeStateByName("A"));    // Protismer
	EXPECT_FALSE(stateGraph.changeStateByName("D"));    // Bez souvislosti
}