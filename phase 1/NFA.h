//
// Created by Mariam on 12/1/2024.
//

#ifndef NFA_H
#define NFA_H

#include "constants.h"
using namespace std;

class stateNFA {
    long long id;
    unordered_map<char, vector<stateNFA *> > transitions;
    bool isFinal;
    string nameIfFinal;
    int priority;

public:
    stateNFA(long long id) : id(id), isFinal(false), nameIfFinal(""), priority(0) {
    }

    void addTransition(char c, stateNFA *state) {
        transitions[c].push_back(state);
    }

    void addTransitions(char c, const vector<stateNFA *> &states) {
        for (const auto &state: states) {
            transitions[c].push_back(state);
        }
    }

    void setFinal(const string &name, int priority) {
        isFinal = true;
        nameIfFinal = name;
        this->priority = priority;
    }

    void removeFinal() {
        isFinal = false;
        nameIfFinal = "";
    }

    long long getID() const {
        return id;
    }

    bool isFinalState() const {
        return isFinal;
    }

    string getNameIfFinal() const {
        if (nameIfFinal.empty()) {
            return "";
        }
        return nameIfFinal;
    }


    auto getTransitions() {
        return transitions;
    }

    int getPriority() const {
      return priority;
    }

    void print() const {
        cout << "State ID: " << id << endl;
        cout << "Final: " << isFinal << endl;
        cout << "Name if final: " << nameIfFinal << endl;
        cout << "Transitions: " << endl;
        for (const auto &transition: transitions) {
            cout << transition.first << " -> ";
            for (const auto &state: transition.second) {
                cout << state->id << " ";
            }
            cout << endl;
        }
    }
};

class NFA {
    stateNFA *start;
    stateNFA *end;

public:
    NFA(long long &globalStateID) {
        start = new stateNFA(globalStateID++);
        end = new stateNFA(globalStateID++);
        start->addTransition(EPSILON, end);
    }

    NFA() = default;

    NFA(stateNFA *start, stateNFA *end) : start(start), end(end) {
    }


    stateNFA *getStart() const {
        return start;
    }

    stateNFA *getEnd() const {
        return end;
    }
};

class nfaOperations {
public:
    NFA concatenationNFA(NFA &nfa1, NFA &nfa2) {
        stateNFA* endState = nfa1.getEnd();
        stateNFA* startState = nfa2.getStart();
        for (const auto& transition : startState->getTransitions()) {
            endState->addTransitions(transition.first, transition.second);
        }
        return NFA(nfa1.getStart(), nfa2.getEnd());
    }

    NFA unionNFA(NFA &nfa1, NFA &nfa2, long long &globalStateID) {
        stateNFA *newStart = new stateNFA(globalStateID++);
        stateNFA *newEnd = new stateNFA(globalStateID++);
        newStart->addTransitions(EPSILON, {nfa1.getStart(), nfa2.getStart()});
        nfa2.getEnd()->addTransition(EPSILON, newEnd);
        nfa1.getEnd()->addTransition(EPSILON, newEnd);
        return NFA(newStart, newEnd);
    }

    NFA kleene_closureNFA(NFA &nfa, long long &globalStateID) {
        stateNFA *newStart = new stateNFA(globalStateID++);
        stateNFA *newEnd = new stateNFA(globalStateID++);
        newStart->addTransitions(EPSILON, {nfa.getStart(), newEnd});
        nfa.getEnd()->addTransitions(EPSILON, {nfa.getStart(), newEnd});
        return NFA(newStart, newEnd);
    }

    NFA oneOrMore_closureNFA(NFA &nfa, long long &globalStateID) {
        NFA kleeneClosure = kleene_closureNFA(nfa, globalStateID);
        return concatenationNFA(nfa, kleeneClosure);
    }

    NFA characterNFA(char c, long long &globalStateID) {
        stateNFA *startState = new stateNFA(globalStateID++);
        stateNFA *endState = new stateNFA(globalStateID++);
        startState->addTransition(c, endState);
        return NFA(startState, endState);
    }
};

#endif //NFA_H
