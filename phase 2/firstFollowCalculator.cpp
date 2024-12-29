//
// Created by Mariam on 12/22/2024.
//

#include "firstFollowCalculator.h"

FirstFollowCalculator::FirstFollowCalculator(unordered_map<string, vector<string>> grammar, vector<string> nonTerminals) {
    this->grammar = grammar;
    this->nonTerminals = nonTerminals;
    initFirst();
}

void FirstFollowCalculator::initFirst() {
    for(string nonTerminal : nonTerminals) {
        for(string production : grammar[nonTerminal]) {
            vector<string> tokens = splitByDelimiter(production, ' ');
            for(string token : tokens) {
                if(isTerminal(token)) // First(Terminal) = {Terminal}
                    first[token].insert(token);
            }
        }
    }
}

void FirstFollowCalculator::calculateFirst() {
    bool isSetChanged;
    do {
        isSetChanged = false;

        for (string nonTerminal : nonTerminals) {
            for (string production : grammar[nonTerminal]) {
                vector<string> tokens = splitByDelimiter(production, ' ');
                bool prevHasEps = true;
                for(string token : tokens) {
                    // If X -> EPSILON is a production, then add EPSILON to FIRST(X) .
                    if(token == EPSILON) {
                        isSetChanged |= insertToken(first[nonTerminal], token);
                        hasEpsilon[nonTerminal] = true;
                    } else if(prevHasEps) {
                        isSetChanged |= unionSets(first[nonTerminal], first[token]);
                        prevHasEps = hasEpsilon[token];
                    }
                }
            }
        }
    } while(isSetChanged);
}

void FirstFollowCalculator::calculateFollow() {
    // Place $ in FOLLOW(S), where S is the start symbol, and $ is the input right endmarker.
    string startSymbol = nonTerminals.front();
    follow[startSymbol].insert(END_SYMBOL);
    bool isSetChanged;

    do {
        isSetChanged = false;

        for (string nonTerminal : nonTerminals) {
            for (string production : grammar[nonTerminal]) {
                vector<string> tokens = splitByDelimiter(production, ' ');
                // If there is a production A -> aBb, then everything in FIRST(b) except
                // EPSILON is in FOLLOW(B).
                for(int i = 1; i < tokens.size(); i++) {
                    if(isTerminal(tokens[i-1]))
                        continue;
                    isSetChanged |= unionSets(follow[tokens[i-1]], first[tokens[i]]);
                }

                // If there is a production A -> aB, or a production A -> aBb, where FIRST(b)
                // contains EPSILON, then everything in FOLLOW(A) is in FOLLOW(B).
                for(int i = tokens.size() - 1; i >= 0 && !isTerminal(tokens[i]); i--) {
                    isSetChanged |= unionSets(follow[tokens[i]], follow[nonTerminal]);
                    if(!hasEpsilon[tokens[i]])
                        break;
                }
            }
        }
    } while(isSetChanged);

    // Follow set should not contain EPSILON
    for(string nonTerminal : nonTerminals) {
        follow[nonTerminal].erase(EPSILON);
    }
}

unordered_map<string, set<string>> FirstFollowCalculator::getFirst() {
    return first;
}

unordered_map<string, set<string> > FirstFollowCalculator::getFollow() {
    return follow;
}

