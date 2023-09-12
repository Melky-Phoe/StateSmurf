#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>
#include <state_smurf/log_evaluator/CircuitAggregator.hpp>

#include <iostream>
#include <filesystem>



namespace state_smurf::log_evaluator {

bool LogsComparer::compareFiles(std::istream &etalonFile, std::istream &comparedFile,
								const std::string& saveAggregatedPath) {
	state_smurf::log_evaluator::CircuitAggregator circuitAggregator(etalonFile);
	std::vector<std::string> etalonLogs = circuitAggregator.createAggregatedVector(etalonFile);
	std::vector<std::string> comparedLogs;
	if (comparedFile.good()) {
		comparedLogs = circuitAggregator.createAggregatedVector(comparedFile);
	} else if (saveAggregatedPath.empty()) {
		std::cerr << "ERROR: No compare file was provided" << std::endl;
		return false;
	}

	if (!saveAggregatedPath.empty()) {
		if (comparedLogs.empty()) { 	// Creating aggregated etalon only
			if (!createAggregatedFiles(etalonLogs, "etalon", saveAggregatedPath)) {
				return false;
			} else {
				return true;
			}
		} else if (!createAggregatedFiles(etalonLogs, "etalon", saveAggregatedPath) ||
				   !createAggregatedFiles(comparedLogs, "compared", saveAggregatedPath)) {
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
			if (!LineParser::compareLines(etalonLogs[j], comparedLogs[i])) {
				runsAreSame = false;

				// Compared file is shorter than Etalon, we need to finish comparing Etalon file
				if (isStartOfRunLog(comparedLogs[i])) {
					if (isStartOfRunLog(etalonLogs[j])) {
						std::cerr << "ERROR: Start of Run is different, are you comparing same application?"
								  << std::endl;
						return false;
					}
					for (j += 1; j < etalonLogs.size(); ++j) {
						LineParser::compareLines(etalonLogs[j], "");
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
				LineParser::compareLines("", comparedLogs[i]);
				i++;
			}
		} else if (comparedLogs.size() < etalonLogs.size()) {
			for (u_long j = comparedLogs.size(); j < etalonLogs.size(); ++j) {
				runsAreSame = false;
				LineParser::compareLines(etalonLogs[j], "");
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
			std::cerr << "ERROR: file was not aggregated to circuits, start application with --aggregate option\n"
						 "Created file then use as etalon. For more info read README" << std::endl;
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

			std::vector<std::string> etalonTokens = LineParser::parseLine(etalonLogs[i]);

			if (etalonTokens[static_cast<int>(LineParser::LogTokensIndexes::VERBOSITY)] == "[warning]") {
				std::cout << "WARNING: there is unsuccessful transition in etalon." << std::endl;
			}

		}
		return true;
	} else {
		std::cerr << "ERROR: empty etalon" << std::endl;
		return false;
	}
}

bool LogsComparer::createAggregatedFiles(const std::vector<std::string> &aggregatedLogs, const std::string &fileName,
										 const std::string &path) {
	std::filesystem::path dir(path);
	std::filesystem::create_directories(path);
	std::filesystem::path filePath = dir/fileName;
	std::ofstream newFileStream(filePath.string());
	if (!newFileStream.is_open()) {
		std::cerr << "ERROR: unable to create file: `" << fileName << "` in directory: `" << path << "`" << std::endl;
		return false;
	}
	writeVectorToFile(aggregatedLogs, &newFileStream);

	std::cout << "Aggregated file is saved in " << absolute(filePath) << std::endl;
	return true;
}

bool LogsComparer::writeVectorToFile(const std::vector<std::string> &sourceLogs, std::ofstream *outputStream) {
	for (const auto &line: sourceLogs) {
		*outputStream << line << std::endl;
	}
}


}
