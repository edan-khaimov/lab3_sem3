cmake_minimum_required(VERSION 3.29)
project(lab3)

set(CMAKE_CXX_STANDARD 20)

set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)

find_package(Qt6 COMPONENTS Charts Core Gui Widgets REQUIRED)

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
        UI/headers/MainWindow.h
        UI/source/MainWindow.cpp
        UI/headers/SubsequenceWindow.h
        UI/source/SubsequenceWindow.cpp
        UI/headers/HistogramWindow.h
        UI/source/HistogramWindow.cpp
)

target_link_libraries(lab3 PRIVATE Qt6::Charts Qt6::Core Qt6::Gui Qt6::Widgets)