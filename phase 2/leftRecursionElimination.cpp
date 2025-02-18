#include "leftRecursionElimination.h"
#include <bits/stdc++.h>

using namespace std;

LeftRecursionEliminator::LeftRecursionEliminator() {
    gfp = new grammarFileParser();
}

vector<int> LeftRecursionEliminator::findAllOccurrences(string str, string sub) {
    vector<int> positions;
    size_t pos = str.find(sub, 0);
    while (pos != string::npos) {
        positions.push_back(pos);
        pos = str.find(sub, pos + 1);
    }
    return positions;
}

void LeftRecursionEliminator::eliminateNonImmediateLeftRecursion(int x, int y) {
    string nonTerminal1 = gfp->nonTerminals[x];
    string nonTerminal2 = gfp->nonTerminals[y];
    vector<string> productions1 = gfp->grammar[nonTerminal1];
    vector<string> productions2 = gfp->grammar[nonTerminal2];
    vector<string> newProductions2;

    for (const auto &production: productions2) {
        if (production.substr(0, nonTerminal1.size()) == nonTerminal1) {
            string right = production.substr(nonTerminal1.size());
            for (const auto &prod1: productions1) {
                newProductions2.push_back(prod1 + " " + right);
            }
        } else {
            newProductions2.push_back(production);
        }
    }

    gfp->grammar[nonTerminal2] = newProductions2;
}

void LeftRecursionEliminator::eliminateImmediateLeftRecursion(int i, unordered_map<string, vector<string>> &newGrammar,
                                                              vector<string> &newNonTerminals) {
    string nonTerminal = gfp->nonTerminals[i];
    string newNonTerminal = nonTerminal + "'";
    vector<string> productions = gfp->grammar[nonTerminal];
    vector<string> newProductions, newProductionsPrime, alphas, betas;

    for (const auto &production: productions) {
        if (production.substr(0, nonTerminal.size()) == nonTerminal) {
            alphas.push_back(production.substr(nonTerminal.size()));
        } else {
            betas.push_back(production);
        }
    }

    if (alphas.empty()) {
        newGrammar[nonTerminal] = productions;
        newNonTerminals.push_back(nonTerminal);
        return;
    }
    newNonTerminals.push_back(nonTerminal);
    newNonTerminals.push_back(newNonTerminal);

    for (const auto &beta: betas) {
        if (beta == "\\L") {
            newProductions.push_back(newNonTerminal);
        } else {
            newProductions.push_back(beta + " " + newNonTerminal);
        }
    }

    for (const auto &alpha: alphas) {
        newProductionsPrime.push_back(alpha + " " + newNonTerminal);
    }
    newProductionsPrime.push_back(string(1, EPSILON));
    newGrammar[newNonTerminal] = newProductionsPrime;
    newGrammar[nonTerminal] = newProductions;
}

void LeftRecursionEliminator::eliminateLeftRecursion(const string &filePath) {
    gfp->readFile(filePath);
    unordered_map<string, vector<string>> newGrammar;
    vector<string> newNonTerminals;
    for (int i = 0; i < gfp->nonTerminals.size(); i++) {
        for (int j = i + 1; j < gfp->nonTerminals.size(); j++) {
            eliminateNonImmediateLeftRecursion(i, j);
        }
        eliminateImmediateLeftRecursion(i, newGrammar, newNonTerminals);
    }
    gfp->grammar = newGrammar;
    gfp->nonTerminals = newNonTerminals;
}

grammarFileParser LeftRecursionEliminator::getGrammarFileParser() {
    return *gfp;
}