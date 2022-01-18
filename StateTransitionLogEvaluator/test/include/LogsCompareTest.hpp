#pragma once

#include <gtest/gtest.h>

#include <fstream>

class LogsCompareTest : public ::testing::Test {
protected:
    void SetUp() override {
        etalon.open("../../test/testLogs/etalon.log", std::ios_base::in);
        if (!etalon.is_open()) {
            std::cerr << "Unable to open ../../test/testLogs/etalon.log" << std::endl;
        }
    }
    void TearDown() override {
        etalon.close();
    }
    std::ifstream etalon;
};
