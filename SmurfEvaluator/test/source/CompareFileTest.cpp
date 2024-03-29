#include <CompareFileTest.hpp>
#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <gtest/gtest.h>

#include <fstream>

#define LOGS_PATH "testLogs/"
using namespace state_smurf::log_evaluator;
namespace compare_file {
std::ifstream openFile(std::string filename) {
    std::ifstream file;
    file.open(LOGS_PATH + filename, std::ios_base::in);
    if (!file.is_open()) {
        std::cerr << "Unable to open " << LOGS_PATH << filename << std::endl;
    }
    return file;
}

TEST_F(CompareFileTest, shorterCompared) {
    std::ifstream shorterCompare = openFile("shorter.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, shorterCompare, ""));
    shorterCompare.close();
}

TEST_F(CompareFileTest, longerCompared) {
    std::ifstream longerCompare = openFile("longer.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, longerCompare, ""));
    longerCompare.close();
}

TEST_F(CompareFileTest, sameFiles) {
    std::ifstream compare = openFile("etalon.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, differentFiles) {
    std::ifstream compare = openFile("different.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, emptyFile) {
    std::ifstream compare = openFile("empty.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, emptyEtalon) {
    std::ifstream compare = openFile("empty.log");
    EXPECT_FALSE(LogsComparer::compareFiles(compare, etalon, ""));
    compare.close();
}

TEST_F(CompareFileTest, bothEmpty) {
    std::ifstream compare1 = openFile("empty.log");
    std::ifstream compare2 = openFile("empty.log");
    EXPECT_FALSE(LogsComparer::compareFiles(compare1, compare2, ""));
    compare1.close();
    compare2.close();
}

TEST_F(CompareFileTest, sameRuns) {
    std::ifstream compare = openFile("multipleRuns/sameRun.log");
    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, longerRun) {
    std::ifstream compare = openFile("multipleRuns/longerRun.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, shorterRun) {
    std::ifstream compare = openFile("multipleRuns/shorterRun.log");
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare, ""));
    compare.close();
}

TEST_F(CompareFileTest, aggregatedFile) {
	std::ifstream aggregatedEtalon = openFile("aggregated");
	std::ifstream original = openFile("sourceOfAggregated.log");

	EXPECT_TRUE(LogsComparer::compareFiles(aggregatedEtalon, original, ""));
	aggregatedEtalon.clear();
	aggregatedEtalon.seekg(std::ios::beg);
	original.clear();
	original.seekg(std::ios::beg);
	EXPECT_TRUE(LogsComparer::compareFiles(original, aggregatedEtalon, ""));

	aggregatedEtalon.close();
}

TEST_F(CompareFileTest, CreateAggregatedFile) {
	std::ifstream empty;
	std::ifstream etalonOriginal = openFile("sourceOfAggregated.log");

	EXPECT_TRUE(LogsComparer::compareFiles(etalonOriginal, empty, LOGS_PATH "testGenerated/"));

	etalonOriginal.clear();
	etalonOriginal.seekg(std::ios::beg);
	std::ifstream aggregated = openFile("testGenerated/etalon");

	EXPECT_TRUE(LogsComparer::compareFiles(aggregated, etalonOriginal, ""));
	etalonOriginal.close();
	aggregated.close();
}

}