#pragma once

#include <bits/stdc++.h>
#include <iostream>
#include "substitutions.cpp"
#include "postfixRegexConvertor.cpp"

using namespace std;

class RegularFileReader {
public:
    bool isOperator(char c) {
        return c == UNION_OPERATOR || c == KLEENE_STAR_OPERATOR || c == PLUS_OPERATOR || c == CONCATENATION_OPERATOR ||
               c == LEFT_PARENTHESIS || c == RIGHT_PARENTHESIS;
    }

    unordered_map<string, string> expressions;
    vector<string> orderedExpressions;
    unordered_map<string, string> definitions;
    vector<string> orderedDefinition;
    vector<string> keywords;
    vector<string> punctuations;

    RegularFileReader() {
        expressions.clear();
        orderedExpressions.clear();
        definitions.clear();
        orderedDefinition.clear();
        keywords.clear();
        punctuations.clear();
    }


    string trim(const string &s) {
        size_t startPos = s.find_first_not_of(" \t\n\r\f\v");
        size_t endPos = s.find_last_not_of(" \t\n\r\f\v");
        return (startPos == string::npos || endPos == string::npos) ? "" : s.substr(startPos, endPos - startPos + 1);
    }


    void splitBySpace(const string &line, vector<string> &tokens) {
        stringstream ss(line);
        string word;
        while (ss >> word) {
            word = trim(word);
            tokens.push_back(word);
        }
    }

    string expandRangeOperation(string regex) {
        string expanded;
        int length = regex.length();

        auto isRange = [](char start, char end) {
            return start <= end;
        };

        auto expandRange = [](char start, char end) {
            string result = "(";
            for (char c = start; c < end; ++c) {
                result.push_back(c);
                result.push_back(UNION_OPERATOR);
            }
            result.push_back(end);
            result.push_back(RIGHT_PARENTHESIS);
            return result;
        };

        for (int i = 0; i < length; ++i) {
            if (isalnum(regex[i]) && i + 1 < length && regex[i + 1] == '-') {
                if (i + 2 < length && isalnum(regex[i + 2])) {
                    char startChar = regex[i];
                    char endChar = regex[i + 2];
                    if (isRange(startChar, endChar)) {
                        expanded += expandRange(startChar, endChar);
                    } else {
                        expanded += startChar;
                        expanded += '-';
                        expanded += endChar;
                    }
                    i += 2; // Skip the end character as it's already processed
                } else {
                    expanded += regex[i];
                    expanded += '-';
                    ++i;
                }
            } else {
                if (regex[i] == ' ') {
                    continue;
                }
                expanded += regex[i];
            }
        }
        return expanded;
    }

    vector<string> getNameAndValue(const string &line, const string &delimiter) {
        size_t pos = line.find(delimiter);
        if (pos == string::npos) return {};
        string name = line.substr(0, pos);
        string value = line.substr(pos + delimiter.length());
        value = trim(value);
        value = expandRangeOperation(value); // Expand range operation
        return {trim(name), value};
    }

    void parseLine(const string &line) {
        string trimmedLine = trim(line);
        if (trimmedLine.find(':') != string::npos) {
            vector<string> nameAndValue = getNameAndValue(trimmedLine, ":");
            expressions[nameAndValue[0]] = nameAndValue[1];
            orderedExpressions.push_back(nameAndValue[0]);
        } else if (trimmedLine.find('=') != string::npos) {
            vector<string> nameAndValue = getNameAndValue(trimmedLine, "=");
            definitions[nameAndValue[0]] = nameAndValue[1];
            orderedDefinition.push_back(nameAndValue[0]);
        } else if (trimmedLine.front() == '{') {
            string substr = trimmedLine.substr(1, trimmedLine.size() - 2);
            splitBySpace(substr, keywords);
        } else if (trimmedLine.front() == '[') {
            string substr = trimmedLine.substr(1, trimmedLine.size() - 2);
            splitBySpace(substr, punctuations);
        } else {
            cerr << "Error: Invalid line format -> " << line << endl;
        }
    }


    void infixToPostfix() {
        postfixRegexConvertor convertor;
        for (int i = 0; i < orderedExpressions.size(); i++) {
            expressions[orderedExpressions[i]] = convertor.infixToPostfix(expressions[orderedExpressions[i]]);
        }
        for (int i = 0; i < keywords.size(); i++) {
            keywords[i] = convertor.infixToPostfix(keywords[i]);
        }
        for (int i = 0; i < punctuations.size(); i++) {
            punctuations[i] = convertor.infixToPostfix(punctuations[i]);
        }
    }

    bool isConcatNeeded(char currentChar, char nextChar, char prevChar) {
        if (nextChar == '\0') return false;
        bool isCurrent = !isOperator(currentChar) || currentChar == RIGHT_PARENTHESIS || currentChar ==
                         KLEENE_STAR_OPERATOR || currentChar == PLUS_OPERATOR;
        isCurrent = isCurrent && currentChar != ESCAPE_CHARACTER;
        bool isNextOperand = !isOperator(nextChar) || nextChar == LEFT_PARENTHESIS || nextChar == ESCAPE_CHARACTER;
        if (prevChar == ESCAPE_CHARACTER) {
            isCurrent = true;
        }

        return isCurrent && isNextOperand;
    }

    string concatRegex(string expression) {
        string newExpression;
        size_t length = expression.size();

        for (int i = 0; i < length; ++i) {
            char currentChar = expression[i];
            char nextChar = (i + 1 < length) ? expression[i + 1] : '\0';
            char prevChar = (i - 1 >= 0) ? expression[i - 1] : '\0';

            newExpression.push_back(currentChar);

            if (isConcatNeeded(currentChar, nextChar, prevChar)) {
                newExpression.push_back(CONCATENATION_OPERATOR);
            }
        }
        return newExpression;
    }

    void addConcat() {
        for (const auto &expressionName: orderedExpressions) {
            expressions[expressionName] = concatRegex(expressions[expressionName]);
        }
        for (int i = 0; i < keywords.size(); i++) {
            keywords[i] = concatRegex(keywords[i]);
        }
        for (int i = 0; i < punctuations.size(); i++) {
            punctuations[i] = concatRegex(punctuations[i]);
        }
    }



    void readLexicalRules(const string &filename) {
        ifstream file(filename);
        if (!file.is_open()) {
            cerr << "Error: Could not open file " << filename << endl;
            return;
        }

        string line;
        while (getline(file, line)) {
            if (line.empty() || line[0] == '#') continue;
            parseLine(line);
        }
        file.close();
        substitute();
        addConcat();
        infixToPostfix();
    }

    void substitute() {
        regexSubstitutions substitutions;
        substitutions.insert(orderedDefinition[0]);
        for (int i = 1; i < orderedDefinition.size(); i++) {
            string expanded = substitutions.replace(definitions[orderedDefinition[i]], definitions);
            definitions[orderedDefinition[i]] = expanded;
            substitutions.insert(orderedDefinition[i]);
        }
        for (int i = 0; i < orderedExpressions.size(); i++) {
            string expanded = substitutions.replace(expressions[orderedExpressions[i]], definitions);
            expressions[orderedExpressions[i]] = expanded;
            substitutions.insert(orderedExpressions[i]);
        }
    }

    void printAll() {
        cout << "Expressions: " << endl;
        for (const auto &expression: orderedExpressions) {
            cout << expression << " : " << expressions[expression] << endl;
        }
        cout << endl;
        cout << "Definitions: " << endl;
        for (const auto &definition: orderedDefinition) {
            cout << definition << " : " << definitions[definition] << endl;
        }
        cout << endl;
        cout << "Keywords: " << endl;
        for (const auto &keyword: keywords) {
            cout << keyword << " ";
        }
        cout << endl;
        cout << "Punctuations: " << endl;
        for (const auto &punctuation: punctuations) {
            cout << punctuation << " ";
        }
    }
};
