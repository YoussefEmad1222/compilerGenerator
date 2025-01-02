#ifndef PREDICTIVEPARSINGTABLE_H
#define PREDICTIVEPARSINGTABLE_H
#include<bits/stdc++.h>

using namespace std;

class PredictiveParsingTable {
    unordered_map<string, set<string> > firstSets;
    unordered_map<string, set<string> > followSets;
    unordered_map<string, vector<string> > grammar;
    vector<string> nonTerminals;
    bool tableComputed;

public:
    unordered_map<string, unordered_map<string, string> > parsingTable;

    PredictiveParsingTable(
        unordered_map<string, set<string> > firstSets,
        unordered_map<string, set<string> > followSets,
        unordered_map<string, vector<string> > grammar,
        vector<string> nonTerminals
    );

    void computeTable();

    void updateParsingTable(const string &A, const vector<string> &alpha);

    set<string> computeFirstOfSequence(const vector<string> &sequence);

    string joinProduction(const vector<string> &production);

    unordered_map<string, unordered_map<string, string> > getTable();

    void printTable();
};

#endif // PREDICTIVEPARSINGTABLE_H
