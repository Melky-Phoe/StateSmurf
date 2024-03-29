#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
	/**
	 * Class used for aggregating TransitionSmurf Logs into Circuits
	 * Circuit is set of Vertexes that can appear in loops, based od State Diagram
	 */
	class CircuitAggregator {
	public:
		/**
		 * Constructor, filters non-TransitionSmurf logs out of sourceLogFile and finds circuits_ of state diagram
		 * @param sourceLogFile
		 */
		CircuitAggregator() = default;

		void ensureCircuitsInitialized(std::istream &sourceLogFile);
		
		/**
		 * Aggregates logs into circuits that are saved to file given by param
		 * @param newFileName - filename where aggregated logs will be written. Must be existing path.
		 */
		std::vector<std::string> createAggregatedVector(std::istream &sourceLogFile);
	
	private:
		/**
		 * Finds longest possible circuit in file starting with current vertex
		 * If end of run is found, returns END_FOUND.
		 * If no circuit was found, returns NOT_FOUND.
		 * @return index of circuit
		 */
		long getCircuit(long lastCircuit);
		
		/**
		 * Called if end of file or new run was found
		 * Check if application run ends in 1 circuit. This circuit doesn't have to be finished
		 * @param currentCircuit index of last circuit found
		 * @return true if states are from same circuit or end was found
		 */
		void handleEnd(long currentCircuit, std::vector<std::string> &targetLogFile);
		
		void handleStart(std::vector<std::string> &targetLogFile);
		
		void handleNoCircuits(std::vector<std::string> &targetLogFile);
		
		void printCircuitLog(std::vector<std::string> &targetLogFile, long circuitIndex);
		
		/**
		 * If multiple possible Circuits are found, the longest is taken
		 * @param circuitFoundIndexes indexes of circuits that are possible
		 * @return index of longest circuit
		 */
		long getLongestCircuitIndex(const std::vector<int> &circuitFoundIndexes);
		
		/// All possible circuits
		std::vector<std::vector<std::string>> circuitList_;
		
		/// All TransitionSmurf logs saved in Vector
		std::vector<std::string> transitionLogVector_;
		u_long transitionIndex_{0};
		
		enum returnCodes {
			NOT_FOUND = -1,
			END_FOUND = -2,
			START_FOUND = -3,
			NO_CIRCUITS = -4
		};
	};
	
}


