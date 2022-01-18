#include <FilterTests.hpp>

#include <bringauto/log_evaluator/Filter.hpp>

using namespace bringauto::log_evaluator;
TEST_F(FilterTests, filterFile) {
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.392] [sampleApp] [info] [StateTransition] Any log with [StateTransition] prefix");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.391] [sampleApp] [info] [StateTransition] after double prefix");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "[2021-12-20 10:05:29.392] [sampleApp] [info] [StateTransition] After other logs, also last");
    EXPECT_EQ(Filter::findNextTransitionLog(file), "");
}
