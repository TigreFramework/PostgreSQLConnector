#include <libpq-fe.h>
#include <TigreFramework/PostgreSQLConnector/Exceptions/PostgreSQLException.h>
#include <TigreFramework/String/String.h>
#include <TigreFramework/Database/Line.h>
#include <TigreFramework/Types/Math.h>
#include "PostgreSQLStatement.h"

PostgreSQLStatement::PostgreSQLStatement(PGconn *connection, std::string sql) : DataObjectStatement() {
    this->connection = connection;
    this->sql = std::move(sql);
}

PostgreSQLStatement::~PostgreSQLStatement() {
    this->closeCursor();
}

bool PostgreSQLStatement::execute(const Param &params) {
    if(!this->bindValueByName.empty() || !this->bindValueByIndex.empty()){
        throw PostgreSQLException("Do not use execute bind and bindValue at the same time.");
    }
    this->sql = params.bind(this->sql);
    this->execute(this->sql);
}

bool PostgreSQLStatement::execute() {
    Param param;
    param.set(this->bindValueByIndex);
    param.set(this->bindValueByName);
    return this->execute(param);
}

bool PostgreSQLStatement::execute(std::string sql) {
    if(this->res != nullptr){
        PQclear(this->res);
        this->res = nullptr;
    }
    this->res = PQexec(this->connection, sql.c_str());
    this->status = PQresultStatus(this->res);
    if (this->status != PGRES_TUPLES_OK && this->status != PGRES_COMMAND_OK) {
        auto error = PQerrorMessage(this->connection);
        auto resError = PQresultErrorMessage(this->res);
        auto message = std::string("\"PostgreSQLStatement::execute\" failed: ") + error + " | " + resError;

        PQclear(this->res);
        this->res = nullptr;

        throw PostgreSQLException(message);
    }

    return true;
}

Line PostgreSQLStatement::fetch() {
    if(++this->current < PQntuples(this->res)) {
        Line line;
        auto nFields = PQnfields(this->res);
        for (int j = 0; j < nFields; j++) {
            auto string_val = std::string(PQgetvalue(this->res, this->current, j));
            auto val = Value(string_val);
            line[PQfname(this->res, j)] = val;
        }
        return line;
    }
    return {};
}

vector<Line> PostgreSQLStatement::fetchAll() {
    if(this->res == nullptr){
        throw PostgreSQLException("No Statement Executed!");
    }
    vector<Line> result;
    Line line;
    do {
        line = this->fetch();
        if(!line.empty()) {
            result.push_back(line);
        }
    } while(!line.empty());
    return result;
}

int PostgreSQLStatement::rowCount() {
    auto count = std::string(PQcmdTuples(this->res));
    if(!count.empty()) {
        return Math::toInt(count);
    }
    return PQntuples(this->res);
}

std::string PostgreSQLStatement::errorInfo() {
    auto error = PQerrorMessage(this->connection);
    auto resError = PQresultErrorMessage(this->res);
    return error ?: resError;
}

int PostgreSQLStatement::errorCode() {
    return this->status;
}

int PostgreSQLStatement::columnCount() {
    return PQnfields(this->res);
}

void PostgreSQLStatement::closeCursor() {
    if(this->res != nullptr) {
        PQclear(this->res);
        this->res = nullptr;
    }
}

std::string PostgreSQLStatement::debugDumpParams() {
    return this->sql;
}

void PostgreSQLStatement::bindValue(int index, Value value) {
    this->bindValueByIndex[index] = value;
}

void PostgreSQLStatement::bindValue(std::string name, Value value) {
    this->bindValueByName[name] = value;
}

std::string PostgreSQLStatement::quote(const std::string &value) {
    return "'" + value + "'";
}
/*

std::string PostgreSQLStatement::valueToSting(Value value) {
    std::string result;
    if (value.isString()) {
        result += std::string("'");
        result += value.getString();
        result += std::string("'");
    } else if (value.isInteger()) {
        result += std::to_string(value.getInteger());
    } else if (value.isFloat()) {
        result += std::to_string(value.getFloat());
    } else if (value.isDouble()) {
        result += std::to_string(value.getDouble());
    }
    return result;
}*/