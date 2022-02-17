#include <TransitionSmurfTests.hpp>

namespace state_smurf::transition {

TEST_F(StateTransitionTest, allValid) {
	EXPECT_TRUE(transition->goToState("A")); //start
	EXPECT_TRUE(transition->goToState("A"));
	EXPECT_TRUE(transition->goToState("B"));
	EXPECT_TRUE(transition->goToState("C"));
	EXPECT_TRUE(transition->goToState("D"));
	EXPECT_TRUE(transition->goToState("A"));
	EXPECT_TRUE(transition->goToState("D"));
}

TEST_F(StateTransitionTest, goToSelf) {
	EXPECT_TRUE(transition->goToState("A")); //start
	EXPECT_TRUE(transition->goToState("A")); //valid
	EXPECT_TRUE(transition->goToState("B")); // just going elsewhere
	EXPECT_FALSE(transition->goToState("B"));    //invalid
}

TEST_F(StateTransitionTest, notExistingState) {
	EXPECT_FALSE(transition->goToState("Invalid")); // First does not exist
	EXPECT_TRUE(transition->goToState("A"));
	EXPECT_FALSE(transition->goToState("Invalid")); // Other does not exist
}

TEST_F(StateTransitionTest, notStartingState) {
	EXPECT_FALSE(transition->goToState("D"));
}

TEST_F(StateTransitionTest, invalidEdge) {
	EXPECT_TRUE(transition->goToState("B"));
	EXPECT_FALSE(transition->goToState("A"));    // edge in Oposite dirrection
	EXPECT_FALSE(transition->goToState("D"));    // No Edge at all
}

/*
TEST_F(StateTransitionTest, inState) {
	EXPECT_FALSE(transition->inState("Ivalid"));
	EXPECT_FALSE(transition->inState("A")); // valid but not in
}
*/


}