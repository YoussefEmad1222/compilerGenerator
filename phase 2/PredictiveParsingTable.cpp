#include "PredictiveParsingTable.h"
#include "FirstFollowCalculator.h"
#include "../constants.h"

using namespace std;

PredictiveParsingTable::PredictiveParsingTable(
    unordered_map<string, set<string> > first,
    unordered_map<string, set<string> > follow,
    unordered_map<string, vector<string> > grammar,
    vector<string> nonTerminals
) : first(first), follow(follow), grammar(grammar),
    nonTerminals(nonTerminals), tableComputed(false) {
}

void PredictiveParsingTable::computeTable() {
    if (first.empty()) throw invalid_argument("First sets are empty");
    if (follow.empty()) throw invalid_argument("Follow sets are empty");

    bool stillLL1 = true;
    for (const auto &production: grammar) {
        string NT = production.first;
        for (const auto &RHS: production.second) {
            stillLL1 = updateTableUsingProduction(NT, RHS);

            if(!stillLL1){
                throw invalid_argument("Grammar is not LL(1)");
            }
        }
    }

    for (const auto &NT: nonTerminals) {
        for (const auto &tokenInFollow: follow[NT]) {
            if (parsingTable[NT].find(tokenInFollow) == parsingTable[NT].end()) {
                parsingTable[NT][tokenInFollow] = ERR;
            }
        }
    }
}



bool PredictiveParsingTable::updateTableUsingProduction(const string &LHS, const string &RHS) {
    set<string> firstOfProduction = GetFirstOfProduction(RHS);

    // Add entries for terminals in First(RHS) (excluding EPSILON_STR)
    for (const auto &tokenInFirst : firstOfProduction) {
        if (tokenInFirst != EPSILON_STR) {
            if(parsingTable[LHS].find(tokenInFirst) != parsingTable[LHS].end() && parsingTable[LHS][tokenInFirst] != RHS){
                // Conflict detected
                cerr << "Conflict detected in parsing table: [" << LHS << ", " << tokenInFirst << "]" << endl;
                return false;
            }
            else    parsingTable[LHS][tokenInFirst] = RHS;
        }
    }

    // If EPSILON_STR is in First(RHS), add entries for Follow(LHS)
    if (firstOfProduction.find(EPSILON_STR) != firstOfProduction.end()) {
        for (const auto &tokenInFollow : follow[LHS]) {
            if(parsingTable[LHS].find(tokenInFollow) != parsingTable[LHS].end() && parsingTable[LHS][tokenInFollow] != RHS){
                // Conflict detected
                cerr << "Conflict detected in parsing table: [" << LHS << ", " << tokenInFollow << "]" << endl;
                return false;
            }
            else    parsingTable[LHS][tokenInFollow] = RHS;
        }
    }

    // If epsilon is in FIRST(alpha) and $ is in FOLLOW(A), add alpha to the table at [A, $]
    if (firstOfProduction.find(EPSILON_STR) != firstOfProduction.end() &&
        follow[LHS].find("$") != follow[LHS].end()) {
        if(parsingTable[LHS].find("$") != parsingTable[LHS].end() && parsingTable[LHS]["$"] != RHS){
            // Conflict detected
            cerr << "Conflict detected in parsing table: [" << LHS << ", " << "$" << "]" << endl;
            return false;
        }
        else    parsingTable[LHS]["$"] = RHS;
    }

    return true; // Successfully updated the parsing table
}



// set<string> PredictiveParsingTable::GetFirstOfProduction(const string &production) {
//     set<string> firstOfProduction;

//     vector<string> productionTokens = FirstFollowCalculator::splitByDelimiter(production, ' ');
//     for (const auto &token: productionTokens) {
//         set<string> firstOfToken = first[token];

//         firstOfProduction.insert(firstOfToken.begin(), firstOfToken.end());

//         if (firstOfProduction.find( EPSILON_STR) == firstOfProduction.end())
//             break;  // No need to continue because the EPS symbol is not in the first of this token so the next token's first shouldn't be added
        
//     }

//     return firstOfProduction;
// }

set<string> PredictiveParsingTable::GetFirstOfProduction(const string &production) {
    set<string> firstSetOfProduction;
    bool productionCanBeEPS = true; // Assume production can produce EPSILON until proven otherwise

    // Split production into tokens
    vector<string> productionTokens = FirstFollowCalculator::splitByDelimiter(production, ' ');

    for (const auto &token : productionTokens) {
        set<string> firstOfToken = first[token];

        // Add all non-EPSILON symbols from the First Set of the token
        for (const auto &f : firstOfToken) {
            if (f != EPSILON_STR) {
                firstSetOfProduction.insert(f);
            }
        }

        // If the token cannot produce EPSILON, the production cannot produce EPSILON
        if (firstOfToken.find(EPSILON_STR) == firstOfToken.end()) {
            productionCanBeEPS = false;
            break; // No need to check further tokens
        }
    }

    // If the entire production can produce EPSILON, add it to the First Set
    if (productionCanBeEPS) {
        firstSetOfProduction.insert(EPSILON_STR);
    }

    return firstSetOfProduction;
}


unordered_map<string, unordered_map<string, string> > PredictiveParsingTable::getTable() {
    if (!tableComputed) {
        computeTable();
        tableComputed = true;
    }
    return parsingTable;
}

void PredictiveParsingTable::printTable() {
    for (const auto &nonTerminal: nonTerminals) {
        cout << "Non-terminal: " << nonTerminal << endl;
        for (const auto &entry: parsingTable[nonTerminal]) {
            cout << "  Terminal: " << entry.first << " => Production: " << entry.second << endl;
        }
        cout << endl;
    }
}
