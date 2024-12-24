#include <bits/stdc++.h>
#include "grammarFileParser.h"
#include "../constants.h"
using namespace std;

class leftFactoring {
public:
    grammarFileParser *gfp;

    explicit leftFactoring(grammarFileParser *gfp) {
        this->gfp = gfp;
    }


    vector<string> getSuffixes(const vector<string> &productions, int common_prefix_length, int i,
                               int common_prefix_index) {
        vector<string> suffixes;
        for (int j = i; j <= common_prefix_index; j++) {
            suffixes.push_back(productions[j].substr(common_prefix_length));
        }
        return suffixes;
    }

    void leftFactor() {
        vector<string> newNonTerminals;
        for (string nonTerminal: gfp->nonTerminals) {
            newNonTerminals.push_back(nonTerminal);
            performLeftFactoring(nonTerminal, newNonTerminals);
        }
        gfp->nonTerminals = newNonTerminals;
    }

    void performLeftFactoring(string nonTerminal, vector<string> &newNonTerminals) {
        vector<string> productions = gfp->grammar[nonTerminal];
        string prime = "*";
        sort(productions.begin(), productions.end());
        vector<string> newProductions;
        for (int i = 0; i < productions.size(); i++) {
            vector<string> production = gfp->splitByDelimiter(productions[i], ' ');
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
            if (commonPrefixLength == MAX_INT) {
                newProductions.push_back(productions[i]);
                continue;
            }
            vector commonPrefix(production.begin(), production.begin() + commonPrefixLength);
            string concatenatedCommonPrefix = "";
            for (int k = 0; k < commonPrefix.size() - 1; k++) {
                concatenatedCommonPrefix += commonPrefix[k] + " ";
            }
            concatenatedCommonPrefix += commonPrefix[commonPrefix.size() - 1];
            string newNonTerminal = nonTerminal + prime;
            prime += "*";
            vector<string> suffixes = getSuffixes(productions, concatenatedCommonPrefix.size(), i, commonPrefixIndex);
            newProductions.push_back(concatenatedCommonPrefix + " " + newNonTerminal);
            vector<string> productionsPrime;
            for (string suffix: suffixes) {
                if (suffix.empty()) {
                    productionsPrime.push_back(string(1, EPSILON));
                } else {
                    productionsPrime.push_back(suffix);
                }
            }
            gfp->grammar[newNonTerminal] = productionsPrime;
            newNonTerminals.push_back(newNonTerminal);
            i = commonPrefixIndex;
        }
        gfp->grammar[nonTerminal] = newProductions;
    }
};
