#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/CircuitFinder.hpp>
#include <state_smurf/log_evaluator/CircuitCreator.hpp>
#include <state_smurf/StateDiagramDefinition.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>


static cxxopts::Options createArgOpts() {
	cxxopts::Options options {"BringAuto daemon"};
	options.add_options()
			("e,etalon", "Path to Etalon.log file", cxxopts::value<std::string>())
			("compare", "Path to .log file which we want to compare with etalon", cxxopts::value<std::string>())
			("create", "Switch to create circuits of etalon file")
			("t, target", "Target filename, where circuit-log will be written", cxxopts::value<std::string>()->default_value("./circuit.log"))
			("h,help", "Print help message");
	return options;
}

cxxopts::ParseResult parseArgOpts(int argc, char** argv) {
	cxxopts::Options options = createArgOpts();
	try {
		auto parsedOptions = options.parse(argc, argv);
		if (parsedOptions.count("help")) {
			std::cout << options.help() << std::endl;
			exit(EXIT_SUCCESS);
		}

		if (!parsedOptions.count("etalon")) {
			std::cerr << "Error: no etalon file provided\n";
			std::cout << options.help() << std::endl;
			exit(EXIT_FAILURE);
		}
		if (!parsedOptions.count("compare") && !parsedOptions.count("create")) {
			std::cerr << "Error: no compare file provided\n";
			std::cout << options.help() << std::endl;
			exit(EXIT_FAILURE);
		}
		
		return parsedOptions;
	} catch (...) {
		std::cerr << "Parsing of arguments failed\n" << options.help();
		exit(EXIT_FAILURE);
	}
	
}

int main(int argc, char **argv) {
	auto args = parseArgOpts(argc, argv);
	
	bool createEtalonCircuits = args.count("create");
	std::string etalonFilePath = args["etalon"].as<std::string>();
	std::string compareFilePath = {};
	if (!createEtalonCircuits) {
		compareFilePath = args["compare"].as<std::string>();
	}
	std::string targetFileName = args["target"].as<std::string>();
	
	state_smurf::log_evaluator::CircuitFinder CF(state_smurf::createDiagram());
	auto circuits = CF.find();
	// DEBUG print
	for (const auto& cir : circuits) {
		for (const auto& n: cir) {
			std::cout<< n << " ";
		}
		std::cout<< std::endl;
	}
	
	// Opening files
	std::ifstream etalonFile;
	etalonFile.open(etalonFilePath, std::ios_base::in);
	if (!etalonFile.is_open()) {
		std::cerr << "Unable to open " << etalonFilePath << std::endl;
		return EXIT_FAILURE;
	}
	if (createEtalonCircuits) {
		state_smurf::log_evaluator::CircuitCreator circuitCreator(etalonFile, circuits);
		circuitCreator.createCircuit(targetFileName);
		etalonFile.close();
		return EXIT_SUCCESS;
	}
	std::ifstream compareFile;
	compareFile.open(compareFilePath);
	if (!compareFile.is_open()) {
		std::cerr << "Unable to open " << compareFilePath << std::endl;
		return EXIT_FAILURE;
	}

    if (!state_smurf::log_evaluator::LogsComparer::compareFiles(etalonFile, compareFile)) {
        return EXIT_FAILURE;
    }

	etalonFile.close();
    compareFile.close();
	return EXIT_SUCCESS;

}