#include "PostgreSQLConfiguration.h"
#include "PostgreSQLDataObject.h"

DataObject* PostgreSQLConfiguration::getInstance(const string &dns, const string &username, const string &passwd,
                                                 const map<string, string> &options) {
    return new PostgreSQLDataObject(dns, username, passwd, options);
}
