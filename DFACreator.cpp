//
// Created by Mariam on 11/29/2024.
//

#include "DFACreator.h"

#include "constants.h"

// TODO: change this to take NFA in its current form and create the needed data structures
// TODO: Handle final states by names and priority
DFACreator::DFACreator(
    ll &nStart,
     unordered_set<ll> &nAccept,
    map<pair<ll, char>, set<ll> > &nTransitions,
     set<char> &inputs)
    : nStart(nStart),
      nAccept(nAccept),
      nTransitions(nTransitions),
      inputs(inputs) {}

DFACreator::DFACreator(const NFA nfa): nStart(), nAccept(), nTransitions(), inputs() {
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
            nAccept.insert(currentID);
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
 * Returns the final DFA
 */
DFA DFACreator::getDFA() const {
    map<set<ll>, ll> dStates;
    map<pair<ll, char>, ll> dTransitions;
    queue<set<ll>> unmarked;
    set<ll> dAccept;
    int id = 0;

    // Initialize DFA with start state and its eps-closure
    const set<ll> startStates = eps_closure(set{nStart}, nTransitions);
    dStates[startStates] = id++;
    unmarked.push(startStates);
    ll dStart = dStates[startStates];

    // Loop till marking (processing) all states in DFA
    while(!unmarked.empty()) {
        set<ll> state = unmarked.front();
        unmarked.pop();

        for(const auto& input : inputs) {
            if(input == EPSILON) // No eps transitions in DFA
                continue;

            set<ll> e_closure = eps_closure(move(state, input, nTransitions), nTransitions);
            if(!e_closure.empty() && dStates.find(e_closure) == dStates.end()) {
                dStates[e_closure] = id++;
                unmarked.push(e_closure);

                // Mark state as accepting if at least one of its elements is accepting
                for(const auto& s: e_closure) {
                    if(nAccept.find(s) != nAccept.end()) {
                        dAccept.insert(dStates[e_closure]);
                        break;
                    }
                }
            }

            // Add state and transition input to DFA transition table
            dTransitions[{dStates[state], input}] = dStates[e_closure];
        }
    }

    return {dStart, dAccept, inputs, dTransitions};
}

/*
 * Returns the set of states with epsilon transitions from "states" in the given NFA
 */
set<ll> DFACreator::eps_closure(const set<ll> &states, map<pair<ll, char>, set<ll>> nTransitions) {
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
set<ll> DFACreator::move(const set<ll>& states, const char& input, map<pair<ll, char>, set<ll>> nTransitions) {
    set<ll> nextStates;
    for(const auto& state : states) {
        for(const auto& u : nTransitions[{state, input}]) {
            nextStates.insert(u);
        }
    }
    return nextStates;
}