cmake_minimum_required(VERSION 3.27)
project(complierGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(complierGenerator main.cpp
        regexFileReader.cpp
        postfixRegexConvertor.cpp
        substitutions.cpp
        NFAcreation.cpp
        constants.h
        DFACreator.cpp
        DFACreator.h
)
