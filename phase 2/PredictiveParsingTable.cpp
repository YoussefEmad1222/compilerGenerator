#include "firstFollowCalculator.h"

class PredictiveParsingTable {
private:
    unordered_map<string, vector<string>> grammar;
    vector<string> nonTerminals;
    unordered_map<string, set<string>> first;
    unordered_map<string, set<string>> follow;
    unordered_map<string, unordered_map<string, string>> parsingTable;
    const string EPSILON = "#";
    const string END_SYMBOL = "$";

    bool isLL1 = true;

public:
    PredictiveParsingTable(unordered_map<string, vector<string>> grammar, 
                           vector<string> nonTerminals, 
                           unordered_map<string, set<string>> first, 
                           unordered_map<string, set<string>> follow)
        : grammar(grammar), nonTerminals(nonTerminals), first(first), follow(follow) {}

    void constructTable() {
        for (string nonTerminal : nonTerminals) {
            for (string production : grammar[nonTerminal]) {
                vector<string> tokens = FirstFollowCalculator::splitByDelimiter(production, ' ');
                set<string> firstSet;

                // Compute FIRST(production)
                bool prevHasEpsilon = true;
                for (string token : tokens) {
                    if (prevHasEpsilon) {
                        unionSet(firstSet, first[token]);
                        prevHasEpsilon = hasEpsilon(token);
                    } else {
                        break;
                    }
                }

                // Add FOLLOW(nonTerminal) if EPSILON is in FIRST(production)
                if (prevHasEpsilon && firstSet.find(EPSILON) != firstSet.end()) {
                    unionSet(firstSet, follow[nonTerminal]);
                }

                for (string terminal : firstSet) {
                    if (terminal == EPSILON) continue;
                    if (parsingTable[nonTerminal][terminal].empty()) {
                        parsingTable[nonTerminal][terminal] = production;
                    } else {
                        // LL(1) conflict detected
                        isLL1 = false;
                        cerr << "Grammar is not LL(1). Conflict at Table[" << nonTerminal
                             << "][" << terminal << "] between \""
                             << parsingTable[nonTerminal][terminal] << "\" and \"" << production << "\".\n";
                    }
                }

                if (firstSet.find(EPSILON) != firstSet.end()) {
                    for (string terminal : follow[nonTerminal]) {
                        if (parsingTable[nonTerminal][terminal].empty()) {
                            parsingTable[nonTerminal][terminal] = EPSILON;
                        } else {
                            // LL(1) conflict detected
                            isLL1 = false;
                            cerr << "Grammar is not LL(1). Conflict at Table[" << nonTerminal
                                 << "][" << terminal << "] between \""
                                 << parsingTable[nonTerminal][terminal] << "\" and \"" << EPSILON << "\".\n";
                        }
                    }
                }
            }
        }
    }

    void printTable() {
        if (!isLL1) {
            cout << "The grammar is not LL(1), so the predictive parsing table cannot be reliably used.\n";
            return;
        }

        cout << "Predictive Parsing Table:\n";
        for (auto &row : parsingTable) {
            string nonTerminal = row.first;
            cout << nonTerminal << ":\n";
            for (auto &col : row.second) {
                cout << "  " << col.first << " -> " << col.second << "\n";
            }
        }
    }

    unordered_map<string, unordered_map<string, string>> getParsingTable() {
        return parsingTable;
    }

    bool isGrammarLL1() {
        return isLL1;
    }

private:
    void unionSet(set<string> &target, const set<string> &source) {
        for (string item : source) {
            target.insert(item);
        }
    }

    bool hasEpsilon(string symbol) {
        return first[symbol].find(EPSILON) != first[symbol].end();
    }
};
