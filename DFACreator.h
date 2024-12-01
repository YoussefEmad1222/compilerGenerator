//
// Created by Mariam on 11/29/2024.
//

#ifndef DFACREATION_H
#define DFACREATION_H

#include <bits/stdc++.h>

#include "DFA.h"
#include "NFA.h"

#define ll long long

using namespace std;

class DFACreator {
public:
    explicit DFACreator(const NFA nfa);
    void writeAllStatesToFile(const string &filename);
    void createDFA();
    DFA getDFA();

private:
    ll nStart;
    unordered_map<ll, pair<int, string>> nAccept;
    map<pair<ll, char>, set<ll>> nTransitions;
    unordered_set<char> inputs;
    DFA dfa;

    set<ll> eps_closure(const set<ll> &states);
    set<ll> move(const set<ll>& states, const char& input);
};



#endif //DFACREATION_H
