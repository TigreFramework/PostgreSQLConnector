cmake_minimum_required(VERSION 3.4)
project(TigreFramework)

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11")

set(SYSTEM_FILES
    PostgreSQL.h
    PostgreSQL.cpp
        PostgreSQLServiceProvider.cpp PostgreSQLServiceProvider.h PostgreSQLConfiguration.cpp PostgreSQLConfiguration.h PostgreSQLDataObject.cpp PostgreSQLDataObject.h Exceptions/PostgreSQLException.cpp Exceptions/PostgreSQLException.h PostgreSQLStatement.cpp PostgreSQLStatement.h)

set(PostgreSQL_TYPE_INCLUDE_DIR /usr/include/postgresql/)

find_package(PostgreSQL REQUIRED)

set(SOURCE_FILES ${SYSTEM_FILES})

add_library(tgPostgreSQLConnector ${SYSTEM_FILES})
target_include_directories(tgPostgreSQLConnector PRIVATE ${PostgreSQL_INCLUDE_DIRS})
target_link_libraries(tgPostgreSQLConnector ${PostgreSQL_LIBRARIES} tgTypes tgDatabase)