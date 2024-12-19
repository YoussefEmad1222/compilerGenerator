
#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 1/DFACreator.h"
#include "phase 1/lexicalAnalyzer.cpp"
#include "phase 1/DFA_minimizer.hpp"

using namespace std;
namespace fs = filesystem;

int main() {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules("../input/regex_rules.txt");
    regexLoader.printAll();
    NFACreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions, regexLoader.keywords, regexLoader.punctuations);
    const NFA nfa = nfaCreator.combineNFAs();
    // Construct DFA from NFA
    DFACreator dfaCreator(nfa);
    dfaCreator.createDFA();
    DFA minimized = minimizeDFA(dfaCreator.getDFA());
    LexicalAnalyzer analyzer(minimized);
    cout << endl;
    analyzer.Analyze("../input/input.txt", "../output/tokens.txt");
    //parser code

    return 0;
}
