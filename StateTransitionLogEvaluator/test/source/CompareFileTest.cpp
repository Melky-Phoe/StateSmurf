#include <CompareFileTest.hpp>
#include <bringauto/log_evaluator/LogsComparer.hpp>
#include <gtest/gtest.h>

#include <fstream>

#define LOGS_PATH "../../test/testLogs/"
using namespace bringauto::log_evaluator;
namespace compare_file {
std::ifstream openFile(std::string path) {
    std::ifstream file;
    file.open(path, std::ios_base::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open" << path << std::endl;
    }
    return file;
}

TEST_F(CompareFileTest, shorterCompared) {
    std::ifstream shorterCompare = openFile("../../test/testLogs/shorter.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, shorterCompare));
    shorterCompare.close();
}

TEST_F(CompareFileTest, longerCompared) {
    std::ifstream longerCompare = openFile("../../test/testLogs/longer.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, longerCompare));
    longerCompare.close();
}

TEST_F(CompareFileTest, sameFiles) {
    std::ifstream compare = openFile("../../test/testLogs/etalon.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, differentFiles) {
    std::ifstream compare = openFile("../../test/testLogs/different.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, corruptedFile) {
    std::ifstream compare = openFile("../../test/testLogs/corrupted.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, notExistingFile) {
    // TODO je tento test potreba? O otevirani souboru se stara nekdo jiny
    std::ifstream compare = openFile("notExisting.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, longLine) {
    std::ifstream compare = openFile("../../test/testLogs/longLine.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, emptyFile) {
    std::ifstream compare = openFile("../../test/testLogs/empty.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));
    compare.close();
}

TEST_F(CompareFileTest, emptyEtalon) {
    std::ifstream compare = openFile("../../test/testLogs/empty.log");
    EXPECT_FALSE(LogsComparer::compareFiles(compare, etalon));
    compare.close();
}

TEST_F(CompareFileTest, bothEmpty) {
    std::ifstream compare1 = openFile("../../test/testLogs/empty.log");
    std::ifstream compare2 = openFile("../../test/testLogs/empty.log");
    EXPECT_TRUE(LogsComparer::compareFiles(compare1, compare2));
    compare1.close();
    compare2.close();
}
}