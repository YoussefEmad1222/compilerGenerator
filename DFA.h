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

    void writeAllStatesToFile(const string &filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        unordered_set<State> states;
        for (const auto &[fst, snd]: transitions) {
            states.insert(fst.first);
        }

        // States
        file << "States: ";
        for (const auto &state: states) {
            file << state << " ";
        }
        file << endl;

        // Accepting States
        file << "Accepting States: ";
        for (const auto &[fst, snd]: acceptStates) {
            file << fst << " ";
        }
        file << endl;

        // Transitions current,next,input
        for (const auto &[fst, snd]: transitions) {
            file << fst.first << "," << snd << "," << string(1, fst.second) << endl;
        }

        file.close();
    }
};

#endif //DFA_H

