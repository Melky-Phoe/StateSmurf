#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/helpers/LineParser.hpp>
#include <state_smurf/log_evaluator/CircuitAggregator.hpp>
#include <state_smurf/log_evaluator/helpers/FileHelper.hpp>

#include <iostream>
#include <filesystem>


namespace state_smurf::log_evaluator {

bool LogsComparer::compareFiles(std::ifstream &etalonFile, std::ifstream &comparedFile,
								const std::string& saveAggregatedPath) {
	CircuitAggregator circuitAggregator;
	std::vector<std::string> etalonLogs = loadLogs(etalonFile, circuitAggregator);
	std::vector<std::string> comparedLogs;
	if (comparedFile.is_open()) {
		comparedLogs = loadLogs(comparedFile, circuitAggregator);
	} else if (saveAggregatedPath.empty()) {
		std::cerr << "ERROR: No compare file was provided. Enter compared file or use option --save-aggregated to save aggregated etalon" << std::endl;
		return false;
	}

	if (!saveAggregatedPath.empty()) {
		if (comparedLogs.empty()) { 	// Creating aggregated etalon only
			if (!helpers::FileHelper::createAggregatedFiles(etalonLogs, "etalon", saveAggregatedPath)) {
				return false;
			} else {
				return true;
			}
		} else if (!helpers::FileHelper::createAggregatedFiles(etalonLogs, "etalon", saveAggregatedPath) ||
				   !helpers::FileHelper::createAggregatedFiles(comparedLogs, "compared", saveAggregatedPath)) {
			return false;
		}
	}

	if (!validateEtalon(etalonLogs)) {
		return false;
	}
	if (comparedLogs.empty()) {
		std::cerr << "ERROR: Empty Compare file" << std::endl;
		return false;
	}

	bool filesAreSame = true;
	bool runsAreSame = true;

	int i = 0;
	if (!isStartOfRunLog(comparedLogs[i])) {
		std::cerr << "ERROR: compared Log file isn't starting with Start of Run log" << std::endl;
		return false;
	}
	for (int runCount = 1; i < comparedLogs.size() && isStartOfRunLog(comparedLogs[i]); ++runCount) {
		std::cout << "Run number " << runCount << ":" << std::endl;

		for (int j = 0; j < etalonLogs.size() && i < comparedLogs.size(); ++j) {
			if (!helpers::LineParser::compareLines(etalonLogs[j], comparedLogs[i])) {
				runsAreSame = false;

				// Compared file is shorter than Etalon, we need to finish comparing Etalon file
				if (isStartOfRunLog(comparedLogs[i])) {
					if (isStartOfRunLog(etalonLogs[j])) {
						std::cerr << "ERROR: Start of Run is different, are you comparing same application?"
								  << std::endl;
						return false;
					}
					for (j += 1; j < etalonLogs.size(); ++j) {
						helpers::LineParser::compareLines(etalonLogs[j], "");
					}
					break;
				}
			}
			i++;
		}
		if (comparedLogs.size() > etalonLogs.size()) {
			// Etalon run is shorter than compared, needs catching up
			while (i < comparedLogs.size() && !isStartOfRunLog(comparedLogs[i])) {
				runsAreSame = false;
				helpers::LineParser::compareLines("", comparedLogs[i]);
				i++;
			}
		} else if (comparedLogs.size() < etalonLogs.size()) {
			for (u_long j = comparedLogs.size(); j < etalonLogs.size(); ++j) {
				runsAreSame = false;
				helpers::LineParser::compareLines(etalonLogs[j], "");
			}
		}


		if (runsAreSame) {
			std::cout << "\tOK" << std::endl;
		} else {
			filesAreSame = false;
			// starting next run
			runsAreSame = true;
		}
		std::cout << "----------------------------------------------------" << std::endl;
	}

	return filesAreSame;
}

bool LogsComparer::isStartOfRunLog(const std::string &line) {
	if (line.ends_with("Start of Run -- Aggregated")) {
		return true;
	} else {
		if (line.ends_with("Start of Run")) {
			std::cerr << "ERROR: a file was not aggregated to circuits" << std::endl;
		}
		return false;
	}
}

bool LogsComparer::validateEtalon(std::vector<std::string> etalonLogs) {
	if (!etalonLogs.empty()) {
		if (!isStartOfRunLog(etalonLogs[0])) {
			std::cerr << "ERROR: Etalon doesn't start with \"Start of Run -- Aggregated\" log" << std::endl;
			return false;
		}
		for (int i = 1; i < etalonLogs.size(); ++i) {
			if (isStartOfRunLog(etalonLogs[i])) {
				std::cerr << "ERROR: Etalon file must contain only one Run, given file contains more\n"
							 "Terminating..." << std::endl;
				return false;
			}

			std::vector<std::string> etalonTokens = helpers::LineParser::parseLine(etalonLogs[i]);

			if (etalonTokens[static_cast<int>(helpers::LineParser::LogTokensIndexes::VERBOSITY)] == "[warning]") {
				std::cout << "WARNING: there is unsuccessful transition in etalon." << std::endl;
			}

		}
		return true;
	} else {
		std::cerr << "ERROR: empty etalon" << std::endl;
		return false;
	}
}

std::vector<std::string> LogsComparer::loadLogs(std::ifstream &file, CircuitAggregator &circuitAggregator) {
	std::vector<std::string> aggregatedLogs;
	if (helpers::FileHelper::isFileAggregated(file)) {
		aggregatedLogs = helpers::FileHelper::createVectorFromFile(file);
	} else {
		circuitAggregator.ensureCircuitsInitialized(file);
		aggregatedLogs = circuitAggregator.createAggregatedVector(file);
	}
	return aggregatedLogs;
}


}
