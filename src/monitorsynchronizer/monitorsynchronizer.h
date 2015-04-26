#ifndef MONITORSYNCHRONIZER_H
#define MONITORSYNCHRONIZER_H

#include "../ModbusDriver/modbusdrivermonitorinterface.h"
#include "../mbpro.h"
#include "../Core/thread.hpp"
#include "../SQLDriver/mysqldriver.h"

namespace ModbusEngine
{

/**
 * @brief The MonitorSynchronizer class
 *
 * Synchronizes monitor values to database.
 * Uses the delivered monitor interface.
 */
class MonitorSynchronizer : public Thread
{

private:
    /// working cycletime in millisecs
    int cycleTime;

    /// delivered monitor interface (the driver itself)
    ModbusDriverMonitorInterface* monitorInterface;
    /// delivered mbpro file
    MBPro* mbpro;
    /// inner created mysql driver
    MySQLDriver* mysqlDriver;

    /// caches for devices and blocks
    std::map<int,std::string> devicesUpdateCache;
    std::map<int,std::string> blocksUpdateCache;

    /**
     * @brief build_tables
     *
     * Build the required sql data tables.
     * Called by the constructor.
     *
     * The function throws std::string exception.
     */
    void build_tables() throw( std::string );

    /**
     * @brief refresh_tables
     *
     * Refresh datatables.
     */
    void refresh_tables();

public:
    MonitorSynchronizer( MBPro*, ModbusDriverMonitorInterface* ) throw( std::string );

    /**
     * @brief run
     *
     * Inherited function from Thread class.
     */
    void run();

    /**
     * @brief halt
     *
     * Inherited function from Thread class.
     */
    void halt();

};

}

#endif // MONITORSYNCHRONIZER_H
