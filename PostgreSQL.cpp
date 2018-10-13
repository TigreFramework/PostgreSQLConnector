//
// Created by pedro on 29/11/17.
//

#include "PostgreSQL.h"
#include <libpq-fe.h>
#include <TigreFramework/Types/Math.h>

PostgreSQL * PostgreSQL::singleton = nullptr;
std::map<std::thread::id, PostgreSQL*> PostgreSQL::pool;

PostgreSQL::PostgreSQL() {
    auto config = PostgreSQL::getConfiguration();

    string host = config["host"];
    string dbname = config["dbname"];
    string user = config["user"];
    string password = config["password"];
    string port = config["port"];

    string connection = "host="+host+" port="+port+" dbname="+dbname+" user="+user+" password="+password;
    const char *conninfo = connection.c_str();
    this->conn = PQconnectdb(conninfo);
}

Database* PostgreSQL::connection() {
    auto config = PostgreSQL::getConfiguration();
    if(config["manager"] == "singleton"){
        if(PostgreSQL::singleton == nullptr){
            PostgreSQL::singleton = new PostgreSQL();
        }
        return PostgreSQL::singleton;
    }else if(config["manager"] == "pool"){
        std::thread::id id =  std::this_thread::get_id();
        auto it = PostgreSQL::pool.find(id);
        if(it != PostgreSQL::pool.end()) {
            return PostgreSQL::pool[id];
        }else{
            PostgreSQL::pool[id] = new PostgreSQL();
            return PostgreSQL::pool[id];
        }
    }else{
        throw "Invalid Manager!";
    }
}

Lines PostgreSQL::execute(std::string sql) {
    PGresult *res = PQexec(this->conn, sql.c_str());
    if (PQresultStatus(res) != PGRES_TUPLES_OK && PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = PQerrorMessage(this->conn);
        PQclear(res);
        PQfinish(this->conn);
        throw error;
    }

    vector<Line> result;
    auto nFields = PQnfields(res);
    for (int i = 0; i < PQntuples(res); i++) {
        Line line;
        for (int j = 0; j < nFields; j++) {
            Value val = Value(std::string(PQgetvalue(res, i, j)));
            line[std::string(PQfname(res, j))] = std::move(val);
        }
        result.push_back(line);
    }
    return result;
}

Lines PostgreSQL::execute(std::string sql, std::vector<Value> values) {
    std::string result;
    int j = 0;
    for (char i : sql) {
        if(i == '?'){
            if(values[j].isString()){
                result += std::string("'");
                result += values[j].getString();
                result += std::string("'");
            }else if(values[j].isInteger()){
                result += std::to_string(values[j].getInteger());
            }else if(values[j].isFloat()){
                result += std::to_string(values[j].getFloat());
            }else if(values[j].isDouble()){
                result += std::to_string(values[j].getDouble());
            }
            j ++;
        }else{
            result += std::string(1, i);
        }
    }
    return this->execute(result);
}