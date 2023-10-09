#include <state_smurf/log_evaluator/LogsComparer.hpp>

#include <cxxopts.hpp>

#include <fstream>
#include <iostream>
#include <string>



static cxxopts::Options createArgOpts() {
	cxxopts::Options options { "SmurfEvaluator" };
	options.add_options()
				   ("e, etalon", "Path to etalon .log file", cxxopts::value<std::string>())
				   ("c, compare", "Path to .log file which we want to compare with etalon",
					cxxopts::value<std::string>()->default_value(""))
				   ("s, save-aggregated", "If set, aggregated files will be saved. "
										  "Value is path to directory where aggregated files will be created."
										  "In given directory files *etalon* and *compared* will be created",
					cxxopts::value<std::string>())
				   ("h,help", "Print help message");
	return options;
}

cxxopts::ParseResult parseArgOpts(int argc, char **argv) {
	cxxopts::Options options = createArgOpts();
	try {
		auto parsedOptions = options.parse(argc, argv);
		if(parsedOptions.count("help")) {
			std::cout << options.help() << std::endl;
			exit(EXIT_SUCCESS);
		}

		if(!parsedOptions.count("etalon")) {
			std::cerr << "Error: no etalon file provided\n";
			std::cout << options.help() << std::endl;
			exit(EXIT_FAILURE);
		}
		if(!parsedOptions.count("compare") && !parsedOptions.count("save-aggregated")) {
			std::cerr << "Error: no compare file provided\n";
			std::cout << options.help() << std::endl;
			exit(EXIT_FAILURE);
		} else if (!parsedOptions.count("compare") && parsedOptions.count("save-aggregated")) {
			std::cout << "No file to compare was provided. Evaluator will create aggregated file from etalon and exit." << std::endl;
		}

		return parsedOptions;
	} catch(...) {
		std::cerr << "Parsing of arguments failed\n" << options.help();
		exit(EXIT_FAILURE);
	}

}

int main(int argc, char **argv) {
	auto args = parseArgOpts(argc, argv);

	std::string aggregatedPath;
	if(args.count("save-aggregated")) {
		aggregatedPath = args["save-aggregated"].as<std::string>();
	}
	std::string compareFilePath = args["compare"].as<std::string>();
	std::string etalonFilePath = args["etalon"].as<std::string>();

	// Opening files
	std::ifstream etalonFile;
	etalonFile.open(etalonFilePath, std::ios_base::in);
	if(!etalonFile.is_open()) {
		std::cerr << "Unable to open " << etalonFilePath << std::endl;
		return EXIT_FAILURE;
	}
	std::ifstream compareFile;
	if (!compareFilePath.empty()) {
		compareFile.open(compareFilePath);
		if (!compareFile.is_open()) {
			std::cerr << "Unable to open " << compareFilePath << std::endl;
			return EXIT_FAILURE;
		}
	}

	if(!state_smurf::log_evaluator::LogsComparer::compareFiles(etalonFile, compareFile, aggregatedPath)) {
		return EXIT_FAILURE;
	}

	etalonFile.close();
	compareFile.close();
	return EXIT_SUCCESS;

}