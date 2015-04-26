#ifndef MODBUSDRIVERMONITORINTERFACE_H
#define MODBUSDRIVERMONITORINTERFACE_H

#include <vector>
#include <string>

namespace ModbusEngine
{

/**
 * @brief The ModbusDriverMonitorInterface class
 *
 * Defines the modbus driver monitor interface.
 */

class ModbusDriverMonitorInterface
{

public:
    std::vector<std::string> virtual getAllDeviceId() = 0;
    std::vector<std::string> virtual getAllBlockId( std::string deviceId ) = 0;

    std::string virtual readDeviceIp( std::string deviceId ) = 0;
    int virtual readDevicePort( std::string deviceId ) = 0;
    int virtual readDeviceSlaveId( std::string deviceId ) = 0;
    int virtual readDeviceResponseTimeout( std::string deviceId ) = 0;
    int virtual readDeviceConnectionTimeout( std::string deviceId ) = 0;
    std::string virtual readDeviceConnStatus( std::string deviceId ) = 0;

    int virtual readBlockOffset( std::string deviceId, std::string blockId ) = 0;
    int virtual readBlockCount( std::string deviceId, std::string blockId ) = 0;
    int virtual readBlockCycleTime( std::string deviceId, std::string blockId ) = 0;
    int virtual readBlockRetries( std::string deviceId, std::string blockId ) = 0;
    std::string virtual readBlockError( std::string deviceId, std::string blockId ) = 0;

};

}

#endif // MODBUSDRIVERMONITORINTERFACE_H
