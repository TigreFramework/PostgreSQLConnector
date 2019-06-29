#include "PostgreSQLException.h"

PostgreSQLException::PostgreSQLException() : Exception() {}

PostgreSQLException::PostgreSQLException(std::string message) : Exception(std::move(message)) {}
