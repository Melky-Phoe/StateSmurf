#include <LogsCompareTest.hpp>
#include <bringauto/log_evaluator/LogsComparer.hpp>
#include <gtest/gtest.h>

#include <fstream>

using namespace bringauto::log_evaluator;

TEST(LogsComparer, shorterCompared) {
    std::ifstream etalon;
    etalon.open("../../test/testLogs/etalon.log", std::ios_base::in);
    if (!etalon.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
    }
    std::ifstream shorterCompare;
    shorterCompare.open("../../test/testLogs/shorter.log", std::ios_base::in);
    if (!shorterCompare.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/shorter.log" << std::endl;
    }
    EXPECT_FALSE(LogsComparer::compareFiles(etalon, shorterCompare));

    etalon.close();
    shorterCompare.close();
}

TEST(LogsComparer, longerCompared) {
    std::ifstream etalon;
    etalon.open("../../test/testLogs/etalon.log", std::ios_base::in);
    if (!etalon.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
    }
    std::ifstream longerCompare;
    longerCompare.open("../../test/testLogs/longer.log", std::ios_base::in);
    if (!longerCompare.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/longer.log" << std::endl;
    }

    EXPECT_FALSE(LogsComparer::compareFiles(etalon, longerCompare));

    etalon.close();
    longerCompare.close();
}

TEST(LogsComparer, sameFiles) {
    std::ifstream etalon;
    etalon.open("../../test/testLogs/etalon.log", std::ios_base::in);
    if (!etalon.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
    }
    std::ifstream compare;
    compare.open("../../test/testLogs/etalon.log", std::ios_base::in);
    if (!compare.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
    }

    EXPECT_TRUE(LogsComparer::compareFiles(etalon, compare));

    etalon.close();
    compare.close();
}

TEST(LogsComparer, differentFiles) {
    std::ifstream etalon;
    etalon.open("../../test/testLogs/etalon.log", std::ios_base::in);
    if (!etalon.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
    }
    std::ifstream compare;
    compare.open("../../test/testLogs/different.log", std::ios_base::in);
    if (!compare.is_open()) {
        std::cerr << "Unable to open ../../test/testLogs/different.log" << std::endl;
    }

    EXPECT_FALSE(LogsComparer::compareFiles(etalon, compare));

    etalon.close();
    compare.close();
}