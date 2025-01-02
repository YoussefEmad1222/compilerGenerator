#pragma once

#include "PredictiveParsingTable.cpp"
#include <stack>
#include <vector>
#include <iostream>
#include <unordered_map>
#include <set>
#include <string>

using namespace std;

class Parser {
private:
    vector<string>& nonTerminals;
    PredictiveParsingTable& parsingTable;
    const string START_SYMBOL = "METHOD_BODY";  // Replace with your grammar's start symbol
    const string END_SYMBOL = "$";

    
    void printStack(const stack<string>& s) {
        stack<string> temp = s;  // Create a copy to avoid modifying the original stack
        while (!temp.empty()) {
            cout << temp.top() << " ";
            temp.pop();
        }
        cout << "\n";
    }


public:
    Parser(PredictiveParsingTable& table, vector<string>& nonTerminals) : parsingTable(table), nonTerminals(nonTerminals) {}

    bool parse(const vector<pair<string, string>> &tokens) {
        stack<string> parseStack;
        parseStack.push(END_SYMBOL);
        parseStack.push(START_SYMBOL);

        size_t tokenIndex = 0;

        while (!parseStack.empty()) {
            printStack(parseStack);

            string top = parseStack.top();
            parseStack.pop();
            if (tokenIndex >= tokens.size()) {
                cerr << "Unexpected end of input. Parsing failed.\n";
                return false;
            }

            string currentToken = tokens[tokenIndex].second;


            if (top == END_SYMBOL) {
                if (currentToken == END_SYMBOL) {
                    cout << "Parsing successful!\n";
                    return true;
                } else {
                    cerr << "Expected end of input but found: " << currentToken << "\n";
                    return false;
                }
            } else if (isTerminal(top)) {
                if (top == currentToken) {
                    tokenIndex++;
                } else {
                    cerr << "Syntax error: expected " << top << " but found " << currentToken << "\n";
                    return false;
                }
            } else {
                string production = parsingTable.parsingTable[top][currentToken];

                if (production.empty()) {
                    cerr << "Syntax error: no rule for " << top << " with token " << currentToken << "\n";
                    return false;
                }

                if (production != "#") {
                    vector<string> productionTokens = FirstFollowCalculator::splitByDelimiter(production, ' ');
                    for (auto it = productionTokens.rbegin(); it != productionTokens.rend(); ++it) {
                        parseStack.push(*it);
                    }
                }
            }
        }

        cerr << "Unexpected end of parsing stack.\n";
        return false;
    }

private:
    bool isTerminal(const string &symbol) {
        return find(nonTerminals.begin(), nonTerminals.end(), symbol) == nonTerminals.end();
    }
};



vector<pair<string, string>> readTokens(const string &filename) {
    vector<pair<string, string>> tokens;
    ifstream file(filename);
    if (!file.is_open()) {
        cerr << "Error: Unable to open file " << filename << endl;
        return tokens;
    }

    string line;
    while (getline(file, line)) {
        istringstream iss(line);
        string lexeme, type;
        if (iss >> lexeme >> type) {
            type = "'" + type + "'";
            tokens.emplace_back(lexeme, type);
        } else {
            cerr << "Error: Malformed line in token file: " << line << endl;
        }
    }

    file.close();
    return tokens;
}