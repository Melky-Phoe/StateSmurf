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
			("s, save-aggregated", "If set, aggregated files will be saved. "
									   "Value is path to directory where aggregated files will be created.", cxxopts::value<std::string>()->default_value(""))
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
		
		return parsedOptions;
	} catch (...) {
		std::cerr << "Parsing of arguments failed\n" << options.help();
		exit(EXIT_FAILURE);
	}
	
}

int main(int argc, char **argv) {
	auto args = parseArgOpts(argc, argv);

	std::string compareFilePath = args["compare"].as<std::string>();
	std::string etalonFilePath = args["etalon"].as<std::string>();

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
	
	if (!state_smurf::log_evaluator::LogsComparer::compareFiles(etalonFile, compareFile, args["save-aggregated"].as<std::string>())) {
		return EXIT_FAILURE;
	}
	
	etalonFile.close();
	compareFile.close();
	return EXIT_SUCCESS;
	
}