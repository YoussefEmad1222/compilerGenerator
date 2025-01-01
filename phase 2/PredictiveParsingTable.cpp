#include "firstFollowCalculator.h"

class PredictiveParsingTable {
private:
    unordered_map<string, vector<string>> grammar;
    vector<string> nonTerminals;
    unordered_map<string, set<string>> first;
    unordered_map<string, set<string>> follow;
    const string EPSILON = "#";
    const string END_SYMBOL = "$";
    unordered_map<string, unordered_map<string, string>> parsingTable;

    bool isLL1 = true;

    void constructLL1Table(const unordered_map<string, vector<string>>& grammar,
                            const unordered_map<string, set<string>>& first,
                            const unordered_map<string, set<string>>& follow) {
            // Iterate over each non-terminal in the grammar
            for (const auto& entry : grammar) {
                string nonTerminal = entry.first;
                
                // Iterate over each production rule of the non-terminal
                for (const string& production : entry.second) {
                    // Find the first set of the production
                    set<string> firstOfProduction;
                    bool epsilonInFirst = false;

                    // Check if the production starts with a terminal or ε (epsilon)
                    if (production == "ε") {
                        epsilonInFirst = true;
                    } else {
                        // Extract the first set of the production (handle terminals and non-terminals)
                        for (char symbol : production) {
                            if (first.count(string(1, symbol)) > 0) {
                                firstOfProduction = first.at(string(1, symbol));
                                break;
                            } else {
                                firstOfProduction.insert(string(1, symbol));
                            }
                        }
                    }

                    // Add the corresponding production to the table based on First set
                    for (const string& terminal : firstOfProduction) {
                        if (terminal != "ε") {
                            parsingTable[nonTerminal][terminal] = production;
                        }
                    }

                    // Handle ε (empty string) in First set
                    if (epsilonInFirst) {
                        for (const string& terminal : follow.at(nonTerminal)) {
                            parsingTable[nonTerminal][terminal] = production;
                        }
                    }
                }
            }
        }

        void printParsingTable() {
            for (const auto& entry : parsingTable) {
                string nonTerminal = entry.first;
                cout << "Non-terminal: " << nonTerminal << endl;
                
                for (const auto& terminalEntry : entry.second) {
                    cout << "  Terminal: " << terminalEntry.first << " -> " << terminalEntry.second << endl;
                }
            }
        }

public:
    PredictiveParsingTable(unordered_map<string, vector<string>> grammar, 
                           vector<string> nonTerminals, 
                           unordered_map<string, set<string>> first, 
                           unordered_map<string, set<string>> follow)
        : grammar(grammar), nonTerminals(nonTerminals), first(first), follow(follow) {}

    void constructTable() {
        constructLL1Table(grammar, first, follow);
    }

    void printTable() {
        printParsingTable();
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