#ifndef TIGREFRAMEWORK_POSTGRESQLDATAOBJECT_H
#define TIGREFRAMEWORK_POSTGRESQLDATAOBJECT_H

#include <TigreFramework/Database/DataObject.h>
#include <TigreFramework/Database/DataObjectStatement.h>

#ifndef LIBPQ_FE_H
typedef struct PGresult;
typedef struct PGconn;
#endif

class PostgreSQLDataObject : public DataObject {

public:
    /**
     * Create Connection
     * @param dns
     * @param username
     * @param passwd
     * @param options
     */
    PostgreSQLDataObject(const string &dns, const string &username, const string &passwd,
                         const map<string, string> &options);
    /**
     * Destroy Connection
     */
    ~PostgreSQLDataObject() override ;
    /**
     * Inicia uma transação
     */
    void beginTransaction();
    /**
     * Envia uma transação
     */
    void commit();
    /**
     * Fetch the SQLSTATE associated with the last operation on the database handle
     */
    void errorCode();
    /**
     * Fetch extended error information associated with the last operation on the database handle
     */
    void errorInfo();
    /**
     * Executa uma instrução SQL e retornar o número de linhas afetadas
     */
    void exec();
    /**
     * Recuperar um atributo da conexão com o banco de dados
     */
    void getAttribute();
    /**
     * Retorna um array com os drivers PDO disponíveis
     */
    void getAvailableDrivers();
    /**
     * Checks if inside a transaction
     */
    void inTransaction();
    /**
     * Returns the ID of the last inserted row or sequence value
     */
    int lastInsertId();
    /**
     * Prepares a statement for execution and returns a statement object
     */
    DataObjectStatement* prepare(std::string sql);
    /**
     * Executes an SQL statement, returning a result set as a PDOStatement object
     */
    void query();
    /**
     * Quotes a string for use in a query
     */
    void quote();
    /**
     * Rolls back a transaction
     */
    void rollBack();
    /**
     * Set an attribute
     */
    void setAttribute();

private:
    PGconn * connection;
    std::vector<DataObjectStatement*> statement;


};


#endif //TIGREFRAMEWORK_POSTGRESQLDATAOBJECT_H
