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
    PQclear(res);
    this->res = nullptr;
}

bool PostgreSQLStatement::execute() {
    return this->execute(this->sql);
}

bool PostgreSQLStatement::execute(std::map<string, Value> params) {
    std::string prepared_sql;

    if(params.empty()) {
        prepared_sql = this->sql;
    } else {
        Tigre::String sql = this->sql;
        for(const auto & param : params) {
            const auto & p = param.second;
            Tigre::String value;
            if (p.isString()) {
                value += std::string("\"");
                value += p.getString();
                value += std::string("\"");
            } else if (p.isInteger()) {
                value += std::to_string(p.getInteger());
            } else if (p.isFloat()) {
                value += std::to_string(p.getFloat());
            } else if (p.isDouble()) {
                value += std::to_string(p.getDouble());
            }
            sql.replace(param.first, value);
        }
        prepared_sql = sql.getValue();
    }

    return this->execute(prepared_sql);
}

bool PostgreSQLStatement::execute(std::vector<Value> params) {
    std::string result;
    int j = 0;
    if(!params.empty()) {
        for (char i : this->sql) {
            if (i == '?' && j < params.size()) {
                if (params[j].isString()) {
                    result += std::string("\"");
                    result += params[j].getString();
                    result += std::string("\"");
                } else if (params[j].isInteger()) {
                    result += std::to_string(params[j].getInteger());
                } else if (params[j].isFloat()) {
                    result += std::to_string(params[j].getFloat());
                } else if (params[j].isDouble()) {
                    result += std::to_string(params[j].getDouble());
                }
                j++;
            } else {
                result += std::string(1, i);
            }
        }
    } else {
        result = this->sql;
    }
    return this->execute(result);
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
