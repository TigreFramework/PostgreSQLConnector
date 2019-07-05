#ifndef TIGREFRAMEWORK_POSTGRESQLSTATEMENT_H
#define TIGREFRAMEWORK_POSTGRESQLSTATEMENT_H


#include <TigreFramework/Database/DataObjectStatement.h>

class PostgreSQLStatement : public DataObjectStatement {

public:
    PostgreSQLStatement(PGconn * connection, std::string sql);
    ~PostgreSQLStatement() override ;
    
    /**
     * Bind a column to a PHP variable
     */
    void bindColumn() override { }
    /**
     * Binds a parameter to the specified variable name
     */
    void bindParam() override { }
    /**
     * Binds a value to a parameter
     */
    void bindValue(int index, Value value) override ;
    void bindValue(std::string name, Value value) override ;
    /**
     * Closes the cursor, enabling the statement to be executed again
     */
    void closeCursor() override ;
    /**
     * Returns the number of columns in the result set
     */
    int columnCount() override ;
    /**
     * Dump an SQL prepared command
     */
    std::string debugDumpParams() override ;
    /**
     * Fetch the SQLSTATE associated with the last operation on the statement handle
     */
    int errorCode() override ;
    /**
     * Fetch extended error information associated with the last operation on the statement handle
     */
    std::string errorInfo() override ;
    /**
     * Executes a prepared statement
     *
     * Using named field
     * Using interrogation as marker
     *
     * @param params
     * @return bool
     */
    bool execute(const Param& params) override ;
    /**
     * No bind Used
     * @return bool
     */
    bool execute()  override ;
    /**
     * Fetches the next row from a result set
     */
    Line fetch() override ;
    /**
     * Returns an array containing all of the result set rows
     */
    vector<Line> fetchAll() override ;
    /**
     * Returns the number of rows affected by the last SQL statement
     */
    int rowCount() override ;

private:
    PGconn * connection;
    PGresult * res = nullptr;
    string sql;
    int current = -1;

    int status = -1;

    std::map<int, Value> bindValueByIndex;
    std::map<std::string, Value> bindValueByName;

    bool execute(std::string sql);

};


#endif //TIGREFRAMEWORK_POSTGRESQLSTATEMENT_H
