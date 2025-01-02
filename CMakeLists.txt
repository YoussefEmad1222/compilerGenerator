cmake_minimum_required(VERSION 3.27)
project(complierGenerator)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_BUILD_TYPE Debug)

add_executable(complierGenerator main.cpp
        "phase 1/regexFileReader.cpp"
        "phase 1/postfixRegexConvertor.cpp"
        "phase 1/substitutions.cpp"
        "phase 1/NFAcreation.cpp"
        "constants.h"
        "phase 1/DFACreator.cpp"
        "phase 1/DFACreator.h"
        "phase 2/grammarFileParser.cpp"
        "phase 2/grammarFileParser.h"
        "phase 2/leftRecursionElimination.cpp"
        "phase 2/leftRecursionElimination.h"
        "phase 2/leftFactoring.cpp"
        "phase 2/firstFollowCalculator.cpp"
        "phase 2/predictiveParsingTable.cpp"
        "phase 2/predictiveParsingTable.h"
        "phase 2/parser.cpp"
        "phase 2/parser.h"
)