#include <iostream>
#include <fstream>
#include <sstream>
#include <bits/stdc++.h>
#include "regexFileReader.cpp"
#include "NFAcreation.cpp"

#define EPSILON '\0'

using namespace std;
namespace fs = filesystem;

int main() {
    RegularFileReader regexLoader;
    regexLoader.readLexicalRules("../input/regex_rules.txt");
    regexLoader.printAll();
    nfaCreation nfaCreator;
    nfaCreator.createRegexNFAs(regexLoader.expressions, regexLoader.orderedExpressions);
    nfaCreator.printAllStates();
    return 0;

}
