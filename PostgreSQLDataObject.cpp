#include <libpq-fe.h>
#include <TigreFramework/PostgreSQLConnector/Exceptions/PostgreSQLException.h>
#include "PostgreSQLDataObject.h"
#include "PostgreSQLStatement.h"

PostgreSQLDataObject::PostgreSQLDataObject(const string &dns, const string &username, const string &passwd,
                                           const map<string, string> &options) : DataObject() {

    string connInfo = dns + " user="+username+" password="+passwd;
    this->connection = PQconnectdb(connInfo.c_str());
}

PostgreSQLDataObject::~PostgreSQLDataObject() {
    if(this->connection != nullptr) {
        PQfinish(this->connection);
    }
    for (auto &i : this->statement) {
        delete i;
    }
    this->statement.clear();
}

/**
 * Inicia uma transação
 */
void PostgreSQLDataObject::beginTransaction() {
    auto res = PQexec(this->connection, "BEGIN");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = std::string("BEGIN command failed: ")+PQerrorMessage(this->connection);
        PQclear(res);
        throw PostgreSQLException(error);
    }
    PQclear(res);
}
/**
 * Envia uma transação
 */
void PostgreSQLDataObject::commit() {
    auto res = PQexec(this->connection, "COMMIT");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = std::string("COMMIT command failed: ")+PQerrorMessage(this->connection);
        PQclear(res);
        throw PostgreSQLException(error);
    }
    PQclear(res);
}
/**
 * Fetch the SQLSTATE associated with the last operation on the database handle
 */
void PostgreSQLDataObject::errorCode() {}
/**
 * Fetch extended error information associated with the last operation on the database handle
 */
void PostgreSQLDataObject::errorInfo() {}
/**
 * Executa uma instrução SQL e retornar o número de linhas afetadas
 */
int PostgreSQLDataObject::exec(std::string sql) {
    auto psqls = new PostgreSQLStatement(this->connection, sql);
    this->statement.push_back(psqls);
    psqls->execute();
    return psqls->rowCount();
}
/**
 * Recuperar um atributo da conexão com o banco de dados
 */
void PostgreSQLDataObject::getAttribute() {}
/**
 * Retorna um array com os drivers PDO disponíveis
 */
void PostgreSQLDataObject::getAvailableDrivers() {}
/**
 * Checks if inside a transaction
 */
void PostgreSQLDataObject::inTransaction() {}
/**
 * Returns the ID of the last inserted row or sequence value
 */
int PostgreSQLDataObject::lastInsertId() {}
/**
 * Prepares a statement for execution and returns a statement object
 */
DataObjectStatement* PostgreSQLDataObject::prepare(std::string sql) {
    auto psqls = new PostgreSQLStatement(this->connection, sql);
    this->statement.push_back(psqls);
    return psqls;
}
/**
 * Executes an SQL statement, returning a result set as a PDOStatement object
 */
void PostgreSQLDataObject::query() {}
/**
 * Quotes a string for use in a query
 */
void PostgreSQLDataObject::quote() {}
/**
 * Rolls back a transaction
 */
void PostgreSQLDataObject::rollBack() {
    auto res = PQexec(this->connection, "ROLLBACK");
    if (PQresultStatus(res) != PGRES_COMMAND_OK) {
        std::string error = std::string("ROLLBACK command failed: ")+PQerrorMessage(this->connection);
        PQclear(res);
        throw PostgreSQLException(error);
    }
    PQclear(res);
}
/**
 * Set an attribute
 */
void PostgreSQLDataObject::setAttribute() {}
