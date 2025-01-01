#include "phase 1/regexFileReader.cpp"
#include "phase 1/NFAcreation.cpp"
#include "phase 2/firstFollowCalculator.cpp"
#include "phase 2/LeftRecursionElimination.cpp"
#include "phase 2/leftFactoring.cpp"
#include "phase 2/grammarFileParser.cpp"
#include "phase 2/PredictiveParsingTable.cpp"

using namespace std;
namespace fs = filesystem;

int main() {
    //  processLexicalAnalysis("../input/regex_rules.txt", "../input/input.txt", "../output/tokens.txt");
    grammarFileParser gfp = grammarFileParser();
    gfp.readFile("input/grammar.txt");
    LeftRecursionEliminator lre = LeftRecursionEliminator();
    lre.eliminateLeftRecursion("input/grammar.txt");
    leftFactoring lf = leftFactoring(lre.gfp);
    lf.leftFactor();
    lf.gfp->printAll();

    FirstFollowCalculator ffc = FirstFollowCalculator(lf.gfp->grammar, lf.gfp->nonTerminals);
    // First Set
    ffc.calculateFirst();
    unordered_map<string, set<string> > first = ffc.getFirst();
    
    // cout << "First Set:" << endl;
    // for (auto &p: first) {
    //     cout << p.first << " : {";
    //     for (auto &s: p.second) {
    //         cout << s << ", ";
    //     }
    //     cout << "}" << endl;
    // }

    // Follow Set
    ffc.calculateFollow();
    unordered_map<string, set<string> > follow = ffc.getFollow();

    // cout << "Follow Set:" << endl;
    // for (auto &p: follow) {
    //     cout << p.first << " : {";
    //     for (auto &s: p.second) {
    //         cout << s << ", ";
    //     }
    //     cout << "}" << endl;
    // }

    // Predictive Parsing Table
    PredictiveParsingTable ppt = PredictiveParsingTable(lf.gfp->grammar, lf.gfp->nonTerminals, first, follow);
    ppt.constructTable();
    ppt.printTable();

    return 0;
}
