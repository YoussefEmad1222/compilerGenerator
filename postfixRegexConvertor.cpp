#include <iostream>
#include <bits/stdc++.h>
#include "constants.h"

using namespace std;

class postfixRegexConvertor {
    bool isOperator(char c) {
        return c == UNION_OPERATOR || c == KLEENE_STAR_OPERATOR || c == PLUS_OPERATOR || c == CONCATENATION_OPERATOR ||
               c == LEFT_PARENTHESIS || c == RIGHT_PARENTHESIS;
    }

    int precedence(char op) {
        if (op == KLEENE_STAR_OPERATOR || op == PLUS_OPERATOR) return 3;
        if (op == CONCATENATION_OPERATOR) return 2;//concatenation
        if (op == UNION_OPERATOR) return 1;
        return 0;
    }


public:
    string infixToPostfix(const string &reStr) {
        stack<char> stack;
        string output;

        for (int i = 0; i < reStr.size(); i++) {
            char c = reStr[i];
            if (c == ESCAPE_CHARACTER) {
                output += c;
                if (i + 1 < reStr.size()) {
                    output += reStr[++i];
                }
            } else if (isalnum(c) || c == EPSILON || !isOperator(c)) {
                output += c;
            } else if (c == LEFT_PARENTHESIS) {
                stack.push(c);
            } else if (c == RIGHT_PARENTHESIS) {
                while (!stack.empty() && stack.top() != LEFT_PARENTHESIS) {
                    output += stack.top();
                    stack.pop();
                }
                if (!stack.empty()) stack.pop();
            } else {
                while (!stack.empty() && precedence(stack.top()) >= precedence(c)) {
                    output += stack.top();
                    stack.pop();
                }
                stack.push(c);
            }
        }
        while (!stack.empty()) {
            output += stack.top();
            stack.pop();
        }
        return output;
    }
};