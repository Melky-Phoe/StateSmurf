#include <FilterTests.hpp>

#include "state_smurf/log_evaluator/helpers/Filter.hpp"

using namespace state_smurf::log_evaluator::helpers;
TEST_F(FilterTests, filterFile) {
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Any log with [TransitionSmurf] prefix");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.391] [sampleApp] [info] [TransitionSmurf] Going to state after_double_prefix");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] After other logs, also last");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "");
}

TEST_F(FilterTests, notExistingFile) {
    std::ifstream invalid_file;
    invalid_file.open("invalid_path", std::ios_base::in);
    EXPECT_EQ(Filter::findNextTransitionLog(invalid_file), "");
}
