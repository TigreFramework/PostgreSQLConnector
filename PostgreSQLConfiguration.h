#ifndef TIGREFRAMEWORK_POSTGRESQLCONFIGURATION_H
#define TIGREFRAMEWORK_POSTGRESQLCONFIGURATION_H


#include <TigreFramework/Database/DataObjectConfiguration.h>
#include <TigreFramework/Core/Kernel/Application/Configurable.h>

class PostgreSQLConfiguration : public DataObjectConfiguration, public Configurable {

public:
    DataObject* getInstance(const string &dns, const string &username, const string &passwd, const map<string, string> &options);

};


#endif //TIGREFRAMEWORK_POSTGRESQLCONFIGURATION_H
