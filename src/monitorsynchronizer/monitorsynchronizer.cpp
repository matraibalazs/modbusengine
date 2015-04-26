#include <sstream>

#include "monitorsynchronizer.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine {

MonitorSynchronizer::MonitorSynchronizer( MBPro* mbpro,
                                          ModbusDriverMonitorInterface* monitorInterface )
                                          throw( std::string )
{
    this->monitorInterface = monitorInterface;
    this->mbpro = mbpro;
    this->cycleTime = 500;

    /// create the sql driver object
    try
    {
        this->mysqlDriver = new MySQLDriver( this->mbpro->db.dbUrl,
                                             this->mbpro->db.dbPort,
                                             this->mbpro->db.dbName,
                                             this->mbpro->db.dbUser,
                                             this->mbpro->db.dbPass );
    }
    catch( SQLDriverException ex )
    {
        throw "Error: " + ex.description;
    }

    /// create required tables
    try{
         this->build_tables();
    } catch( std::string ex ) {
        throw "Error: " + ex;
    }
}

void MonitorSynchronizer::build_tables() throw( std::string )
{
    std::stringstream sql;

    /// connect to db...
    try
    {
        this->mysqlDriver->connect();
    }
    catch( SQLDriverException ex )
    {
        throw ex.description;
    }

    /// drop tables...
    try
    {
        sql << "DROP TABLE devices;";
        this->mysqlDriver->execute( sql.str() );
        sql.str("");
        sql << "DROP TABLE blocks;";
        this->mysqlDriver->execute( sql.str() );
    }
    catch( SQLDriverException )
    {
        /// doing nothing :-)
    }

    /// create and fill tables
    try
    {
        sql.str("");
        sql << "CREATE TABLE devices";
        sql << "(";
        sql << "id int(11) NOT NULL,";
        sql << "device_id varchar(500) DEFAULT NULL,";
        sql << "ip varchar(100) DEFAULT NULL,";
        sql << "port int(11) DEFAULT NULL,";
        sql << "slave_id int(11) DEFAULT NULL,";
        sql << "response_timeout int(11) DEFAULT NULL,";
        sql << "connection_timeout int(11) DEFAULT NULL,";
        sql << "conn_status varchar(100) DEFAULT NULL,";
        sql << "PRIMARY KEY (id)";
        sql << ")";
        sql << "DEFAULT CHARSET=utf8 COLLATE=utf8_hungarian_ci ENGINE=MEMORY;";
        this->mysqlDriver->execute( sql.str() );

        sql.str("");
        sql << "CREATE TABLE blocks";
        sql << "(";
        sql << "id int(11),";
        sql << "block_id varchar(500) COLLATE utf8_hungarian_ci,";
        sql << "device_id varchar(500) COLLATE utf8_hungarian_ci,";
        sql << "offset int(11) DEFAULT NULL,";
        sql << "count int(11) DEFAULT NULL,";
        sql << "cycle_time int(11) DEFAULT NULL,";
        sql << "retries int(11) DEFAULT NULL,";
        sql << "error varchar(100) COLLATE utf8_hungarian_ci DEFAULT NULL,";
        sql << "PRIMARY KEY (id)";
        sql << ")";
        sql << "DEFAULT CHARSET=utf8 COLLATE=utf8_hungarian_ci ENGINE=MEMORY;";
        this->mysqlDriver->execute( sql.str() );

        std::vector<std::string> deviceIds = monitorInterface->getAllDeviceId();
        int id = 0;
        for( std::vector<std::string>::iterator it = deviceIds.begin();
             it != deviceIds.end(); it++ )
        {
            std::string deviceId = *it;

            sql.str("");
            sql << "INSERT INTO devices";
            sql << " VALUES ";
            sql << "(";
            sql << id << ",";
            sql << "'" << deviceId << "',";
            sql << "'" << monitorInterface->readDeviceIp( deviceId ) << "',";
            sql << monitorInterface->readDevicePort( deviceId ) << ",";
            sql << monitorInterface->readDeviceSlaveId( deviceId ) << ",";
            sql << monitorInterface->readDeviceResponseTimeout( deviceId ) << ",";
            sql << monitorInterface->readDeviceConnectionTimeout( deviceId ) << ",";
            sql << "'" << monitorInterface->readDeviceConnStatus( deviceId ) << "'";
            sql << ");";
            this->mysqlDriver->execute( sql.str() );

            /// fill the devices cache...
            this->devicesUpdateCache[ id++ ] = monitorInterface->readDeviceConnStatus( deviceId );
        }

        id = 0;
        for( std::vector<std::string>::iterator it = deviceIds.begin();
             it != deviceIds.end(); it++ )
        {
            std::string deviceId = *it;
            std::vector<std::string> blockIds = monitorInterface->getAllBlockId( deviceId );
            for( std::vector<std::string>::iterator it_1 = blockIds.begin(); it_1 != blockIds.end(); it_1++ )
            {
                std::string blockId = *it_1;

                sql.str("");
                sql << "INSERT INTO blocks";
                sql << " VALUES ";
                sql << "(";
                sql << id << ",";
                sql << "'" << blockId << "',";
                sql << "'" << deviceId << "',";
                sql << monitorInterface->readBlockOffset( deviceId, blockId ) << ",";
                sql << monitorInterface->readBlockCount( deviceId, blockId ) << ",";
                sql << monitorInterface->readBlockCycleTime( deviceId, blockId ) << ",";
                sql << monitorInterface->readBlockRetries( deviceId, blockId ) << ",";
                sql << "'" << monitorInterface->readBlockError( deviceId, blockId ) << "'";
                sql << ");";
                this->mysqlDriver->execute( sql.str() );

                /// fill te blocks cache...
                this->blocksUpdateCache[ id++ ] = monitorInterface->readBlockError( deviceId, blockId );
           }
        }
    }
    catch( SQLDriverException ex )
    {
        this->mysqlDriver->close();
        throw ex.description;
    }

    /// close DB
    this->mysqlDriver->close();
}

void MonitorSynchronizer::refresh_tables()
{
    /// connect to db...
    try
    {
        this->mysqlDriver->connect();
    }
    catch( SQLDriverException )
    {
        /// ...
    }

    try
    {
        /// refresh device connection statuses
        int di = 0;
        std::vector<std::string> deviceIds = monitorInterface->getAllDeviceId();
        for( std::vector<std::string>::iterator it = deviceIds.begin(); it != deviceIds.end(); it++ )
        {
            std::string deviceId = *it;
            std::string connStatus = monitorInterface->readDeviceConnStatus( deviceId );

            if( this->devicesUpdateCache[ di ] != connStatus )
            {
                std::stringstream sql;
                sql << "UPDATE devices SET conn_status='" << connStatus << "'";
                sql << " WHERE device_id='" << deviceId << "';";
                this->mysqlDriver->execute( sql.str() );
                this->devicesUpdateCache[ di ] = connStatus;
            }
            di++;
        }

        /// refresh block error statuses
        int bi = 0;
        for( std::vector<std::string>::iterator it = deviceIds.begin(); it != deviceIds.end(); it++ )
        {
            std::string deviceId = *it;
            std::vector<std::string> blockIds = monitorInterface->getAllBlockId( deviceId );
            for( std::vector<std::string>::iterator it_1 = blockIds.begin(); it_1 != blockIds.end(); it_1++ )
            {
                std::string blockId = *it_1;
                std::string error = monitorInterface->readBlockError( deviceId, blockId );

                if( this->blocksUpdateCache[ bi ] != error )
                {
                    std::stringstream sql;
                    sql << "UPDATE blocks SET error='" << error << "'";
                    sql << " WHERE device_id='" << deviceId << "' AND " << "block_id='" << blockId << "';";
                    this->mysqlDriver->execute( sql.str() );
                    this->blocksUpdateCache[ bi ] = error;
                }
                bi++;
            }
        }
    }
    catch( SQLDriverException )
    {
        this->mysqlDriver->close();
    }

    /// close the connection
    this->mysqlDriver->close();
}

void MonitorSynchronizer::run()
{
    while( true ) {
        /// refresh monitor data...
        this->refresh_tables();

        /// wait a bit...
        Thread::msleep( this->cycleTime );
    }
}

void MonitorSynchronizer::halt(){}

}
