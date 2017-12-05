//
// Created by pedro on 29/11/17.
//

#ifndef INTEGRATION_POSTGRESS_H
#define INTEGRATION_POSTGRESS_H

#include <libpq-fe.h>
#include <string>
#include <map>
#include <vector>
#include <iostream>
#include "../Types/Value.h"

using namespace std;

#define Line map<string, string>

class Result {

    public:
        Result(PGresult *pResult);
        ~Result();
        std::vector<Line> fetchAll();
        Line fetch();
        int size();

    private:
        PGresult *pResult;
        int location = 0;
};

class PostgreSQL {

    public:
        PostgreSQL();
        ~PostgreSQL();
        Result *execute(std::string);

        static std::string prepare(std::string command, std::vector<Value> values);

    private:
        PGconn *conn;

};


#endif //INTEGRATION_POSTGRESS_H
