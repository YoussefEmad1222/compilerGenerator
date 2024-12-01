#include "constants.h"
#include "NFA.h"

using namespace std;

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