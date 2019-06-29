#ifndef TIGREFRAMEWORK_POSTGRESQLEXCEPTION_H
#define TIGREFRAMEWORK_POSTGRESQLEXCEPTION_H


#include <TigreFramework/Core/Kernel/Exception.h>

class PostgreSQLException : public Exception {

public:
    PostgreSQLException();
    PostgreSQLException(std::string message);

};


#endif //TIGREFRAMEWORK_POSTGRESQLEXCEPTION_H
