#include "state_smurf/log_evaluator/helpers/LineParser.hpp"
#include <gtest/gtest.h>

using namespace state_smurf::log_evaluator::helpers;
namespace compare_line {


TEST(CompareLineTest, differentTime) {
    EXPECT_TRUE(LineParser::compareLines("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state etalon", "[1980-01-01 00:00:00.000] [sampleApp] [info] [TransitionSmurf] Going to state etalon"));

}

TEST(CompareLineTest, differentApp) {
    EXPECT_FALSE(LineParser::compareLines("[1980-01-01 00:00:00.000] [sampleApp] [info] [TransitionSmurf] Going to state etalon", "[1980-01-01 00:00:00.000] [otherApp] [info] [TransitionSmurf] Going to state etalon"));
}

TEST(CompareLineTest, emptyTransition1) {
    EXPECT_FALSE(LineParser::compareLines("[1980-01-01 00:00:00.000] [sampleApp] [info] [TransitionSmurf] Going to state etalon", ""));
}

TEST(CompareLineTest, emptyTransition2) {
	EXPECT_FALSE(LineParser::compareLines("", "[1980-01-01 00:00:00.000] [sampleApp] [info] [TransitionSmurf] Going to state etalon"));
}

TEST(CompareLineTest, emptyTransition3) {
	EXPECT_TRUE(LineParser::compareLines("", ""));
}

TEST(CompareLineTest, differentCircuitNumber) {
	EXPECT_TRUE(LineParser::compareLines("\"[2021-12-20 10:05:29.392] In circuit 0: [A, B, C, ]\"", "\"[2000-10-20 10:05:00.000] In circuit 1: [A, B, C, ]\""));
}
}