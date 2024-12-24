#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 1/DFACreator.h"
#include "phase 1/lexicalAnalyzer.cpp"
#include "phase 1/DFA_minimizer.hpp"
#include "phase 2/firstFollowCalculator.h"
#include "phase 2/LeftRecursionElimination.cpp"
#include "phase 2/firstFollowComputation.h"
#include "phase 2/leftFactoring.cpp"
using namespace std;
namespace fs = filesystem;

int main() {
    //  processLexicalAnalysis("../input/regex_rules.txt", "../input/input.txt", "../output/tokens.txt");
    grammarFileParser gfp = grammarFileParser();
    gfp.readFile("../input/grammar.txt");
    LeftRecursionEliminator lre = LeftRecursionEliminator();
    lre.eliminateLeftRecursion("../input/grammar.txt");
    leftFactoring lf = leftFactoring(&gfp);


    FirstFollowCalculator ffc = FirstFollowCalculator(lre.getGrammarFileParser().grammar,
                                                        lre.getGrammarFileParser().nonTerminals);
    // First Set
    ffc.calculateFirst();
    unordered_map<string, set<string>> first = ffc.getFirst();

    // Follow Set
    ffc.calculateFollow();
    unordered_map<string, set<string>> follow = ffc.getFollow();
    
    return 0;
}
