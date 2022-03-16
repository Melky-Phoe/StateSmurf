#include <state_smurf/log_evaluator/LineParser.hpp>

#include <gtest/gtest.h>

#include <string>
#include <vector>

TEST(LineParser, splitSpace) {
	std::vector<std::string> tokens;
	std::string line = "A    B";
	state_smurf::log_evaluator::LineParser::splitString(tokens, line);
	EXPECT_EQ(tokens[0], "A");
	EXPECT_EQ(tokens[1], "B");
}