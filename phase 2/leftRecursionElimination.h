#ifndef LEFTRECURSIONELIMINATOR_H
#define LEFTRECURSIONELIMINATOR_H

#include "grammarFileParser.h"
#include "../constants.h"
#include <vector>
#include <string>
#include <unordered_map>

using namespace std;

class LeftRecursionEliminator {
public:
    grammarFileParser *gfp;

    LeftRecursionEliminator();

    vector<int> findAllOccurrences(string str, string sub);

    void eliminateNonImmediateLeftRecursion(int x, int y);

    void eliminateImmediateLeftRecursion(int i, unordered_map<string, vector<string>> &newGrammar,
                                         vector<string> &newNonTerminals);

    void eliminateLeftRecursion(const string &filePath);

    grammarFileParser getGrammarFileParser();
};

#endif // LEFTRECURSIONELIMINATOR_H