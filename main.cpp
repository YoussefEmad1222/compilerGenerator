#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 1/DFACreator.h"
#include "phase 1/lexicalAnalyzer.cpp"
#include "phase 1/DFA_minimizer.hpp"
#include "phase 2/LeftRecursionElimination.cpp"
using namespace std;
namespace fs = filesystem;

int main() {
    //  processLexicalAnalysis("../input/regex_rules.txt", "../input/input.txt", "../output/tokens.txt");
    grammarFileParser gfp = grammarFileParser();
    gfp.readFile("../input/grammar.txt");
    LeftRecursionEliminator lre = LeftRecursionEliminator();
    lre.eliminateLeftRecursion("../input/grammar.txt");

    //
    return 0;
}
