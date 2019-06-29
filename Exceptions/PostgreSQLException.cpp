//
// Created by pedrosoares on 6/23/19.
//

#include "PostgreSQLException.h"

PostgreSQLException::PostgreSQLException() : Exception() {}

PostgreSQLException::PostgreSQLException(std::string message) : Exception(std::move(message)) {}
