//
// Created by pedro on 29/11/17.
//

#ifndef INTEGRATION_POSTGRESS_H
#define INTEGRATION_POSTGRESS_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <TigreFramework/Database/Database.h>
#include <TigreFramework/Database/Line.h>
#include <thread>
#include <TigreFramework/Database/Value.h>

using namespace std;

struct pg_result;
struct pg_conn;

class PostgreSQL: public Database {

    public:
        PostgreSQL();

        static Database* connection();
        Lines execute(std::string sql, std::vector<Value> values) override;
        Lines execute(std::string sql) override;

    protected:
        pg_conn *conn;

        static PostgreSQL * singleton;
        static std::map<std::thread::id, PostgreSQL*> pool;

};


#endif //INTEGRATION_POSTGRESS_H
