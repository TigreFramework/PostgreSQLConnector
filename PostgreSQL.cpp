#include <postgresql/libpq-fe.h>
#include "PostgreSQL.h"

PostgreSQL::PostgreSQL() {
    const char *conninfo = "host=postgres-database port=5432 dbname=postgres user=postgres password=1234";
    this->conn = PQconnectdb(conninfo);
}

PostgreSQL::~PostgreSQL() {
    PQfinish(conn);
}

Lines PostgreSQL::execute(std::string sql, std::vector<Value> values) {
    return this->execute(sql);
}

Lines PostgreSQL::execute(string sql) {
    PGresult *res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        auto message = std::string("\"PostgreSQL::execute\" failed: ") + PQerrorMessage(conn);
        PQclear(res);
        PQfinish(conn);
        throw message;
        //throw exception();
    }
    return (new Result(res))->fetchAll();
}

Result::Result(PGresult *pResult) {
    this->pResult = pResult;
}

Result::~Result() {
    PQclear(pResult);
}

int Result::size() {
    return atoi(PQcmdTuples(pResult));
}

vector<Line> Result::fetchAll() {
    vector<Line> result;
    auto nFields = PQnfields(pResult);
    for (int i = 0; i < PQntuples(pResult); i++) {
        Line line;
        for (int j = 0; j < nFields; j++) {
            auto string_val = std::string(PQgetvalue(pResult, i, j));
            auto val = Value(string_val);
            line[PQfname(pResult, j)] = val;
        }
        result.push_back(line);
    }
    return result;
}

Line Result::fetch() {
    if(this->location >= this->size()){
        //return NULL;
    }
    Line line;
    auto nFields = PQnfields(pResult);
    for (int j = 0; j < nFields; ++j) {
        auto string_val = std::string(PQgetvalue(pResult, this->location, j));
        auto val = Value(string_val);
        line[PQfname(pResult, j)] = val;
    }
    this->location++;
    return line;
}

std::string PostgreSQL::prepare(std::string command, std::vector<Value> values) {
    std::string result = "";
    int j = 0;
    for (int i = 0; i < command.length(); ++i) {
        if(command[i] == '?'){
            if(values[j].isString()){
                result += std::string("\"");
                result += values[j].getString();
                result += std::string("\"");
            }else if(values[j].isInteger()){
                result += std::to_string(values[j].getInteger());
            }else if(values[j].isFloat()){
                result += std::to_string(values[j].getFloat());
            }else if(values[j].isDouble()){
                result += std::to_string(values[j].getDouble());
            }
            j ++;
        }else{
            result += std::string(1, command[i]);
        }
    }
    return result;
}