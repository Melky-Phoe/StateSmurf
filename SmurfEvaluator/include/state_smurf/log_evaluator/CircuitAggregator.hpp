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
	 * Constructor, filters non-TransitionSmurf logs out of sourceLogFile and finds circuits of state diagram
	 * @param sourceLogFile
	 */
	explicit CircuitAggregator(std::istream& sourceLogFile);
	
	/**
	 * Aggregates logs into circuits that are saved to file given by param
	 * @param newFileName - filename where aggregated logs will be written. Must be existing path.
	 */
	void createAggregatedFile(const std::string& newFileName);

private:
	/**
	 * Finds longest possible circuit in file starting with current vertex
	 * @return index of circuit
	 */
	long getCircuit();
	
	/**
	 * Called if end of file or new run was found
	 * application runs should end in 1 circuit. Checks if all states are in the circuit
	 * @param currentCircuit index of last circuit found
	 * @return true if states are from same circuit
	 */
	bool handleEnd(long currentCircuit);
	
	/**
	 * If multiple possible Circuits are found, the longest is taken
	 * @param circuitFoundIndexes indexes of circuits that are possible
	 * @return index of longest circuit
	 */
	long getLongestCircuitIndex(const std::vector<int>& circuitFoundIndexes);
	
	std::vector<std::vector<std::string>> _circuitList;
	std::vector<std::string> _transitionLogVector;
	u_long _transitionIndex {0};
};

}


