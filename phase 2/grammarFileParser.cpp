#include "grammarFileParser.h"
#include <bits/stdc++.h>

using namespace std;

grammarFileParser::grammarFileParser() = default;

string grammarFileParser::trim(const string &s) {
    string production = s;
    replace(production.begin(), production.end(), '\n', ' ');
    replace(production.begin(), production.end(), '\t', ' ');
    replace(production.begin(), production.end(), '\r', ' ');
    size_t startPos = production.find_first_not_of(" ");
    size_t endPos = production.find_last_not_of(" ");
    return (startPos == string::npos || endPos == string::npos)
               ? ""
               : production.substr(startPos, endPos - startPos + 1);
}

vector<string> grammarFileParser::splitByDelimiter(const string &str, char delimiter) {
    vector<string> tokens;
    stringstream ss(str);
    string token;
    while (getline(ss, token, delimiter)) {
        tokens.push_back(token);
    }
    return tokens;
}

void grammarFileParser::addRule(const string &nonTerminal, vector<string> productions) {
    nonTerminals.push_back(nonTerminal);
    for (auto &production: productions) {
        production = trim(production);
    }
    grammar[nonTerminal] = move(productions);
}

void grammarFileParser::parseGrammarRule(const string &rule) {
    size_t pos = rule.find("::=");
    if (pos == string::npos) {
        cerr << "Error: Invalid rule format -> " << rule << endl;
        return;
    }
    string nonTerminal = trim(rule.substr(0, pos));
    string production = rule.substr(pos + 3);
    vector<string> productions = splitByDelimiter(production, '|');
    addRule(nonTerminal, productions);
}

void grammarFileParser::readFile(const string &grammarFile) {
    ifstream file(grammarFile);
    if (!file.is_open()) {
        cerr << "Error: Could not open file " << grammarFile << endl;
        return;
    }

    string line;
    while (getline(file, line, '#')) {
        if (line.empty() || line[0] == '#') continue;
        parseGrammarRule(line);
    }
}

void grammarFileParser::addProduction(const string &nonTerminal, const string &production) {
    grammar[nonTerminal].push_back(production);
}

void grammarFileParser::printAll() const {
    for (const auto &nonTerminal: nonTerminals) {
        cout << nonTerminal << " ::= ";
        for (size_t i = 0; i < grammar.at(nonTerminal).size(); ++i) {
            if (grammar.at(nonTerminal)[i] == "#") {
                cout << "ε";
            } else {
                cout << grammar.at(nonTerminal)[i];
            }
            if (i < grammar.at(nonTerminal).size() - 1) {
                cout << " | ";
            }
        }
        cout << endl;
    }
}

unordered_map<string, vector<string> > grammarFileParser::getGrammar() const {
    return grammar;
}

vector<string> grammarFileParser::getNonTerminals() const {
    return nonTerminals;
}
