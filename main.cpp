#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 2/leftRecursionElimination.h"
#include "phase 2/leftFactoring.h"
#include "phase 2/grammarFileParser.h"
#include "phase 2/PredictiveParsingTable.h"
#include "phase 2/FirstFollowCalculator.h"
#include "phase 2/parser.h"

using namespace std;
namespace fs = filesystem;

void printTokens(const vector<pair<string, string>> &tokens) {
    cout << "Tokens:" << endl;
    for (const auto &token : tokens) {
        cout << "Lexeme: " << token.first << ", Type: " << token.second << endl;
    }
}

int main() {
    //  processLexicalAnalysis("../input/regex_rules.txt", "../input/input.txt", "../output/tokens.txt");
    LeftRecursionEliminator lre = LeftRecursionEliminator();
    lre.eliminateLeftRecursion("../input/grammar.txt");
    leftFactoring lf = leftFactoring(lre.gfp);
    lf.leftFactor();
    lf.gfp->printAll();

    FirstFollowCalculator ffc = FirstFollowCalculator(lf.gfp->grammar, lf.gfp->nonTerminals);
    // First Set
    ffc.calculateFirst();
    unordered_map<string, set<string> > first = ffc.getFirst();
    

    // Follow Set
    ffc.calculateFollow();
    unordered_map<string, set<string> > follow = ffc.getFollow();

    
    PredictiveParsingTable ppt = PredictiveParsingTable(first, follow, lf.gfp->grammar, lf.gfp->nonTerminals);
    ppt.computeTable();
    ppt.printTable();

    // Read tokens from the file
    string tokenFile = "../output/tokens.txt";
    vector<pair<string, string>> tokens = readTokens(tokenFile);

    // Add an end-of-input token
    tokens.emplace_back("$", "$");
    printTokens(tokens);

    // Parser
    Parser parser = Parser(ppt, lf.gfp->nonTerminals);
    parser.parse(tokens);

    return 0;
}
