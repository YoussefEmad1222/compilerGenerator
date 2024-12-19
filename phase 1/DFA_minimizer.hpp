
#ifndef DFA_MINIMIZER_HPP
#define DFA_MINIMIZER_HPP

#include <bits/stdc++.h>
#include "DFA.h"

using namespace std;


template<typename T>
void printSet(const std::set<T> &s, const std::string &name) {
    std::cout << name << ": { ";
    for (const auto &element: s) {
        std::cout << element << " ";
    }
    std::cout << "}\n";
}

static inline DFA minimizeDFA(const DFA &dfa) {
    vector<set<State> > partition;
    set<State> finalStates, nonFinalStates;
    if (dfa.acceptStates.count(dfa.startState)) {
        finalStates.insert(dfa.startState);
    } else {
        nonFinalStates.insert(dfa.startState);
    }

    for (const auto &trans: dfa.transitions) {
        if (dfa.acceptStates.count(trans.second)) {
            finalStates.insert(trans.second);
        } else {
            nonFinalStates.insert(trans.second);
        }

        if (dfa.acceptStates.count(trans.first.first)) {
            finalStates.insert(trans.first.first);
        } else {
            nonFinalStates.insert(trans.first.first);
        }
    }

    // Z

    partition.push_back(finalStates);
    partition.push_back(nonFinalStates);

    bool changed = true;

    // Refine partition until no changes
    while (changed) {
        changed = false;
        std::vector<std::set<long long> > newPartition;

        // Map to track which partition each state belongs to
        std::map<long long, size_t> stateToGroup;
        for (size_t i = 0; i < partition.size(); ++i) {
            for (auto state: partition[i]) {
                stateToGroup[state] = i;
            }
        }

        for (const auto &group: partition) {
            // Divide the group based on state signatures
            std::map<std::vector<pair<int, string> >, std::set<long long> > groups;

            for (auto state: group) {
                std::vector<pair<int, string> > signature;
                std::string myToken = dfa.acceptStates.find(state) != dfa.acceptStates.end()
                                          ? dfa.acceptStates.at(state)
                                          : "";

                for (char input: dfa.inputs) {
                    auto iter = dfa.transitions.find({state, input});
                    if (iter != dfa.transitions.end()) {
                        long long target = iter->second;

                        // Check if the target state is in the partition groups
                        if (stateToGroup.find(target) != stateToGroup.end()) {
                            // Find the associated string from acceptStates
                            std::string targetToken = dfa.acceptStates.find(target) != dfa.acceptStates.end()
                                                          ? dfa.acceptStates.at(target)
                                                          : "";

                            // Add the group index and the string to the signature
                            signature.push_back({stateToGroup[target], targetToken});
                        } else {
                            signature.push_back({-1, myToken}); // Invalid transition
                        }
                    } else {
                        signature.push_back({-1, myToken}); // No transition for this input
                    }
                }
                groups[signature].insert(state);
            }

            // Add newly divided groups to the new partition
            for (const auto &entry: groups) {
                if (entry.second.size() < group.size()) changed = true;
                newPartition.push_back(entry.second);
            }
        }


        // cout << "new partition\n\n\n\n";
        // std::cout << "Partition contents:\n";
        // for (size_t i = 0; i < partition.size(); ++i) {
        //     std::cout << "Group " << i << ": { ";
        //     for (auto state : partition[i]) {
        //         std::cout << state << " ";
        //         auto it = dfa.acceptStates.find(state);
        //         if (it != dfa.acceptStates.end()) {
        //             std::cout << "State " << state << " has string: " << it->second << std::endl;
        //         } else {
        //             std::cout << "State " << state << " is NOT in acceptStates." << std::endl;
        //         }
        //     }
        //     std::cout << "}\n";
        // }

        // fflush(NULL);

        // cout << "\n\n\nold partitions\n\n\n\n";
        // printPartition(partition);
        partition = newPartition;
    }


    // Construct new minimized DFA
    unordered_map<long long, string> newAcceptStates;
    unordered_map<long long, long long> representative;
    map<pair<long long, char>, long long> newTransitions;
    long long newStartState = -1;
    long long newStateId = 0;
    bool startStateAssigned = false;

    for (const auto &group: partition) {
        long long repr = *group.begin();

        if (group.count(dfa.startState) && !startStateAssigned) {
            newStartState = 0; // Assign ID 0 to the start state
            for (auto state: group) representative[state] = 0;
            startStateAssigned = true;
        } else {
            newStateId++;
            for (auto state: group) representative[state] = newStateId;
        }

        string mergedAcceptToken;
        for (auto state: group) {
            if (dfa.acceptStates.count(state)) {
                mergedAcceptToken = dfa.acceptStates.at(state);
                newAcceptStates[representative[repr]] = dfa.acceptStates.at(state);
                break;
            }
        }
    }

    for (const auto &trans: dfa.transitions) {
        long long from = representative[trans.first.first];
        char input = trans.first.second;
        long long to = representative[trans.second];
        newTransitions[{from, input}] = to;
    }

    return DFA(newStartState, newAcceptStates, dfa.inputs, newTransitions);
}

#endif // DFA_MINIMIZER_HPP
