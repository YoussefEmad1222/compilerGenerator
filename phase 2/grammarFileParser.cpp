#include <bits/stdc++.h>

using namespace std;

class grammarFileParser {
public:
    unordered_map<string, vector<string>> grammar;
    vector<string> nonTerminals;

    grammarFileParser() = default;

    static string trim(const string &s) {
        string production = s;
        production.erase(remove(production.begin(), production.end(), '\n'), production.end());
        production.erase(remove(production.begin(), production.end(), '\t'), production.end());
        production.erase(remove(production.begin(), production.end(), '\r'), production.end());

        size_t startPos = production.find_first_not_of(" ");
        size_t endPos = production.find_last_not_of(" ");
        return (startPos == string::npos || endPos == string::npos) ? "" : production.substr(startPos, endPos - startPos + 1);
    }

    static vector<string> splitByDelimiter(const string &str, char delimiter) {
        vector<string> tokens;
        stringstream ss(str);
        string token;
        while (getline(ss, token, delimiter)) {
            tokens.push_back(token);
        }
        return tokens;
    }

    void addRule(const string &nonTerminal, vector<string> productions) {
        nonTerminals.push_back(nonTerminal);
        for (auto &production : productions) {
            production = trim(production);
        }
        grammar[nonTerminal] = move(productions);
    }

    void parseGrammarRule(const string &rule) {
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

    void readFile(const string &grammarFile) {
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

    void addProduction(const string &nonTerminal, const string &production) {
        grammar[nonTerminal].push_back(production);
    }

    void printAll() const {
        for (const auto &nonTerminal : nonTerminals) {
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

    unordered_map<string, vector<string>> getGrammar() const {
        return grammar;
    }

    vector<string> getNonTerminals() const {
        return nonTerminals;
    }
};