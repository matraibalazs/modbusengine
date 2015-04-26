#include "mysqldriver.h"

#include <iostream>

namespace ModbusEngine
{

MySQLDriver::MySQLDriver( std::string url,
                          std::string port,
                          std::string dbName,
                          std::string user,
                          std::string pass )
{
    this->url = url;
    this->port = port;
    this->dbName = dbName;
    this->user = user;
    this->pass = pass;

    this->conn = NULL;

    try
    {
        this->mysql = get_driver_instance();
    }
    catch( sql::SQLException& ex )
    {
        // Nothing to do...fucker...
    }
}

void MySQLDriver::connect() throw( SQLDriverException )
{
    if( this->conn == NULL )
    {
        try
        {
            std::stringstream ss;
            ss << "tcp://" << this->url << ":" << this->port;
            this->conn = this->mysql->connect( ss.str(), this->user, this->pass );
            conn->setSchema( this->dbName );
        }
        catch( sql::SQLException& ex )
        {
            this->conn = NULL;
            SQLDriverException e( "connection_error", ex.what() );
            throw e;
        }
    }
}

void MySQLDriver::close() throw( SQLDriverException )
{
    if( this->conn != NULL )
    {
        this->conn->close();
        delete this->conn;
        this->conn = NULL;
    }
}

void MySQLDriver::execute( std::string sql ) throw( SQLDriverException )
{
    sql::Statement* stmt = NULL;

    try
    {
        stmt = this->conn->createStatement();
        stmt->execute( sql );
        stmt->close();
        delete stmt;
    }
    catch( sql::SQLException& ex )
    {
        if( stmt != NULL )
        {
            delete stmt;
        }

        SQLDriverException e( "execute_error", ex.what() );
        throw e;
    }
}

SQLResult MySQLDriver::executeQuery( std::string sql ) throw( SQLDriverException )
{
    sql::Statement* stmt = NULL;
    sql::ResultSet* rs = NULL;
    sql::ResultSetMetaData* meta_data = NULL;

    try
    {
        stmt = this->conn->createStatement();
        rs = stmt->executeQuery( sql );
        meta_data = rs->getMetaData();
        unsigned int colnum = meta_data->getColumnCount();
        std::map<int,SQLRow> rows;
        int k = 0;

        while( rs->next() )
        {
            std::map<std::string,std::string> row;

            for( unsigned int i = 1; i <= colnum; i++ )
            {
                std::pair<std::string,std::string> p;
                p.first = meta_data->getColumnName( i );

                if( meta_data->getColumnTypeName( i ) == "INT" )
                {
                    int val = rs->getInt( p.first );
                    std::stringstream ss;
                    ss << val;
                    p.second = ss.str();
                }
                else if( meta_data->getColumnTypeName( i ) == "VARCHAR" )
                {
                    std::string val = rs->getString( p.first );
                    p.second = val;
                }

                row.insert( p );
            }

            std::pair<int,SQLRow> p;
            p.first = k;
            p.second = row;
            rows.insert( p );
            k++;
        }

        stmt->close();
        delete rs;
        delete stmt;

        SQLResult res( rows );
        return res;
    }
    catch( sql::SQLException& ex )
    {
        if( stmt != NULL )
        {
            delete stmt;
        }

        if( rs != NULL )
        {
            delete rs;
        }

        SQLDriverException e( "execute_error", ex.what() );
        throw e;
    }
}

} // namespace ModbusEngine
