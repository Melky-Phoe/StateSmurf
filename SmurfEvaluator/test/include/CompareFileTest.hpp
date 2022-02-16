#pragma once

#include <gtest/gtest.h>

#include <fstream>

class CompareFileTest : public ::testing::Test {
protected:
    void SetUp() override {
        etalon.open("testLogs/etalon.log", std::ios_base::in);
        if (!etalon.is_open()) {
            std::cerr << "Unable to open testLogs/etalon.log" << std::endl;
        }
    }
    void TearDown() override {
        etalon.close();
    }
    std::ifstream etalon;
};
