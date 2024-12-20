#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 1/DFACreator.h"
#include "phase 1/lexicalAnalyzer.cpp"
#include "phase 1/DFA_minimizer.hpp"
#include "phase 2/LeftRecursionElimination.cpp"
using namespace std;
namespace fs = filesystem;

void processLexicalAnalysis(const string &regexRulesFile, const string &inputFile, const string &outputFile) {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules(regexRulesFile);
    regexLoader.printAll();

    NFACreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions, regexLoader.keywords,
                               regexLoader.punctuations);
    const NFA nfa = nfaCreator.combineNFAs();

    // Construct DFA from NFA
    DFACreator dfaCreator(nfa);
    dfaCreator.createDFA();
    DFA minimized = minimizeDFA(dfaCreator.getDFA());

    LexicalAnalyzer analyzer(minimized);
    cout << endl;
    analyzer.Analyze(inputFile, outputFile.c_str());
    //parser code
}

int main() {
  //  processLexicalAnalysis("../input/regex_rules.txt", "../input/input.txt", "../output/tokens.txt");
    LeftRecursionEliminator lre = LeftRecursionEliminator();
    lre.eliminateLeftRecursion("../input/grammar.txt");
    return 0;
}
