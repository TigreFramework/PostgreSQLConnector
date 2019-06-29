#ifndef INTEGRATION_POSTGRESS_H
#define INTEGRATION_POSTGRESS_H

#include <string>
#include <map>
#include <vector>
#include <iostream>
#include <TigreFramework/Database/Value.h>
#include <TigreFramework/Database/Database.h>

using namespace std;

#ifndef LIBPQ_FE_H
typedef struct PGresult;
typedef struct PGconn;
#endif

/*#define Line map<string, string>*/

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

class PostgreSQL : public Database {

    public:
        PostgreSQL();
        ~PostgreSQL();
        Lines execute(std::string);
        Lines execute(std::string sql, std::vector<Value> values);

        static std::string prepare(std::string command, std::vector<Value> values);

    private:
        PGconn *conn;

};


#endif //INTEGRATION_POSTGRESS_H
