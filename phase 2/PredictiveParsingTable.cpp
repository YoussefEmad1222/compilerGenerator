#ifndef PREDICTIVEPARSINGTABLE_H
#define PREDICTIVEPARSINGTABLE_H

// Definition of constants
#define EPSILON "#"
#define END "$"
#define ERR "ERR"

using namespace std;
// Define constants
class PredictiveParsingTable {
private:
    unordered_map<string, set<string>> firstSets;
    unordered_map<string, set<string>> followSets;
    unordered_map<string, vector<string>> grammar;
    vector<string> nonTerminals;
    bool tableComputed;

public:
    unordered_map<string, unordered_map<string, string>> parsingTable;  // Updated ParsingTable type
    // Constructor modified to accept precomputed first and follow sets
    PredictiveParsingTable(
        unordered_map<string, set<string>> firstSets,
        unordered_map<string, set<string>> followSets,
        unordered_map<string, vector<string>> grammar,
        vector<string> nonTerminals
    ) : firstSets(firstSets), followSets(followSets), grammar(grammar),
        nonTerminals(nonTerminals), tableComputed(false) {}

    void computeTable() {
        // Ensure that the first and follow sets are available
        if (firstSets.empty()) throw invalid_argument("First sets are empty");
        if (followSets.empty()) throw invalid_argument("Follow sets are empty");

        try {
            // For each production rule A -> alpha
            for (const auto &symbolAndProductions : grammar) {
                for (const auto &production : symbolAndProductions.second) {
                    updateParsingTable(symbolAndProductions.first, FirstFollowCalculator::splitByDelimiter(production, ' '));
                }
            }
        } catch (const std::invalid_argument& e) {
            // If a production rule already exists, then the grammar is not LL(1)
            cout << "The grammar is not LL(1)." << endl;
            throw e;
        }

        // Add sync symbols to the table at [A, a] that are empty for each terminal a in FOLLOW(A)
        for (const auto &A : nonTerminals) {
            for (const auto &a : followSets.at(A)) {
                if (parsingTable[A].find(a) == parsingTable[A].end()) {
                    parsingTable[A][a] = ERR;
                }
            }
        }
    }

    void updateParsingTable(const string &A, const vector<string> &alpha) {
        // For each non-epsilon terminal a in FIRST(alpha), add alpha to the table at [A, a]
        set<string> alphaFirst = computeFirstOfSequence(alpha);
        for (const auto &a : alphaFirst) {
            if (a != EPSILON) {
                parsingTable[A][a] = joinProduction(alpha);  // Store production as a string (adjust to your needs)
            }
        }

        // If epsilon is in FIRST(alpha), add alpha to the table at [A, b] for each terminal b in FOLLOW(A)
        if (alphaFirst.find(EPSILON) != alphaFirst.end()) {
            for (const auto &b : followSets.at(A)) {
                parsingTable[A][b] = joinProduction(alpha);
            }
        }

        // If epsilon is in FIRST(alpha) and $ is in FOLLOW(A), add alpha to the table at [A, $]
        if (alphaFirst.find(EPSILON) != alphaFirst.end() &&
            followSets.at(A).find(END) != followSets.at(A).end()) {
            parsingTable[A][END] = joinProduction(alpha);
        }
    }

    set<string> computeFirstOfSequence(const vector<string>& sequence) {
        set<string> result;
        bool allCanBeEmpty = true;

        for (const auto& symbol : sequence) {
            set<string> symFirst = firstSets[symbol];

            // Add all non-epsilon symbols from FIRST(symbol)
            for (const auto& f : symFirst) {
                if (f != EPSILON) result.insert(f);
            }

            // If this symbol can't produce epsilon, stop here
            if (symFirst.find(EPSILON) == symFirst.end()) {
                allCanBeEmpty = false;
                break;
            }
        }

        if (allCanBeEmpty && !sequence.empty()) {
            result.insert(EPSILON);
        }

        return result;
    }

    string joinProduction(const vector<string>& production) {
        string result = "";
        for (const string& symbol : production) {
            result += symbol + " ";  // Concatenate symbols with space
        }
        return result;
    }

    unordered_map<string, unordered_map<string, string>> getTable() {
        if (!tableComputed) {
            computeTable();
            tableComputed = true;
        }
        return parsingTable;
    }

    void printTable() {
        for (const auto& nonTerminal : nonTerminals) {
            cout << "Non-terminal: " << nonTerminal << endl;
            for (const auto& entry : parsingTable[nonTerminal]) {
                cout << "  Terminal: " << entry.first << " => Production: " << entry.second << endl;
            }
            cout << endl;
        }
    }

};

#endif