cmake_minimum_required(VERSION 3.27)
project(complierGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(complierGenerator main.cpp
        "phase 1/regexFileReader.cpp"
        "phase 1/postfixRegexConvertor.cpp"
        "phase 1/substitutions.cpp"
        "phase 1/NFAcreation.cpp"
        constants.h
        "phase 1/DFACreator.cpp"
        "phase 1/DFACreator.h"
)
