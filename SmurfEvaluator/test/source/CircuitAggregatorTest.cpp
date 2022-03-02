#include <state_smurf/log_evaluator/CircuitAggregator.hpp>

#include <gtest/gtest.h>

#include <fstream>
#include <string>
#include <vector>

#define LOG_PATH "testLogs/CircuitDiagrams/aggregator/"

namespace circuit_aggregator {
	std::ifstream openFile(std::string filename) {
		std::ifstream file;
		file.open(LOG_PATH + filename, std::ios_base::in);
		EXPECT_TRUE(file.is_open());
		return file;
	}
	
	TEST(CircuitAggregator, linear) {
		std::ifstream file = openFile("linear");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state A");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state B");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state C");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, singleCircuit) {
		std::ifstream file = openFile("singleCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, C, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, onlyCircuit) {
		std::ifstream file = openFile("singleCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, C, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, endInCircuit) {
		std::ifstream file = openFile("endInCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, C, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, endOutCircuit) {
		std::ifstream file = openFile("endOutCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, C, ]");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state A");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state B");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state D");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, unstarted) {
		std::ifstream file = openFile("unstartedCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state A");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Going to state B");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, shortLongCircuit) {
		std::ifstream file = openFile("shortLongCircuit");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 1: [A, B, C, ]");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, ]");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 1: [A, B, C, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, shortLongEnd) {
		std::ifstream file = openFile("shortLongEnd");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 1: [A, B, C, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	/*
	 * Known issue
	 * if there is only one circuit but longer with same vertexes is possible, application will find End (handleEnd function)
	 * and because no circuit was found before, it will log only transitions (Going to state A, Going to state B)
	 *
	TEST(CircuitAggregator, shortEnd) {
	    // Shorter from two circuits, containing same 1st and 2nd vertexes
		std::ifstream file = openFile("shortEnd");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	*/
	TEST(CircuitAggregator, doubleShortEnd) {
		// Shorter of circuits happening twice
		std::ifstream file = openFile("doubleShortEnd");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, ]");
		EXPECT_EQ(constAggregated, logs);
	}
	
	TEST(CircuitAggregator, shortEnd2) {
		// End in shorter circuit, but circuits have different 2nd vertex
		std::ifstream file = openFile("shortEnd2");
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(file);
		std::vector<std::string> logs = circuitAggregator.createAggregatedVector(file);
		std::vector<std::string> constAggregated;
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] [sampleApp] [info] [TransitionSmurf] Start of Run -- Aggregated");
		constAggregated.emplace_back("[2021-12-20 10:05:29.392] In circuit 0: [A, B, ]");
		EXPECT_EQ(constAggregated, logs);
	}
}