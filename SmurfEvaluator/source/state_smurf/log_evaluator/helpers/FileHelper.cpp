#include <state_smurf/log_evaluator/helpers/FileHelper.hpp>
#include <state_smurf/log_evaluator/helpers/Filter.hpp>

#include <filesystem>
#include <fstream>
#include <iostream>



namespace state_smurf::log_evaluator::helpers {

bool FileHelper::createAggregatedFiles(const std::vector<std::string> &aggregatedLogs, const std::string &fileName,
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

void FileHelper::writeVectorToFile(const std::vector<std::string> &sourceLogs, std::ofstream *outputStream) {
	for (const auto &line: sourceLogs) {
		*outputStream << line << std::endl;
	}
}

bool FileHelper::isFileAggregated(std::istream &file) {
	auto line = helpers::Filter::findNextTransitionLog(file);
	bool retVal = false;
	if (line.ends_with("Start of Run -- Aggregated")) {
		retVal = true;
	}
	file.clear();
	file.seekg(std::ios::beg);
	return retVal;
}

std::vector<std::string> FileHelper::createVectorFromFile(std::istream &file) {
	std::vector<std::string> lines;
	std::string line;

	while (std::getline(file, line)) {
		lines.push_back(line);
	}
	return lines;
}
}
