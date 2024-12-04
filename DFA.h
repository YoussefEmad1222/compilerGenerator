//
// Created by Mariam on 11/29/2024.
//

#ifndef DFA_H
#define DFA_H
#include <bits/stdc++.h>

using namespace std;

typedef long long State;

struct DFA {
    State startState;
    unordered_map<State, string> acceptStates;
    unordered_set<char> inputs;
    map<pair<State, char>, State> transitions;

    DFA(): startState(0) {
    }


    DFA(State startState, unordered_map<State, string> acceptStates, unordered_set<char> inputs,
        map<pair<State, char>, State> transitions)
    : startState(std::move(startState)),
      acceptStates(std::move(acceptStates)),
      inputs(std::move(inputs)),
      transitions(std::move(transitions)) {}

    
    bool move(const State& state, const char& input, State& nextState) {
        // Make sure that the transition exists first
        // This is done to not insert a new transition with 'state' and 'input'
        if(transitions.find({state, input}) != transitions.end()){
            nextState = transitions[{state, input}];
            return true;
        }
    
        return false;
    }
};

#endif //DFA_H

