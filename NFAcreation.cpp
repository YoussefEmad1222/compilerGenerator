#include "NFA.cpp"
#include "constants.h"
#include <functional>

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
                case ESCAPE_CHARACTER: {
                    if (postfixRegex[i + 1] == 'L') {
                        nfaStack.push(operations.characterNFA(EPSILON, globalStateID));
                        i++;
                    }
                    nfaStack.push(operations.characterNFA(postfixRegex[++i], globalStateID));
                    break;
                }
                case UNION_OPERATOR: {
                    NFA nfa1 = nfaStack.top();
                    nfaStack.pop();
                    NFA nfa2 = nfaStack.top();
                    nfaStack.pop();
                    nfaStack.push(operations.unionNFA(nfa1, nfa2, globalStateID));
                    break;
                }
                case PLUS_OPERATOR:
                    nfaStack.push(operations.oneOrMore_closureNFA(nfaStack.top(), globalStateID));
                    break;
                case KLEENE_STAR_OPERATOR:
                    nfaStack.push(operations.kleene_closureNFA(nfaStack.top(), globalStateID));
                    break;
                case CONCATENATION_OPERATOR: {
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
    nfaCreation() {
        globalStateID = 0;
        regexNFAs.clear();
    }

    void createRegexNFAs(unordered_map<string, string> &regex, vector<string> &orderedRegex,
                         vector<string> &keywords, vector<string> &punctuations) {
        for (const string &orderedReg: orderedRegex) {
            NFA nfa = thompsonConstruction(regex[orderedReg], orderedReg);
            regexNFAs[orderedReg] = nfa;
        }

        for (const string &keyword: keywords) {
            NFA nfa = thompsonConstruction(keyword, keyword);
            regexNFAs[keyword] = nfa;
        }
        for (const string &punctuation: punctuations) {
            NFA nfa = thompsonConstruction(punctuation, punctuation);
            regexNFAs[punctuation] = nfa;
        }

    }

    void printState(stateNFA &nfa, unordered_set<long long> &set, int indent = 0) {
        if (set.find(nfa.getID()) != set.end()) return;
        set.insert(nfa.getID());

        string indentation(indent, ' ');
        cout << indentation << "State ID: " << nfa.getID()
             << (!nfa.getNameIfFinal().empty() ? " (Final: " + nfa.getNameIfFinal() + ")" : "") << endl;

        if (nfa.getTransitions().empty()) {
            cout << indentation << "  NO TRANSITIONS" << endl;
        } else {
            cout << indentation << "  TRANSITIONS: " << endl;
        }

        for (const auto &transition: nfa.getTransitions()) {
            cout << indentation << "  ON ";
            if (transition.first != EPSILON) {
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
