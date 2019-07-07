#include <libpq-fe.h>
#include <TigreFramework/PostgreSQLConnector/Exceptions/PostgreSQLException.h>
#include <TigreFramework/String/String.h>
#include <TigreFramework/Database/Line.h>
#include <TigreFramework/Types/Math.h>
#include "PostgreSQLStatement.h"

#define MONEY 790
#define DATE 1082
#define NUMERIC 1700
#define TIMESTAMP 1114
#define DOUBLE 701
#define VARCHAR 1043
#define INT 23

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
    this->sql = params.bind(this->sql, this);
    this->execute(this->sql);
}

bool PostgreSQLStatement::execute() {
    Param param;
    param.set(this->bindValueByIndex);
    param.set(this->bindValueByName);
    this->bindValueByIndex.clear();
    this->bindValueByName.clear();
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
            std::string column_name = PQfname(this->res, j);
            if(this->bindColumnByIndex.find(j) != this->bindColumnByIndex.end()) {
                line[column_name] = this->bindColumnByIndex[j];
            }

            auto colType = PQftype(this->res, j);
            auto isNull = PQgetisnull(this->res, this->current, j);
            auto value = PQgetvalue(this->res, this->current, j);
            switch(colType) {
                case DOUBLE:
                    if(!isNull){
                        line[column_name] = Math::toDouble(value);
                    } else {
                        line[column_name] = "";
                    }
                    break;
                case INT:
                    if(!isNull){
                        line[column_name] = Math::toInt(value);
                    } else {
                        line[column_name] = "";
                    }
                    break;
                default:
                    line[column_name] = std::string(value);
            }
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

void PostgreSQLStatement::bindColumn(int index, Value value) {
    this->bindColumnByIndex[index] = value;
}
