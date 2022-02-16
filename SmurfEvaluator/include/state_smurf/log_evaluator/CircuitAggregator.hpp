#pragma once

#include <fstream>
#include <vector>

namespace state_smurf::log_evaluator {
/**
 * Class used for aggregating Logs of application using TransitionSmurf into Circuits
 */
class CircuitAggregator {
public:
	/**
	 * Constructor, filters non-TransitionSmurf logs out of sourceLogFile and finds circuits of state diagram in StateDiagramDefinition.hpp
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
	 * Finds longest possible circuit in file
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
	 * If we find multiple possible Circuits, we have to take the longest
	 * @param circuitFoundIndexes indexes of circuits that are possible
	 * @return index of longest circuit
	 */
	long getLongestCircuitIndex(const std::vector<int>& circuitFoundIndexes);
	
	std::vector<std::vector<std::string>> _circuitList;
	std::vector<std::string> _transitionLogVector;
	u_long _transitionIndex {0};
};

}


