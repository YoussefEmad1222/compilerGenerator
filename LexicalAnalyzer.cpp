#ifndef LEXICALANALYZER
#define LEXICALANALYZER

#include <cstring>
#include "DFA.h"


std::string circularArrayToString(const char* arr, int start, int end, int size) {
    // Calculate the total length of the substring
    int length = (end >= start) ? (end - start + 1) : (size - start + end + 1);

    // Allocate a buffer to hold the result
    char* buffer = new char[length];

    // Copy the first chunk (from start to the end of the array, if needed)
    int firstChunkSize = std::min(size - start, length);
    std::memcpy(buffer, arr + start, firstChunkSize);

    // Copy the second chunk (from the beginning of the array, if needed)
    if (length > firstChunkSize) {
        std::memcpy(buffer + firstChunkSize, arr, length - firstChunkSize);
    }

    // Construct the resulting string
    std::string result(buffer, length);

    // Free the allocated memory
    delete[] buffer;

    return result;
}

bool insertCharArray(char* circularArray, int& end, int size, int capacity, const char* input, int inputSize) {
    // Check if there is enough space in the circular array
    if (size + inputSize > capacity) {
        return false; // Not enough space
    }

    // Calculate the number of elements to copy in two chunks (if wrapping occurs)
    int firstChunkSize = std::min(inputSize, capacity - end - 1);
    std::memcpy(circularArray + end + 1, input, firstChunkSize);

    // If wrapping occurs, copy the remaining elements to the beginning of the array
    if (inputSize > firstChunkSize) {
        std::memcpy(circularArray, input + firstChunkSize, inputSize - firstChunkSize);
    }

    // Update tail pointer and size
    end = (end + inputSize) % capacity;
    return true; // Successfully inserted
}


#define MAXTOKENSIZE 100

class LexicalAnalyzer
{
private:
    DFA dfa;
    vector<string> symbolTable;
public:
    explicit LexicalAnalyzer(DFA DFA);
    void Analyze(const string &filePath);
    bool FindFirstTokenInString(std::string string, int stringStart, int &tokenEnd, State currentState, std::string &tokenClass);
};

LexicalAnalyzer::LexicalAnalyzer(DFA DFA)
{
    dfa = DFA;
}

void LexicalAnalyzer::Analyze(const string &filePath)
{
    std::ifstream file(filePath, std::ios::in);

    if (!file.is_open()) {
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
        if(FindFirstTokenInString(fileContent, stringStart, tokenEnd, dfa.startState, tokenClass))
        {
            if(tokenClass == "")
                continue;

            string lexeme = fileContent.substr(stringStart, tokenEnd - stringStart + 1);

            if(std::string("id").compare(tokenClass) == 0)
                symbolTable.push_back(lexeme);

            cout << lexeme << " " << tokenClass << endl;
            stringStart = tokenEnd + 1;
            tokenClass = "";
        }
    }

    file.close();
}

bool LexicalAnalyzer::FindFirstTokenInString(std::string string, int stringStart, int& tokenEnd, State currentState, std::string& tokenClass){
    for(int i = stringStart; i < string.length(); i++){

        State nextState;

        // move to the next state
        if(dfa.move(currentState, string[i], nextState))
            currentState = nextState;
        else
            // true means we stopped at a state. so we either found an accepting state before or no state was found and no transition available
            // accepting state -> tokenClass != ""
            // else no transition and do panic recovery
            return true;

        // check if the current state is an accepting state
        unordered_map<State, std::string>::iterator it = dfa.acceptStates.find(currentState);

        // check if the current state is an accepting state 
        if(it != dfa.acceptStates.end()){
            tokenClass = it -> second; // cache the accepting state to access it in the future
            tokenEnd = i;
        }
        
    }
    // false means we couldn't find anything in the given string
    return false;
}

#endif //LEXICALANALYZER