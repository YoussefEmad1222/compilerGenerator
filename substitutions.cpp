#include <iostream>
#include <bits/stdc++.h>

using namespace std;

class TrieNode {
public:
    unordered_map<char, TrieNode *> children;
    bool isEnd;

    TrieNode() {
        isEnd = false;
    }
};

class regexSubstitutions {
    TrieNode *Root = new TrieNode();

public:

    void insert(string &def) {
        TrieNode *current = Root;
        for (char c: def) {
            if (current->children.find(c) == current->children.end()) {
                current->children[c] = new TrieNode();
            }
            current = current->children[c];
        }
        current->isEnd = true;
    }

    string longestPrefix(string &input, int start) {
        string longestPrefix = "";
        string prefix = "";
        TrieNode *current = Root;
        for (int i = start; i < input.length(); i++) {
            char c = input[i];
            if (current->children.find(c) == current->children.end()) {
                break;
            }
            prefix += c;
            current = current->children[c];
            if (current->isEnd) {
                longestPrefix = prefix;
            }
        }
        return longestPrefix;
    }

    string replace(string regex, unordered_map<string, string> &definitions) {
        string expandedRegex;
        int n = regex.length();
        int start = 0;
        while (start < n) {
            string longest = longestPrefix(regex, start);
            if (!longest.empty()) {
                expandedRegex += '(';
                expandedRegex += definitions[longest];
                expandedRegex += ')';
                start += longest.length();
            } else {
                expandedRegex += regex[start];
                start++;
            }
        }
        return expandedRegex;
    }


};