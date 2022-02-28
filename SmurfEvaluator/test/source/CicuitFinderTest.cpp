#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

TEST(CircuitFinder, emptyDiagram) {
	std::ifstream file("testLogs/CircuitDiagrams/invalid/emptyDiagram");
	EXPECT_TRUE(file.is_open());
	testing::internal::CaptureStderr();
	new state_smurf::log_evaluator::CircuitFinder(file);
	EXPECT_EQ("ERROR: Invalid state diagram\n", testing::internal::GetCapturedStderr());
}

TEST(CircuitFinder, noStart) {
	std::ifstream file("testLogs/CircuitDiagrams/invalid/noStart");
	EXPECT_TRUE(file.is_open());
	testing::internal::CaptureStderr();
	new state_smurf::log_evaluator::CircuitFinder(file);
	EXPECT_EQ("ERROR: no starting vertexes\n", testing::internal::GetCapturedStderr());
}

TEST(CircuitFinder, simpleCircuit) {
	std::ifstream file("testLogs/CircuitDiagrams/finder/simpleCircuit");
	if (!file.is_open()) {
		std::cerr << "Unable to open simpleCircuit" << std::endl;
	}
	state_smurf::log_evaluator::CircuitFinder CF(file);
	std::vector<std::vector<std::string>> constCircuits;
	std::vector<std::string> line;
	line.emplace_back("A");
	line.emplace_back("B");
	line.emplace_back("C");
	constCircuits.push_back(line);
	auto circuits = CF.find();
	EXPECT_EQ(constCircuits, circuits);
}

TEST(CircuitFinder, allPossibleCircuits) {
	std::ifstream file("testLogs/CircuitDiagrams/finder/allPossibleCircuits");
	if (!file.is_open()) {
		std::cerr << "Unable to open allPossibleCircuit" << std::endl;
	}
	state_smurf::log_evaluator::CircuitFinder CF(file);
	std::vector<std::vector<std::string>> constCircuits;
	std::vector<std::string> line;
	line.emplace_back("A");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("B");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("B");
	line.emplace_back("C");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("B");
	line.emplace_back("C");
	line.emplace_back("D");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("C");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("C");
	line.emplace_back("B");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("A");
	line.emplace_back("C");
	line.emplace_back("D");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("B");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("B");
	line.emplace_back("C");
	constCircuits.push_back(line);
	line.clear();
	line.emplace_back("C");
	constCircuits.push_back(line);
	line.clear();
	auto circuits = CF.find();
	EXPECT_EQ(constCircuits, circuits);
}

TEST(CircuitFinder, noCircuit) {
	std::ifstream file("testLogs/CircuitDiagrams/finder/noCircuit");
	if (!file.is_open()) {
	std::cerr << "Unable to open noCircuit" << std::endl;
	}
	state_smurf::log_evaluator::CircuitFinder CF(file);
	std::vector<std::vector<std::string>> constCircuits;
	auto circuits = CF.find();
	EXPECT_EQ(constCircuits, circuits);
}

TEST(CircuitFinder, multiStart) {
	std::ifstream file("testLogs/CircuitDiagrams/finder/multiStart");
	if (!file.is_open()) {
	std::cerr << "Unable to open multiStart" << std::endl;
	}
	state_smurf::log_evaluator::CircuitFinder CF(file);
	std::vector<std::vector<std::string>> constCircuits;
	std::vector<std::string> line;
	line.emplace_back("A");
	line.emplace_back("B");
	line.emplace_back("C");
	constCircuits.push_back(line);
	auto circuits = CF.find();
	EXPECT_EQ(constCircuits, circuits);
}