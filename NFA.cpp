#include <iostream>
#include <unordered_map>
#include <vector>
#include <string>

using namespace std;

class stateNFA {
    long long id;
    unordered_map<char, vector<stateNFA *>> transitions;
    bool isFinal;
    string nameIfFinal;

public:
    stateNFA(long long id) : id(id), isFinal(false), nameIfFinal("") {}

    void addTransition(char c, stateNFA *state) {
        transitions[c].push_back(state);
    }

    void addTransitions(char c, const vector<stateNFA *> &states) {
        for (const auto &state: states) {
            transitions[c].push_back(state);
        }
    }

    void setFinal(const string &name) {
        isFinal = true;
        nameIfFinal = name;
    }

    long long getID() const {
        return id;
    }

    bool isFinalState() const {
        return isFinal;
    }

    string getNameIfFinal() const {
        return nameIfFinal;
    }
    auto getTransitions()  {
        return transitions;
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
        start->addTransition('\0', end);
    }
    NFA() = default;

    NFA(stateNFA *start, stateNFA *end) : start(start), end(end) {}


    stateNFA* getStart() const {
        return start;
    }

    stateNFA* getEnd() const {
        return end;
    }
};

class nfaOperations {
public:
    NFA concatenationNFA(NFA &nfa1, NFA &nfa2) {
        nfa1.getEnd()->addTransition('\0', nfa2.getStart());
        return NFA(nfa1.getStart(), nfa2.getEnd());
    }

    NFA unionNFA(NFA &nfa1, NFA &nfa2, long long &globalStateID) {
        stateNFA *newStart = new stateNFA(globalStateID++);
        stateNFA *newEnd = new stateNFA(globalStateID++);
        newStart->addTransitions('\0', {nfa1.getStart(), nfa2.getStart()});
        nfa2.getEnd()->addTransition('\0', newEnd);
        nfa1.getEnd()->addTransition('\0', newEnd);
        return NFA(newStart, newEnd);
    }

    NFA kleene_closureNFA(NFA &nfa, long long &globalStateID) {
        stateNFA *newStart = new stateNFA(globalStateID++);
        stateNFA *newEnd = new stateNFA(globalStateID++);
        newStart->addTransitions('\0', {nfa.getStart(), newEnd});
        nfa.getEnd()->addTransitions('\0', {nfa.getStart(), newEnd});
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
        endState->setFinal(string(1, c));
        return NFA(startState, endState);
    }
};