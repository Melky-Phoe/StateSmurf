#include <gtest/gtest.h>

int main(int argc, char **argv) {
	// Aggregator should be tested, but I don't see a reasonable way to do it
	::testing::InitGoogleTest(&argc, argv);
	return RUN_ALL_TESTS();
}
