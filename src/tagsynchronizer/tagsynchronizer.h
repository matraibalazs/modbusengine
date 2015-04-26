#ifndef TAGSYNCHRONIZER_H
#define TAGSYNCHRONIZER_H

#include <map>

#include "../Core/thread.hpp"
#include "../mbpro.h"
#include "tag.h"
#include "../ModbusDriver/modbusdriverdatainterface.h"
#include "../SQLDriver/mysqldriver.h"

namespace ModbusEngine
{

/**
 * @brief The TagSynchronizer class
 *
 * Stores the tags, refreshs tags table in database, writes data to modbusdriver.
 * Cache is working on refresh tags.
 */
class TagSynchronizer : public Thread
{

private:
    /// working cycletime
    int cycleTime;

    /// devlivered mbpro file
    MBPro* mbpro;
    /// delivered driver data interface
    ModbusDriverDataInterface* driverInterface;
    /// sql driver for access database
    MySQLDriver* mysqlDriver;
    /// store the tags
    std::map<int,Tag*> tagMap;
    /// caches for tag values and validity flags
    std::map<int,std::string> tagValueCache;
    std::map<int,std::string> tagValidityCache;

    /// build functions for build the required map for tags and create datatables
    void build_tag_map();
    void build_tables() throw( std::string );

    /// read and write helper functions
    void do_read();
    void do_write();

    /// heartbeat algorythm
    void do_heartbeat();

public:
    /**
     * @brief TagSynchronizer
     * @param mbpro         -> delivered mbpro file
     * @param interface     -> delivered driver data interface object
     *
     * Creates the synchronizer object.
     *
     * This function throws std::string exceptions:
     *
     *      ""
     */
    TagSynchronizer( MBPro* mbpro , ModbusDriverDataInterface* interface ) throw( std::string );

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

#endif // TAGSYNCHRONIZER_H
