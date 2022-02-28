#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>
#include <state_smurf/log_evaluator/CircuitAggregator.hpp>

#include <iostream>
#include <filesystem>

namespace state_smurf::log_evaluator {
	
	bool LogsComparer::compareFiles(std::istream &etalonFile, std::istream &comparedFile, std::string saveAggregatedPath) {
		state_smurf::log_evaluator::CircuitAggregator circuitAggregator(etalonFile);
		std::vector<std::string> etalonLogs = circuitAggregator.createAggregatedFile(etalonFile);
		std::vector<std::string> comparedLogs = circuitAggregator.createAggregatedFile(comparedFile);
		
		if (!saveAggregatedPath.empty()) {
			if (!saveAggregatedFiles(etalonLogs, comparedLogs, saveAggregatedPath)) {
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
			std::cout << "ERROR: compared Log file isn't starting with Start of Run log" << std::endl;
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
							std::cout << "ERROR: Start of Run is different, are you comparing same application?"
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
			std::cout << "ERROR: empty etalon" << std::endl;
			return false;
		}
	}
	
	bool LogsComparer::saveAggregatedFiles(const std::vector<std::string>& etalonLogs, const std::vector<std::string>& comparedLogs, const std::string& path) {
		std::filesystem::path dir(path);
		std::filesystem::create_directories(path);
		std::filesystem::path etalonPath = dir / "etalon";
		std::filesystem::path comparedPath = dir / "compared";
		std::ofstream newEtalonFile(etalonPath.string());
		std::ofstream newComparedFile(comparedPath.string());
		if (!newComparedFile.is_open() || !newEtalonFile.is_open()) {
			std::cerr << "ERROR: unable to create files in directory " << path << std::endl;
			return false;
		}
		for (const auto& line : etalonLogs) {
			newEtalonFile << line << std::endl;
		}
		for (const auto& line : comparedLogs) {
			newComparedFile << line << std::endl;
		}
		std::cout << "Aggregated files are saved in directory " << absolute(dir) << std::endl;
		return true;
	}
	
}
