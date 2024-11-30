//
// Created by Mariam on 11/29/2024.
//

#ifndef DFACREATION_H
#define DFACREATION_H

#include <bits/stdc++.h>

#include "DFA.h"
#include "NFA.cpp"

#define ll long long

using namespace std;

class DFACreator {
public:
    DFACreator(long long &nStart, unordered_set<long long> &nAccept,
               map<pair<long long, char>, set<long long>> &nTransitions, set<char> &inputs);

    explicit DFACreator(NFA nfa);
    DFA getDFA() const;

private:
    ll& nStart;
    unordered_set<ll>& nAccept;
    map<pair<ll, char>, set<ll>>& nTransitions;
    set<char>& inputs;

    static set<ll> eps_closure(const set<ll> &states, map<pair<ll, char>, set<ll>> nTransitions);

    static set<ll> move(const set<ll>& states, const char& input, map<pair<ll, char>, set<ll>> nTransitions);
};



#endif //DFACREATION_H
