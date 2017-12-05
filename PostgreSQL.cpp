//
// Created by pedro on 29/11/17.
//

#include "PostgreSQL.h"

PostgreSQL::PostgreSQL() {
    const char *conninfo = "host=localhost port=5432 dbname=Api user=pgsql password=1234";
    this->conn = PQconnectdb(conninfo);
}

PostgreSQL::~PostgreSQL() {
    PQfinish(conn);
}

Result *PostgreSQL::execute(string sql) {
    PGresult *res = PQexec(conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK) {
        fprintf(stderr, "SELECT FROM failed: %s", PQerrorMessage(conn));
        PQclear(res);
        PQfinish(conn);
        throw new exception(); // PQerrorMessage(conn)
    }
    return new Result(res);
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
            line[PQfname(pResult, j)] = PQgetvalue(pResult, i, j);
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
        line[PQfname(pResult, j)] = PQgetvalue(pResult, this->location, j);
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