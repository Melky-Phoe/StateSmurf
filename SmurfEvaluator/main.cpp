#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/CircuitAggregator.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>


static cxxopts::Options createArgOpts() {
	cxxopts::Options options{"BringAuto daemon"};
	options.add_options()
			("e, etalon", "Path to EtalonAggregated file", cxxopts::value<std::string>())
			("c, compare", "Path to .log file which we want to compare with etalon", cxxopts::value<std::string>())
			("a, aggregate", "Path to .log file to create AggregatedFile from", cxxopts::value<std::string>())
			("t, target", "Target AggregatedFile name",
			 cxxopts::value<std::string>()->default_value("./aggregatedFile"))
			("h,help", "Print help message");
	return options;
}

cxxopts::ParseResult parseArgOpts(int argc, char **argv) {
	cxxopts::Options options = createArgOpts();
	try {
		auto parsedOptions = options.parse(argc, argv);
		if (parsedOptions.count("help")) {
			std::cout << options.help() << std::endl;
			exit(EXIT_SUCCESS);
		}
		
		if (!parsedOptions.count("aggregate")) {
			if (!parsedOptions.count("etalon")) {
				std::cerr << "Error: no etalon file provided\n";
				std::cout << options.help() << std::endl;
				exit(EXIT_FAILURE);
			}
			if (!parsedOptions.count("compare")) {
				std::cerr << "Error: no compare file provided\n";
				std::cout << options.help() << std::endl;
				exit(EXIT_FAILURE);
			}
		}
		
		return parsedOptions;
	} catch (...) {
		std::cerr << "Parsing of arguments failed\n" << options.help();
		exit(EXIT_FAILURE);
	}
	
}

int main(int argc, char **argv) {
	auto args = parseArgOpts(argc, argv);
	
	bool createEtalonCircuits = args.count("aggregate");
	std::string etalonFilePath = {};
	std::string compareFilePath = {};
	if (!createEtalonCircuits) {
		compareFilePath = args["compare"].as<std::string>();
		etalonFilePath = args["etalon"].as<std::string>();
	}
	std::string targetFileName = args["target"].as<std::string>();
	
	if (createEtalonCircuits) {
		std::ifstream sourceFile;
		std::string sourceFileName = args["aggregate"].as<std::string>();
		sourceFile.open(sourceFileName, std::ios_base::in);
		if (!sourceFile.is_open()) {
			std::cerr << "Unable to open " << sourceFileName << std::endl;
			return EXIT_FAILURE;
		}
		state_smurf::log_evaluator::CircuitAggregator circuitCreator(sourceFile);
		circuitCreator.createAggregatedFile(targetFileName);
		std::cout << "Created file containing aggregated circuits: " << targetFileName << std::endl;
		sourceFile.close();
		return EXIT_SUCCESS;
	}
	// Opening files
	std::ifstream etalonFile;
	etalonFile.open(etalonFilePath, std::ios_base::in);
	if (!etalonFile.is_open()) {
		std::cerr << "Unable to open " << etalonFilePath << std::endl;
		return EXIT_FAILURE;
	}
	std::ifstream compareFile;
	compareFile.open(compareFilePath);
	if (!compareFile.is_open()) {
		std::cerr << "Unable to open " << compareFilePath << std::endl;
		return EXIT_FAILURE;
	}
	state_smurf::log_evaluator::CircuitAggregator circuitAggregator(compareFile);
	circuitAggregator.createAggregatedFile(targetFileName);
	compareFile.close();
	
	std::ifstream aggregatedCompareFile(targetFileName);
	if (!aggregatedCompareFile.is_open()) {
		std::cerr << "Unable to open created file " << targetFileName << std::endl;
		return EXIT_FAILURE;
	}
	
	if (!state_smurf::log_evaluator::LogsComparer::compareFiles(etalonFile, aggregatedCompareFile)) {
		return EXIT_FAILURE;
	}
	
	etalonFile.close();
	compareFile.close();
	return EXIT_SUCCESS;
	
}