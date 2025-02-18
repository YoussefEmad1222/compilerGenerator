#include "leftFactoring.h"
#include "../constants.h"

leftFactoring::leftFactoring(grammarFileParser *gfp) {
    this->gfp = gfp;
}


vector<string> leftFactoring::getSuffixes(const vector<string> &productions, int common_prefix_length, int i,
                                          int common_prefix_index) {
    vector<string> suffixes;
    for (int j = i; j <= common_prefix_index; j++) {
        suffixes.push_back(productions[j].substr(common_prefix_length));
    }
    return suffixes;
}

void leftFactoring::leftFactor() {
    vector<string> newNonTerminals;
    for (string nonTerminal: gfp->nonTerminals) {
        newNonTerminals.push_back(nonTerminal);
        performLeftFactoring(nonTerminal, newNonTerminals);
    }
    gfp->nonTerminals = newNonTerminals;
}

string leftFactoring::chooseNextPrime(string &nonTerminal) {
    string primeChar;
    if (nonTerminal[nonTerminal.size() - 1] == '*') {
        primeChar = "~";
    } else if (nonTerminal[nonTerminal.size() - 1] == '~') {
        primeChar = "^";
    } else {
        primeChar = "*";
    }
    return primeChar;
}

vector<string> leftFactoring::createProductionPrime(vector<string> suffixes) {
    vector<string> productionsPrime;
    for (string suffix: suffixes) {
        if (suffix.empty()) {
            productionsPrime.push_back(string(1, EPSILON));
        } else {
            suffix = gfp->trim(suffix);
            productionsPrime.push_back(suffix);
        }
    }
    return productionsPrime;
}

void leftFactoring::performLeftFactoring(string nonTerminal, vector<string> &newNonTerminals) {
    vector<string> productions = gfp->grammar[nonTerminal];
    string primeChar = chooseNextPrime(nonTerminal);
    string prime = primeChar;
    sort(productions.begin(), productions.end());
    vector<string> newProductions;
    for (int i = 0; i < productions.size(); i++) {
        vector<string> production = gfp->splitByDelimiter(productions[i], ' ');
        pair<int, int> commonPrefixDate = getCommonPrefixes(productions, production, i);
        int commonPrefixIndex = commonPrefixDate.first;
        int commonPrefixLength = commonPrefixDate.second;
        if (commonPrefixLength == MAX_INT) {
            newProductions.push_back(productions[i]);
            continue;
        }
        string concatenatedCommonPrefix = concatenateCommonPrefix(production, commonPrefixLength);
        string newNonTerminal = nonTerminal + prime;
        prime += primeChar;
        vector<string> suffixes = getSuffixes(productions, concatenatedCommonPrefix.size(), i, commonPrefixIndex);
        newProductions.push_back(concatenatedCommonPrefix + " " + newNonTerminal);
        vector<string> productionsPrime = createProductionPrime(suffixes);
        gfp->grammar[newNonTerminal] = productionsPrime;
        newNonTerminals.push_back(newNonTerminal);
        i = commonPrefixIndex;
        performLeftFactoring(newNonTerminal, newNonTerminals);
    }
    gfp->grammar[nonTerminal] = newProductions;
}

pair<int, int> leftFactoring::getCommonPrefixes(vector<string> productions, vector<string> production, int i) {
    int commonPrefixLength = MAX_INT; // the common prefix elements  in the array of strings
    int commonPrefixIndex = -1;
    for (int j = i + 1; j < productions.size(); j++) {
        int k = 0;
        vector<string> nextProduction = gfp->splitByDelimiter(productions[j], ' ');
        while (k < production.size() && k < nextProduction.size() && production[k] == nextProduction[k]) {
            k++;
        }
        if (k == 0) {
            break;
        }
        commonPrefixLength = min(commonPrefixLength, k);
        commonPrefixIndex = j;
    }
    return make_pair(commonPrefixIndex, commonPrefixLength);
}

string leftFactoring::concatenateCommonPrefix(vector<string> production, int commonPrefixLength) {
    vector commonPrefix(production.begin(), production.begin() + commonPrefixLength);
    string concatenatedCommonPrefix = "";
    for (int k = 0; k < commonPrefix.size() - 1; k++) {
        concatenatedCommonPrefix += commonPrefix[k] + " ";
    }
    concatenatedCommonPrefix += commonPrefix[commonPrefix.size() - 1];
    return concatenatedCommonPrefix;
}
