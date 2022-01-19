#include <bringauto/log_evaluator/LogsComparer.hpp>
#include <bringauto/log_evaluator/Filter.hpp>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <iostream>

namespace bringauto::log_evaluator {


bool LogsComparer::compareFiles(std::istream &etalon, std::istream &compared) {
    std::string etalonLog = Filter::findNextTransitionLog(etalon);
    std::string comparedLog = Filter::findNextTransitionLog(compared);
    bool filesAreSame = true;

    while (!etalonLog.empty()) {
        if (!compareLines(etalonLog, comparedLog)) {
            filesAreSame = false;
        }
        etalonLog = Filter::findNextTransitionLog(etalon);
        comparedLog = Filter::findNextTransitionLog(compared);

    }

    if (!comparedLog.empty()) {
        std::cerr << "compared log is longer than etalon" << std::endl;
        return false;
    }
    return filesAreSame;
}

std::vector<std::string> LogsComparer::parseLine( const std::string& line) {
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" "));
    return tokens;
}

bool LogsComparer::compareLines(const std::string& etalon, const std::string& compared) {
    std::vector<std::string> etalonTokens = parseLine(etalon);
    std::vector<std::string> comparedTokens = parseLine(compared);

    if (etalonTokens[static_cast<int>(LogTokensIndexes::verbosity)] == "[warning]") {
        std::cout << "WARNING: there is unsuccessful transition in etalon" << std::endl;
    }

    for (int i = static_cast<int>(LogTokensIndexes::appName); i < etalonTokens.size(); ++i) {
        if (etalonTokens[i] != comparedTokens[i]) {
            std::cout << "Logs aren't equal:\n"
                         "  Etalon: " << etalon << std::endl <<
                         "  Compared: " << compared << std::endl;
            return false;

        }
    }
    return true;
}


}
