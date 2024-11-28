#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class postfixRegexConvertor {
    bool isOperator(char c) {
        return c == '|' || c == '*' || c == '+' || c == '?' || c =='(' || c == ')';
    }

    int precedence(char op) {
        if (op == '*' || op == '+') return 3;
        if (op == '?') return 2;//concatenation
        if (op == '|') return 1;
        return 0;
    }


public:
    string infixToPostfix(const string &reStr) {
        stack<char> stack;
        string output;

        for (int i = 0; i < reStr.size(); i++) {
            char c = reStr[i];
            if (c == '\\') {
                output += c;
                if (i + 1 < reStr.size()) {
                    output += reStr[++i];
                }
            } else if (isalnum(c) || c == '\0' ||!isOperator(c)) {
                output += c;
            } else if (c == '(') {
                stack.push(c);
            } else if (c == ')') {
                while (!stack.empty() && stack.top() != '(') {
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