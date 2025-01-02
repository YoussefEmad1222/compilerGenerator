//
// Created by Mariam on 12/22/2024.
//

#ifndef FIRSTFOLLOWCALCULATOR_H
#define FIRSTFOLLOWCALCULATOR_H

#include <bits/stdc++.h>

using namespace std;

class FirstFollowCalculator {
    unordered_map<string, vector<string> > grammar;
    vector<string> nonTerminals;
    unordered_map<string, set<string> > first;
    unordered_map<string, set<string> > follow;
    unordered_map<string, bool> hasEpsilon;

    // Function to check if a token is a terminal
    bool isTerminal(const string &token) {
        return token.front() == '\'' && token.back() == '\'';
    }

    // Inserts token into the set of tokens, and returns false if it already exists
    bool insertToken(set<string> &tokens, string token) {
        if (tokens.find(token) == tokens.end()) {
            tokens.insert(token);
            return true;
        }
        return false;
    }

    // Merges both sets into set1, and returns true if set1 changes
    bool unionSets(set<string> &set1, set<string> &set2) {
        bool isChanged = false;
        for (auto token: set2) {
            isChanged |= insertToken(set1, token);
        }
        return isChanged;
    }

    void initFirst();

public:
    FirstFollowCalculator(unordered_map<string, vector<string> > grammar, vector<string> nonTerminals);

    void calculateFirst();

    void calculateFollow();

    unordered_map<string, set<string> > getFirst();

    unordered_map<string, set<string> > getFollow();

    static vector<string> splitByDelimiter(const string &str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }
};

#endif //FIRSTFOLLOWCALCULATOR_H
