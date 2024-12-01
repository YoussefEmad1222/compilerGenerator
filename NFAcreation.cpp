#include "NFA.cpp"
#include "constants.h"
#include <functional>

class nfaCreation {
    long long globalStateID = 0;
    unordered_map<string, NFA> regexNFAs;
    ofstream out;
    unordered_set<long long> allStates;

    NFA thompsonConstruction(string postfixRegex, string name, int priority) {
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
                    NFA nfa1 = nfaStack.top();nfaStack.pop();
                    NFA nfa2 = nfaStack.top();nfaStack.pop();
                    nfaStack.push(operations.unionNFA(nfa1, nfa2, globalStateID));
                    break;
                }
                case PLUS_OPERATOR: {
                    // One or more closure
                    NFA nfa = nfaStack.top();nfaStack.pop();
                    nfaStack.push(operations.oneOrMore_closureNFA(nfa, globalStateID));
                    break;
                }
                case KLEENE_STAR_OPERATOR: {
                    NFA nfa = nfaStack.top();nfaStack.pop();
                    nfaStack.push(operations.kleene_closureNFA(nfa, globalStateID));
                    break;
                }
                case CONCATENATION_OPERATOR: {
                    NFA nfa2 = nfaStack.top();nfaStack.pop();
                    NFA nfa1 = nfaStack.top();nfaStack.pop();
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
        resultNFA.getEnd()->setFinal(name, priority);
        return resultNFA;
    }

    string removeConcat(string regex) {
        string newRegex;
        for (char c: regex) {
            if (c != CONCATENATION_OPERATOR) {
                newRegex += c;
            }
        }
        return newRegex;
    }

public:
    nfaCreation() {
        globalStateID = 0;
        regexNFAs.clear();
    }

    void createRegexNFAs(unordered_map<string, string> &regex, vector<string> &orderedRegex,
                         vector<string> &keywords, vector<string> &punctuations) {
        int priority = orderedRegex.size();
        for (const string &orderedReg: orderedRegex) {
            NFA nfa = thompsonConstruction(regex[orderedReg], orderedReg, priority--);
            regexNFAs[orderedReg] = nfa;
        }

        for (const string &keyword: keywords) {
            string orgKeyword = removeConcat(keyword);
            NFA nfa = thompsonConstruction(keyword, orgKeyword,orderedRegex.size()+1);
            regexNFAs[keyword] = nfa;
        }
        for (const string &punctuation: punctuations) {
            string orgPunctuation = removeConcat(punctuation);
            NFA nfa = thompsonConstruction(punctuation, orgPunctuation,orderedRegex.size()+1);
            regexNFAs[punctuation] = nfa;
        }
    }

    NFA *combineNFAs() {
        const auto newStart = new stateNFA(globalStateID++);

        for (const auto &[fst, snd]: regexNFAs) {
            newStart->addTransition(EPSILON, snd.getStart());
        }

        return new NFA(newStart, nullptr);
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

    void printPath(string name) {
        cout << "Printing path for: " << name << endl;
        unordered_set<long long> visited;
        printState(*regexNFAs[name].getStart(), visited);
    }

        void writeState(stateNFA &nfa, unordered_set<long long> &set, ofstream &file) {
        if (set.find(nfa.getID()) != set.end()) return;
        set.insert(nfa.getID());

        for (const auto &transition: nfa.getTransitions()) {
            for (const auto &state: transition.second) {
                file << nfa.getID() << "," << state->getID() << "," << (transition.first == EPSILON ? "EPSILON" : string(1, transition.first)) << endl;
            }
        }

        for (const auto &transition: nfa.getTransitions()) {
            for (const auto &state: transition.second) {
                writeState(*state, set, file);
            }
        }
    }

    void writePaths(unordered_map<string, NFA> &regexNfa, unordered_set<long long> &set, ofstream &file) {
        for (const auto &regexNFA: regexNfa) {
            writeState(*regexNFA.second.getStart(), set, file);
        }
    }

    void writeAllStatesToFile(const string &filename) {
        ofstream file(filename);
        if (!file.is_open()) {
            cerr << "Failed to open file: " << filename << endl;
            return;
        }

        unordered_set<long long> visited;
        unordered_set<long long> acceptingStates;

        // Collect all states and accepting states
        for (const auto &regexNFA: regexNFAs) {
            collectStates(*regexNFA.second.getStart(), visited, acceptingStates);
        }

        // Write all states
        file << "States: ";
        for (const auto &state: visited) {
            file << state << " ";
        }
        file << endl;

        // Write accepting states
        file << "Accepting States: ";
        for (const auto &state: acceptingStates) {
            file << state << " ";
        }
        file << endl;

        // Write transitions
        visited.clear();
        writePaths(regexNFAs, visited, file);

        file.close();
    }

    void collectStates(stateNFA &nfa, unordered_set<long long> &visited, unordered_set<long long> &acceptingStates) {
        if (visited.find(nfa.getID()) != visited.end()) return;
        visited.insert(nfa.getID());

        if (nfa.isFinalState()) {
            acceptingStates.insert(nfa.getID());
        }

        for (const auto &transition: nfa.getTransitions()) {
            for (const auto &state: transition.second) {
                collectStates(*state, visited, acceptingStates);
            }
        }
    }
};
