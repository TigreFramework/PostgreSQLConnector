cmake_minimum_required(VERSION 3.4)
project(TigreFramework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SYSTEM_FILES
    PostgreSQL.h
    PostgreSQL.cpp
)

set(SOURCE_FILES ${SYSTEM_FILES})

add_library(tgPostgreSQLConnector ${SYSTEM_FILES})