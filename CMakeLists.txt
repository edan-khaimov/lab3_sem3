cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

add_executable(lab3 main.cpp
        headers/Concepts.h
        headers/TypeTraits.h
        headers/MurmurHash.h
        headers/FNV1aHash.h
        headers/IDictionary.h
        headers/MostFrequentSubsequences.h
        source/MostFrequentSubsequences.cpp
        headers/Histogram.h
        headers/SortedSequence.h
)
