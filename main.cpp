
#include "regexFileReader.cpp"
#include "NFAcreation.cpp"


using namespace std;
namespace fs = filesystem;

int main() {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules("../input/regex_rules.txt");
    regexLoader.printAll();
    nfaCreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions, regexLoader.keywords, regexLoader.punctuations);
    nfaCreator.printAllStates();
    return 0;

}
