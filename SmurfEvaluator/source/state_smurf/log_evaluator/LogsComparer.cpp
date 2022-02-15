#include <state_smurf/log_evaluator/LogsComparer.hpp>
#include <state_smurf/log_evaluator/Filter.hpp>
#include <state_smurf/log_evaluator/LineParser.hpp>

#include <iostream>

namespace state_smurf::log_evaluator {

bool LogsComparer::compareFiles(std::istream &etalon, std::istream &compared) {
    std::vector<std::string> etalonLogs = Filter::createTransitionLogVector(etalon);
    if (!validateEtalon(etalonLogs)) {
        return false;
    }
    std::string comparedLog = Filter::findNextTransitionLog(compared);
    bool filesAreSame = true;
    bool runsAreSame = true;

    if (!isStartOfRunLog(comparedLog)) {
        std::cout << "ERROR: compared Log file isn't starting with Start of Run log" << std::endl;
        return false;
    }
    // tady si vytvorim prechody smycek, asi to budu chtit ulozit >> kdyz najdu nesrovnalost, budu se asi chtit podivat kde byla

    for (int runCount = 1; isStartOfRunLog(comparedLog); ++runCount) {
        std::cout << "Run number " << runCount << ":" << std::endl;

        for (int i = 0; i < etalonLogs.size(); ++i) {
            // tady bude zmena, budu porovnavat smycky, jinak by vsechno melo byt stejny
            if (!LineParser::compareLines(etalonLogs[i], comparedLog)) {
                runsAreSame = false;

                // Compared file is shorter than Etalon, we need to finish comapring Etalon file
                if (isStartOfRunLog(comparedLog)) {
                    if (isStartOfRunLog(etalonLogs[i])) {
                        std::cout << "ERROR: Start of Run is different, are you comparing same application?" << std::endl;
                        return false;
                    }
                    for (i += 1; i < etalonLogs.size(); ++i) {
	                    LineParser::compareLines(etalonLogs[i], "");
                    }
                    break;
                }
            }

            comparedLog = Filter::findNextTransitionLog(compared);
        }
        // Etalon run is shorter than compared, needs catching up
        while (!isStartOfRunLog(comparedLog) && !comparedLog.empty()) {
            runsAreSame = false;
	        LineParser::compareLines("", comparedLog);
            comparedLog = Filter::findNextTransitionLog(compared);
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
    if (line.ends_with("Start of Run")) {
        return true;
    } else {
        return false;
    }
}

bool LogsComparer::validateEtalon(std::vector<std::string> etalonLogs) {
    if (!etalonLogs.empty()) {
        if (!isStartOfRunLog(etalonLogs[0])) {
            std::cout << "WARNING: Etalon doesn't contain \"Start of Run\" log" << std::endl;
            return false;
        }
        for (int i = 1; i < etalonLogs.size(); ++i) {
            if (isStartOfRunLog(etalonLogs[i])) {
                std::cerr << "ERROR: Etalon file must contain only one Run, given file contains more\n"
                             "Terminating..." << std::endl;
                return false;
            }

            std::vector<std::string> etalonTokens = LineParser::parseLine(etalonLogs[i]);

            if (etalonTokens[static_cast<int>(LineParser::LogTokensIndexes::verbosity)] == "[warning]") {
                std::cout << "WARNING: there is unsuccessful transition in etalon." << std::endl;
            }

        }
        return true;
    } else {
        std::cout << "WARNING: empty etalon" << std::endl;
        return false;
    }
}

}
