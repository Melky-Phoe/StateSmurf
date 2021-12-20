#include <LogsComparer.h>

#include <boost/algorithm/string.hpp>
#include <boost/algorithm/string/trim.hpp>

#include <iostream>

namespace bringauto {


int LogsComparer::compareFiles(std::istream &etalon, std::istream &compared) {
    std::string etalonLog = Filter::findNextTransitionLog(etalon);
    std::string comparedLog = Filter::findNextTransitionLog(compared);

    for (int i = 0; !etalonLog.empty(); ++i) {
        compareLines(etalonLog, comparedLog);

        etalonLog = Filter::findNextTransitionLog(etalon);
        comparedLog = Filter::findNextTransitionLog(compared);
    }
    // mozna dvojita podminka ve foru bude lepsi
    if (!comparedLog.empty()) {
        std::cerr << "comparedLog is longer" << std::endl;
    }
    return 0;
}

std::vector<std::string> LogsComparer::parseLine( const std::string& line) {
    std::vector<std::string> tokens;
    boost::split(tokens, line, boost::is_any_of(" "));
    return tokens;
}

int LogsComparer::compareLines(const std::string& etalon, const std::string& compared) {
    std::vector<std::string> etalonTokens = parseLine(etalon);
    std::vector<std::string> comparedTokens = parseLine(compared);

    if (etalonTokens[static_cast<int>(LogTokensIndexes::verbosity)] == "[warning]") {
        std::cout << "WARNING: there is unsuccessful transition in etalon" << std::endl;
    }

    for (int i = static_cast<int>(LogTokensIndexes::stateTransition); i < etalonTokens.size(); ++i) {
        if (etalonTokens[i] != comparedTokens[i]) {
            std::cout << "Logs aren't equal:\n"
                         "Etalon: " << etalon << std::endl <<
                         "Compared: " << compared << std::endl;
            break;

        }
    }
    return 0;
}


}
