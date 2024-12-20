#ifndef LEXICALANALYZER
#define LEXICALANALYZER

#include "DFA.h"
#include <bits/stdc++.h>

class LexicalAnalyzer
{
private:
    DFA dfa;
    vector<string> symbolTable;
public:
    explicit LexicalAnalyzer(DFA DFA);
    void Analyze(const string &filePath, const char* outputFilePath);
    void FindFirstTokenInString(std::string string, int stringStart, int &tokenEnd, State currentState, std::string &tokenClass);
};

LexicalAnalyzer::LexicalAnalyzer(DFA DFA)
{
    dfa = DFA;
}



void LexicalAnalyzer::Analyze(const string &filePath, const char* outputFilePath)
{
    std::ifstream file(filePath, std::ios::in);

    if (!file.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::ofstream output(outputFilePath, std::ios::out);

    if (!output.is_open()) {
        std::cerr << "Error opening file!" << std::endl;
        return;
    }

    std::stringstream buffer;
    buffer << file.rdbuf();

    std::string fileContent = buffer.str();

    string tokenClass = "";
    int tokenEnd = 0;

    for(int stringStart = 0; stringStart < fileContent.length(); stringStart++){
        if(fileContent[stringStart] == '\n' || fileContent[stringStart] == '\t' || fileContent[stringStart] == ' ') continue;
        FindFirstTokenInString(fileContent, stringStart, tokenEnd, dfa.startState, tokenClass);
        if(tokenClass == ""){
            output << "Error couldn't find token" << endl;
            continue;
        }

        string lexeme = fileContent.substr(stringStart, tokenEnd - stringStart + 1);

        if(std::string("id").compare(tokenClass) == 0)
            symbolTable.push_back(lexeme);

        output << lexeme << " " << tokenClass << endl;
        stringStart = tokenEnd;
    }

    file.close();
}

void LexicalAnalyzer::FindFirstTokenInString(std::string string, int stringStart, int& tokenEnd, State currentState, std::string& tokenClass){
    tokenClass = "";
    for(int i = stringStart; i < string.length(); i++){

        State nextState;

        // move to the next state
        if(dfa.move(currentState, string[i], nextState))
            currentState = nextState;
        else
            // we stopped at a state. so we either found an accepting state before or no state was found and no transition available
            // accepting state -> tokenClass != ""
            // else no transition and do panic recovery
            return;

        // check if the current state is an accepting state
        unordered_map<State, std::string>::iterator it = dfa.acceptStates.find(currentState);

        // check if the current state is an accepting state 
        if(it != dfa.acceptStates.end()){
            tokenClass = it -> second; // cache the accepting state to access it in the future
            tokenEnd = i;
        }
    }
}

#endif //LEXICALANALYZER