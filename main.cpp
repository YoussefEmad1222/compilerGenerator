
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
    const NFA nfa = nfaCreator.combineNFAs();
    DFACreator dfaCreator(nfa);
    dfaCreator.createDFA();
    DFA minimized = minimizeDFA(dfaCreator.getDFA());
    LexicalAnalyzer analyzer(minimized);
    cout << endl;
    analyzer.Analyze("input/input.txt", "output/tokens.txt");
    return 0;
}
