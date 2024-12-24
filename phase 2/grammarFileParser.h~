#ifndef GRAMMARFILEPARSER_H
#define GRAMMARFILEPARSER_H

#include <unordered_map>
#include <vector>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <algorithm>

class grammarFileParser {
public:
    std::unordered_map<std::string, std::vector<std::string>> grammar;
    std::vector<std::string> nonTerminals;

    grammarFileParser();

    static std::string trim(const std::string &s);
    static std::vector<std::string> splitByDelimiter(const std::string &str, char delimiter);
    void addRule(const std::string &nonTerminal, std::vector<std::string> productions);
    void parseGrammarRule(const std::string &rule);
    void readFile(const std::string &grammarFile);
    void addProduction(const std::string &nonTerminal, const std::string &production);
    void printAll() const;
    std::unordered_map<std::string, std::vector<std::string>> getGrammar() const;
    std::vector<std::string> getNonTerminals() const;
};

#endif // GRAMMARFILEPARSER_H