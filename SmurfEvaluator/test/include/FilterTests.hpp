#pragma once

#include <gtest/gtest.h>

#include <fstream>
#include <iostream>

class FilterTests : public ::testing::Test {
protected:
    void SetUp() override {
        file.open("testLogs/findNext.log", std::ios_base::in);
        if (!file.is_open()) {
            std::cerr << "Unable to open testLogs/findNext.log" << std::endl;
        }
    }
    void TearDown() override {
        file.close();
    }
    std::ifstream file;
};

