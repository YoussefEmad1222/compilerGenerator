cmake_minimum_required(VERSION 3.27)
project(complierGenerator)

set(CMAKE_CXX_STANDARD 17)

add_executable(complierGenerator main.cpp
        regexFileReader.cpp
        postfixRegexConvertor.cpp
        substitutions.cpp
        NFA.cpp
        NFAcreation.cpp
)
