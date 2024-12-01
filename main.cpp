
#include "regexFileReader.cpp"
#include "NFAcreation.cpp"
#include "DFACreator.h"

using namespace std;
namespace fs = filesystem;

int main() {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules("../input/regex_rules.txt");
    regexLoader.printAll();
    nfaCreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions, regexLoader.keywords, regexLoader.punctuations);
    nfaCreator.writeAllStatesToFile("../nfa.txt");
    const NFA nfa = nfaCreator.combineNFAs();
    DFACreator dfaCreator(nfa);
    dfaCreator.createDFA();
    dfaCreator.writeAllStatesToFile("../dfa.txt");
    return 0;

}
