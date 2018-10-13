cmake_minimum_required(VERSION 3.4)
project(TigreFramework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(PostgreSQL_SOURCE_FILES
    PostgreSQL.h
    PostgreSQL.cpp
    PostgreSQLServiceProvider.cpp
)

add_library(tgPostgreSQLConnector ${PostgreSQL_SOURCE_FILES})

if(UNIX AND NOT APPLE)
    set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/include/postgresql/")
    set(PostgreSQL_ADDITIONAL_VERSIONS "10.1")
else()
    set(PostgreSQL_INCLUDE_DIR "/usr/local/opt/libpq/include/")
    set(PostgreSQL_LIBRARY_DIR "/usr/local/opt/pgsql/lib/")
    set(PostgreSQL_TYPE_INCLUDE_DIR "/usr/local/opt/pgsql/include/server/catalog/")
endif()

FIND_PACKAGE(PostgreSQL REQUIRED)
include_directories(${PostgreSQL_INCLUDE_DIR})
target_link_libraries(tgPostgreSQLConnector ${PostgreSQL_LIBRARIES})
