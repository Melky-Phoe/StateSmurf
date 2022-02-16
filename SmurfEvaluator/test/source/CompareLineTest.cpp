#include <CompareLineTest.hpp>
#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <gtest/gtest.h>

#define LOGS_PATH "testLogs/compareLines/"
using namespace state_smurf::log_evaluator;
namespace compare_line {
std::ifstream openFile(std::string filename) {
    std::ifstream file;
    file.open(LOGS_PATH + filename, std::ios_base::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open" << LOGS_PATH << filename << std::endl;
    }
    return file;
}

TEST_F(CompareLineTest, differentTime) {
    std::ifstream compare = openFile("differentTime.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareLineTest, differentApp) {
    std::ifstream compare = openFile("differentApp.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareLineTest, emptyTransition1) {
    std::ifstream compare = openFile("emptyTransition.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareLineTest, emptyTransition2) {
    std::ifstream compare1 = openFile("emptyTransition.log");
    std::ifstream compare2 = openFile("emptyTransition.log");
    EXPECT_TRUE(LogsComparer::compareFiles(compare1, compare2));
    compare1.close();
    compare2.close();
}
}