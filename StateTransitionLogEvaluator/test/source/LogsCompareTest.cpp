#include <LogsCompareTest.hpp>
#include <bringauto/log_evaluator/LogsComparer.hpp>
#include <gtest/gtest.h>

#include <fstream>

using namespace bringauto::log_evaluator;

std::ifstream openFile(std::string path) {
    std::ifstream file;
    file.open(path, std::ios_base::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open" << path << std::endl;
    }
    return file;
}
TEST_F(LogsCompareTest, shorterCompared) {
    std::ifstream shorterCompare = openFile("../../test/testLogs/shorter.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, shorterCompare));
    shorterCompare.close();
}

TEST_F(LogsCompareTest, longerCompared) {
    std::ifstream longerCompare = openFile("../../test/testLogs/longer.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, longerCompare));
    longerCompare.close();
}

TEST_F(LogsCompareTest, sameFiles) {
    std::ifstream compare = openFile("../../test/testLogs/etalon.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(LogsCompareTest, differentFiles) {
    std::ifstream compare = openFile("../../test/testLogs/different.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(LogsCompareTest, corruptedFile) {
    std::ifstream compare = openFile("../../test/testLogs/corrupted.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(LogsCompareTest, notExistingFile) {
    std::ifstream compare = openFile("notExisting.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(LogsCompareTest, missingReadRights) {
    std::ifstream compare = openFile("../../test/testLogs/missingRights.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(LogsCompareTest, longLine) {
    std::ifstream compare = openFile("../../test/testLogs/longLine.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

