#ifndef PARSER_H
#define PARSER_H

#include "PredictiveParsingTable.h"
#include <bits/stdc++.h>

using namespace std;

class Parser {
    vector<string>& nonTerminals;
    PredictiveParsingTable& parsingTable;
    const string START_SYMBOL = "METHOD_BODY";  // Replace with your grammar's start symbol
    const string END_SYMBOL = "$";

    void printStack(const stack<string>& s);

public:
    Parser(PredictiveParsingTable& table, vector<string>& nonTerminals);
    bool parse(const vector<pair<string, string>> &tokens);

private:
    bool isTerminal(const string &symbol);
};

vector<pair<string, string>> readTokens(const string &filename);

#endif // PARSER_H