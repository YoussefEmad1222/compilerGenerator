#ifndef LEFTFACTORING_H
#define LEFTFACTORING_H

#include "grammarFileParser.h"
#include <bits/stdc++.h>

using namespace std;

class leftFactoring {
public:
    grammarFileParser *gfp;

    explicit leftFactoring(grammarFileParser *gfp);

    vector<string> getSuffixes(const vector<string> &productions, int common_prefix_length, int i,
                               int common_prefix_index);

    void leftFactor();

    string chooseNextPrime(string &nonTerminal);

    vector<string> createProductionPrime(vector<string> suffixes);

private:
    void performLeftFactoring(string nonTerminal, vector<string> &newNonTerminals);

    pair<int, int> getCommonPrefixes(vector<string> productions, vector<string> production, int i);

    string concatenateCommonPrefix(vector<string> production, int commonPrefixLength);
};

#endif // LEFTFACTORING_H
