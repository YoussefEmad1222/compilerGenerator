//
// Created by Mariam on 11/29/2024.
//

#include "DFACreator.h"

#include "constants.h"


DFACreator::DFACreator(const NFA nfa): nStart() {
    // Start state
    nStart = nfa.getStart()->getID();

    // Queue for BFS over NFA states
    queue<stateNFA *> stateQueue;
    unordered_set<ll> visited;

    stateQueue.push(nfa.getStart());
    visited.insert(nfa.getStart()->getID());

    // Process states
    while (!stateQueue.empty()) {
        stateNFA *current = stateQueue.front();
        stateQueue.pop();

        ll currentID = current->getID();

        // If the state is final, add it to the accepting states
        if (current->isFinalState()) {
            nAccept[currentID] = {current->getPriority(), current->getNameIfFinal()};
        }

        // Process transitions
        for (const auto &[input, destinations]: current->getTransitions()) {
            inputs.insert(input);

            for (stateNFA *dest: destinations) {
                // Add to transition map
                nTransitions[{currentID, input}].insert(dest->getID());

                // BFS to process new states
                if (visited.find(dest->getID()) == visited.end()) {
                    stateQueue.push(dest);
                    visited.insert(dest->getID());
                }
            }
        }
    }
}


/*
 * This method implements subset construction algorithm to convert NFA to DFA
 */
void DFACreator::createDFA() {
    map<set<ll>, ll> dStates;
    map<pair<ll, char>, ll> dTransitions;
    queue<set<ll>> unmarked;
    unordered_map<long long, string> dAccept;
    int id = 0;

    // Initialize DFA with start state and its eps-closure
    const set<ll> startStates = eps_closure(set{nStart});
    dStates[startStates] = id++;
    unmarked.push(startStates);
    ll dStart = dStates[startStates];

    // Remove epsilon from the set of available inputs
    // No eps transitions in DFA
    inputs.erase(EPSILON);

    // Loop till marking (processing) all states in DFA
    while(!unmarked.empty()) {
        set<ll> state = unmarked.front();
        unmarked.pop();

        for(const auto& input : inputs) {
            set<ll> e_closure = eps_closure(move(state, input));
            if(!e_closure.empty() && dStates.find(e_closure) == dStates.end()) {
                const int stateId = id++;
                dStates[e_closure] = stateId;
                unmarked.push(e_closure);

                // Mark state as accepting if at least one of its elements is accepting
                vector<pair<int, string>> acceptingStates;
                for(auto s: e_closure) {
                    if(nAccept.find(s) != nAccept.end()) {
                        acceptingStates.push_back({nAccept[s].first, nAccept[s].second});
                    }
                }

                // Sort by priority
                sort(acceptingStates.rbegin(), acceptingStates.rend());
                // Set final state as the state of highest priority
                if(acceptingStates.size() != 0) {
                    dAccept[stateId] = acceptingStates.begin()->second;
                }
            }
            // Add state and transition input to DFA transition table
            if(!e_closure.empty())
                dTransitions[{dStates[state], input}] = dStates[e_closure];
        }
    }

    dfa = {dStart, dAccept, inputs, dTransitions};
}


/*
 * Return the final DFA
 */
DFA DFACreator::getDFA() {
    return dfa;
}


void DFACreator::writeAllStatesToFile(const string &filename) {
    dfa.writeAllStatesToFile(filename);
}


/*
 * Returns the set of states with epsilon transitions from "states" in the given NFA
 */
set<ll> DFACreator::eps_closure(const set<ll> &states) {
    stack<ll> stack;
    set<ll> e_closure;

    for(const auto& state : states) {
        stack.push(state);
        e_closure.insert(state);
    }

    while(!stack.empty()) {
        ll state = stack.top();
        stack.pop();
        for(const auto& u : nTransitions[{state, EPSILON}]) {
            e_closure.insert(u);
            stack.push(u);
        }
    }

    return e_closure;
}


/*
 * Returns next states from the transition of "states" under "input" in the given NFA
 */
set<ll> DFACreator::move(const set<ll>& states, const char& input) {
    set<ll> nextStates;
    for(const auto& state : states) {
        for(const auto& u : nTransitions[{state, input}]) {
            nextStates.insert(u);
        }
    }
    return nextStates;
}