#include <bringauto/log_evaluator/LogsComparer.h>

#include <cxxopts.hpp>

#include <iostream>
#include <string>


static cxxopts::Options createArgOpts() {
	cxxopts::Options options {"BringAuto daemon"};
	options.add_options()
			("e,etalon", "Path to Etalon.log file", cxxopts::value<std::string>())
			("c,compare", "Path to .log file which we want to compare with etalon", cxxopts::value<std::string>())
			("h,help", "Print help message");
	return options;
}

static int parseArgOpts(int argc, char** argv,
						std::string* etalonFilePath, std::string* compareFilePath) {
	cxxopts::Options options = createArgOpts();
	try {
		auto parsedOptions = options.parse(argc, argv);
		if (parsedOptions.count("help")) {
			std::cout << options.help() << std::endl;
			return 1;
		}
		if(!parsedOptions.count("etalon")){
			std::cerr << "Error: no etalon file provided\n";
			std::cout << options.help() << std::endl;
			return 1;
		}
		if(!parsedOptions.count("compare")){
			std::cerr << "Error: no compare file provided\n";
			std::cout << options.help() << std::endl;
			return 1;
		}

		etalonFilePath->append(parsedOptions["etalon"].as<std::string>());
		compareFilePath->append(parsedOptions["compare"].as<std::string>());

	} catch (...) {
		std::cerr << "Parsing of arguments failed\n" << options.help();
		return 1;
	}
	return 0;
}

int main(int argc, char **argv) {
	std::string etalonFilePath = {};
	std::string compareFilePath = {};
	if(parseArgOpts(argc, argv, &etalonFilePath, &compareFilePath)) {
		return 1;
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

    bringauto::log_evaluator::LogsComparer::compareFiles(etalonFile, compareFile);

	etalonFile.close();
    compareFile.close();
	return EXIT_SUCCESS;

}