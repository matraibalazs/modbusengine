#ifndef SQLDRIVER_H
#define SQLDRIVER_H

#include "sqldriverexception.hpp"
#include "sqlresult.hpp"

namespace ModbusEngine
{

/**
 * @brief The SQLDriver class
 *
 * Abstract SQL Driver class for access databases.
 */
class SQLDriver
{

public:
    virtual void connect() throw( SQLDriverException ) = 0;
    virtual void close() throw( SQLDriverException ) = 0;

    virtual void execute( std::string sql ) throw( SQLDriverException ) = 0;
    virtual SQLResult executeQuery( std::string sql ) throw( SQLDriverException ) = 0;
};

}

#endif // SQLDRIVER_H
