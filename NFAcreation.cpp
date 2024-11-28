#include "NFA.cpp"
#include <iostream>
#include <unordered_map>
#include <unordered_set>
#include <vector>
#include <stack>
#include <string>

class nfaCreation {
    long long globalStateID = 0;
    unordered_map<string, NFA> regexNFAs;

    NFA thompsonConstruction(string postfixRegex, string name) {
        stack<NFA> nfaStack;
        int n = postfixRegex.length();
        nfaOperations operations;
        for (int i = 0; i < n; i++) {
            char c = postfixRegex[i];
            switch (c) {
                case '\\': {
                    if (postfixRegex[i + 1] == 'L') {
                        nfaStack.push(operations.characterNFA('\0', globalStateID));
                        i++;
                    }
                    nfaStack.push(operations.characterNFA(postfixRegex[++i], globalStateID));
                    break;
                }
                case '|': {
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaStack.push(operations.unionNFA(nfa1, nfa2, globalStateID));
                    break;
                }
                case '+':
                    nfaStack.push(operations.oneOrMore_closureNFA(nfaStack.top(), globalStateID));
                    break;
                case '*':
                    nfaStack.push(operations.kleene_closureNFA(nfaStack.top(), globalStateID));
                    break;
                case '?': {
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();
                    nfaStack.push(operations.concatenationNFA(nfa1, nfa2));
                    break;
                }
                default: {
                    nfaStack.push(operations.characterNFA(c, globalStateID));
                    break;
                }
            }
        }
        NFA resultNFA = nfaStack.top();
        resultNFA.getEnd()->setFinal(name);
        return resultNFA;
    }

public:
    nfaCreation(){
        globalStateID = 0;
        regexNFAs.clear();
    }

    void createRegexNFAs(unordered_map<string, string> &regex, vector<string> &orderedRegex) {
        for (const string &orderedReg: orderedRegex) {
            NFA nfa = thompsonConstruction(regex[orderedReg], orderedReg);
            regexNFAs[orderedReg] = nfa;
        }
    }

    void printState(stateNFA &nfa, unordered_set<long long> &set, int indent = 0) {
        if (set.find(nfa.getID()) != set.end()) return;
        set.insert(nfa.getID());

        string indentation(indent, ' ');
        cout << indentation << "State ID: " << nfa.getID() << (nfa.isFinalState() ? " (Final)" : "") << endl;
        cout << indentation << "Transitions:" << endl;

        for (const auto &transition: nfa.getTransitions()) {
            cout << indentation << "  ON ";
            if (transition.first != '\0') {
                cout << transition.first << " -> ";
            } else {
                cout << "EPSILON -> ";
            }
            for (const auto &state: transition.second) {
                cout << state->getID() << " ";
            }
            cout << endl;
        }
        cout << endl;

        for (const auto &transition: nfa.getTransitions()) {
            for (const auto &state: transition.second) {
                printState(*state, set, indent + 2);
            }
        }
    }

    void printPaths(unordered_map<string, NFA> &regexNfa, unordered_set<long long> &set) {
        for (const auto &regexNFA: regexNfa) {
            cout << "Regex: " << regexNFA.first << endl;
            printState(*regexNFA.second.getStart(), set);
        }
    }

    void printAllStates() {
        cout << "Printing all states: " << endl;
        unordered_set<long long> visited;
        printPaths(regexNFAs, visited);
    }
};
