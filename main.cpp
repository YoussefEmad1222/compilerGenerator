
#include "regexFileReader.cpp"
#include "NFAcreation.cpp"
#include "DFACreator.cpp"
#include "LexicalAnalyzer.cpp"
#include "DFA_minimizer.hpp"

using namespace std;
namespace fs = filesystem;

int main() {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules("input/regex_rules.txt");
    regexLoader.printAll();
    NFACreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions, regexLoader.keywords, regexLoader.punctuations);
    nfaCreator.writeAllStatesToFile("output/nfa.txt");
    const NFA nfa = nfaCreator.combineNFAs();
    DFACreator dfaCreator(nfa);
    dfaCreator.createDFA();
    dfaCreator.writeAllStatesToFile("output/dfa.txt");

    DFA minimized = minimizeDFA(dfaCreator.getDFA());
    dfaCreator.writeAllStatesToFile(minimized, "output/MinDfa.txt");
    
    LexicalAnalyzer analyzer(minimized);
    cout << endl;
    analyzer.Analyze("input/input.txt");
    return 0;
}
