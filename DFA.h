//
// Created by Mariam on 11/29/2024.
//

#ifndef DFA_H
#define DFA_H
#include <bits/stdc++.h>

using namespace std;

struct DFA {
    long long startState;
    unordered_map<long long, string> acceptStates;
    unordered_set<char> inputs;
    map<pair<long long, char>, long long> transitions;

    DFA(): startState(0) {
    }

    DFA(long long startState, unordered_map<long long, string> acceptStates, unordered_set<char> inputs,
        map<pair<long long, char>, long long> transitions)
    : startState(std::move(startState)),
      acceptStates(std::move(acceptStates)),
      inputs(std::move(inputs)),
      transitions(std::move(transitions)) {}

    long long move(const long long& state, const char& input) {
        return transitions[{state, input}];
    }
};

#endif //DFA_H

