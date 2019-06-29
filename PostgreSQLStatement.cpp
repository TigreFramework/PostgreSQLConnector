#include <libpq-fe.h>
#include <TigreFramework/PostgreSQLConnector/Exceptions/PostgreSQLException.h>
#include <TigreFramework/String/String.h>
#include <TigreFramework/Database/Line.h>
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
        prepared_sql = this->sql.c_str();
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
    if (PQresultStatus(this->res) != PGRES_TUPLES_OK) {
        auto message = std::string("\"PostgreSQLStatement::execute\" failed: ") + PQerrorMessage(this->connection);

        PQclear(this->res);
        this->res = nullptr;

        throw PostgreSQLException(message);
    }

    return true;
}

vector<Line> PostgreSQLStatement::fetchAll() {
    if(this->res == nullptr){
        throw PostgreSQLException("No Statement Executed!");
    }
    vector<Line> result;
    auto nFields = PQnfields(this->res);
    for (int i = 0; i < PQntuples(this->res); i++) {
        Line line;
        for (int j = 0; j < nFields; j++) {
            auto string_val = std::string(PQgetvalue(this->res, i, j));
            auto val = Value(string_val);
            line[PQfname(this->res, j)] = val;
        }
        result.push_back(line);
    }
    return result;
}
