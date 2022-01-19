#pragma once

#include <gtest/gtest.h>

#include <fstream>

class CompareLineTest : public ::testing::Test {
protected:
    void SetUp() override {
        etalon.open("../../test/testLogs/compareLines/etalon.log", std::ios_base::in);
        if (!etalon.is_open()) {
            std::cerr << "Unable to open ../../test/testLogs/compareLines/etalon.log" << std::endl;
        }
    }
    void TearDown() override {
        etalon.close();
    }
    std::ifstream etalon;
};
