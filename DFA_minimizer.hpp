
#ifndef DFA_MINIMIZER_HPP
#define DFA_MINIMIZER_HPP

#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;


static inline DFA removeUnreachableStates(const DFA& dfa) {
    set<long long> reachableStates;
    queue<long long> toVisit;
    toVisit.push(dfa.startState);
    reachableStates.insert(dfa.startState);

    while (!toVisit.empty()) {
        long long state = toVisit.front();
        toVisit.pop();
        for (char input : dfa.inputs) {
            auto iter = dfa.transitions.find({state, input});
            if (iter != dfa.transitions.end()) {
                long long nextState = iter->second;
                if (!reachableStates.count(nextState)) {
                    reachableStates.insert(nextState);
                    toVisit.push(nextState);
                }
            }
        }
    }

    // Construct the filtered DFA
    unordered_map<long long, string> newAcceptStates;
    map<pair<long long, char>, long long> newTransitions;
    for (const auto& trans : dfa.transitions) {
        if (reachableStates.count(trans.first.first) &&
            reachableStates.count(trans.second)) {
            newTransitions[trans.first] = trans.second;
        }
    }

    for (const auto& state : dfa.acceptStates) {
        if (reachableStates.count(state.first)) {
            newAcceptStates[state.first] = state.second;
        }
    }

    return DFA(dfa.startState, newAcceptStates, dfa.inputs, newTransitions);
}

static inline DFA minimizeDFA(const DFA& dfa) {
    DFA filteredDFA = removeUnreachableStates(dfa);

    // Initialize states and partition
    set<long long> states;
    for (const auto& trans : filteredDFA.transitions) {
        states.insert(trans.first.first);
        states.insert(trans.second);
    }

    vector<set<long long>> partition;
    set<long long> finalStates, nonFinalStates;

    for (auto state : states) {
        if (filteredDFA.acceptStates.count(state)) {
            finalStates.insert(state);
        } else {
            nonFinalStates.insert(state);
        }
    }

    if (!finalStates.empty()) partition.push_back(finalStates);
    if (!nonFinalStates.empty()) partition.push_back(nonFinalStates);

    bool changed = true;

    // Refine partition until no changes
    while (changed) {
        changed = false;
        vector<set<long long>> newPartition;

        for (const auto& group : partition) {
            map<vector<int>, set<long long>> groups;

            for (auto state : group) {
                vector<int> signature;

                // Create a signature for the state based on transitions and partition groups
                for (char input : filteredDFA.inputs) {
                    auto iter = filteredDFA.transitions.find({state, input});
                    if (iter != filteredDFA.transitions.end()) {
                        auto target = iter->second;
                        size_t index = 0;
                        for (; index < partition.size(); ++index)
                            if (partition[index].count(target)) break;

                        signature.push_back(index);
                    } else {
                        signature.push_back(-1);
                    }
                }
                groups[signature].insert(state);
            }

            for (const auto& entry : groups) {
                if (entry.second.size() < group.size())
                    changed = true;
                newPartition.push_back(entry.second);
            }
        }

        partition = move(newPartition);
    }

    // Construct new minimized DFA
    unordered_map<long long, string> newAcceptStates;
    unordered_map<long long, long long> representative;
    map<pair<long long, char>, long long> newTransitions;
    long long newStartState = -1;
    long long newStateId = 0;
    bool startStateAssigned = false;

    for (const auto& group : partition) {
        long long repr = *group.begin(); // Representative of the group

        if (group.count(filteredDFA.startState) && !startStateAssigned) {
            newStartState = 0; // Assign ID 0 to the start state
            for (auto state : group) representative[state] = 0;
            startStateAssigned = true;
        } else {
            newStateId++;
            for (auto state : group) representative[state] = newStateId;
        }

        string mergedAcceptToken;
        for (auto state : group) {
            if (filteredDFA.acceptStates.count(state)) {
                // This is an assumption I made on how to deal with partitions that have multiple accept states, each
                // with their own token, so currently I only consider one, but i left the option of merging below just in case
#ifdef MERGE_ACCEPT_STATES_TOKENS
                if (!mergedAcceptToken.empty()) {
                    mergedAcceptToken += ",";
                }
                mergedAcceptToken += dfa.acceptStates.at(state);
#else
                mergedAcceptToken = filteredDFA.acceptStates.at(state);
#endif
            }
        }

        if (!mergedAcceptToken.empty()) {
            newAcceptStates[representative[repr]] = mergedAcceptToken;
        }
    }

    for (const auto& trans : filteredDFA.transitions) {
        long long from = representative[trans.first.first];
        char input = trans.first.second;
        long long to = representative[trans.second];
        newTransitions[{from, input}] = to;
    }

    return DFA(newStartState, newAcceptStates, filteredDFA.inputs, newTransitions);
}

#endif // DFA_MINIMIZER_HPP