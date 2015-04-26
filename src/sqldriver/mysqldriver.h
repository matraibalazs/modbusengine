#ifndef MYSQLDRIVER_H
#define MYSQLDRIVER_H

#include <cppconn/driver.h>
#include <cppconn/statement.h>
#include <cppconn/resultset.h>
#include <cppconn/exception.h>

#include "sqldriver.h"

namespace ModbusEngine
{

/**
 * @brief The MySQLDriver class
 *
 * Driver class for access MySQL databases.
 */
class MySQLDriver : public SQLDriver
{

private:
    /// delivered driver object by libmysqlcppconn
    sql::Driver* mysql;
    /// delivered connection object by libmysqlcppconn
    sql::Connection* conn;
    /// main access parameters
    std::string url;
    std::string port;
    std::string dbName;
    std::string user;
    std::string pass;

public:
    MySQLDriver( std::string url,
                 std::string port,
                 std::string dbName,
                 std::string user,
                 std::string pass );

    /**
     * @brief connect
     *
     * Connect to a database. Create the this->conn object via this->mysql.
     *
     * This method throws SQLDriverException.
     */
    void connect() throw( SQLDriverException );

    /**
     * @brief close
     *
     * Close a database connection. Deletes this->conn object.
     *
     * This method throws SQLDriverException.
     */
    void close() throw( SQLDriverException );

    /**
     * @brief execute
     * @param sql -> the sql query string
     *
     * Execute sql query without passing the output.
     * This method is for insert, delete, update, create etc.
     *
     * This method throws SQLDriverException.
     */
    void execute( std::string sql ) throw( SQLDriverException );

    /**
     * @brief executeQuery
     * @param sql   -> query string
     * @return      -> SQLResult object the result of the query
     *
     * Execute sql query with passing the output.
     * This method is for select queries.
     *
     * This method throws SQLDriverException.
     */
    SQLResult executeQuery( std::string sql ) throw( SQLDriverException );
};

}

#endif // SQLSERVICE_H
