cmake_minimum_required(VERSION 3.16)
project(VttParser)

set(CMAKE_CXX_STANDARD 20)

#Boost
FIND_PACKAGE( Boost REQUIRED )
INCLUDE_DIRECTORIES( ${Boost_INCLUDE_DIR} )

add_library(VttParser
        SubtitleItem.cpp
        SubtitleItem.h
        VttParser.cpp
        VttParser.h)

# Build Test Exe
add_executable(VttTest main.cpp SubtitleItem.cpp SubtitleItem.h VttParser.cpp VttParser.h main.cpp)

target_link_libraries(VttParser ${Boost_LIBRARIES})
