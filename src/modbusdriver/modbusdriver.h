#ifndef MODBUSDRIVER_H
#define MODBUSDRIVER_H

#include "modbusdevice.h"
#include "modbusdriverdatainterface.h"
#include "modbusdrivermonitorinterface.h"
#include "../mbpro.h"

namespace ModbusEngine
{

/**
 * @brief The ModbusDriver class
 *
 * Features:
 *   - stores ModbusDevices
 *   - multithread design
 *   - build function for add ModbusDevices
 *
 * Usage:
 *
 * 1. Create instance
 * 2. Call startBlockThreads()
 */

class ModbusDriver :
        public ModbusDriverDataInterface,
        public ModbusDriverMonitorInterface
{

private:
    /// Delivered mbpro file
    MBPro* mbpro;
    /// Map where we stores devices
    std::map<std::string,ModbusDevice*> devices;
    /// Required mutex for multi thread design
    std::mutex driverMutex;

    /**
     * @brief build
     *
     * Helper function for constructor.
     */
    void build_the_tree();

    /**
     * @brief add_modbus_device
     * @param deviceId  -> device id
     * @param device    -> the device
     *
     *  Helper function for add device to devices map.
     */
    void add_modbus_device( std::string deviceId, ModbusDevice* device );

public:
    /**
     * @brief ModbusDriver
     * @param mbpro -> delivered mbpro file
     *
     * Creates full object.
     */
    ModbusDriver( MBPro* mbpro );

    /**
     * @brief startBlockThreads
     *
     * Start all blocks in all devices.
     */
    void startBlockThreads();

    /**
     * @brief readBit
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nBit      -> bit position (bit offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    bool readBit( std::string deviceId, std::string blockId, int nReg, int nBit ) throw( std::string );

    /**
     * @brief writeBit
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nBit      -> bit position (bit offset inside register)
     * @param bit       -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    void writeBit( std::string deviceId, std::string blockId, int nReg, int nBit, bool bit ) throw( std::string );

    /**
     * @brief readByte
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nByte     -> byte position (byte offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint8 readByte( std::string deviceId, std::string blockId, int nReg, int nByte ) throw( std::string );

    /**
     * @brief writeByte
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nByte     -> byte position (byte offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeByte( std::string deviceId, std::string blockId, int nReg, int nByte, uint8 byte ) throw( std::string );

    /**
     * @brief readWord
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint16 readWord( std::string deviceId, std::string blockId, int nReg ) throw( std::string );

    /**
     * @brief writeWord
     * @param deviceId  -> device id
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param word      -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"        -> bad device id
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeWord( std::string deviceId, std::string blockId, int nReg, uint16 word ) throw( std::string );

    /**
     * @brief doWrite
     *
     * Calls all device's doWrite() function.
     */
    void doWrite();

    /**
     * @brief getAllDeviceId
     * @return all device id in vector
     */
    std::vector<std::string> getAllDeviceId();

    /**
     * @brief getAllBlockId
     * @param deviceId
     * @return all block id in a specified device
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    std::vector<std::string> getAllBlockId( std::string deviceId ) throw( std::string );

    /**
     * @brief readDeviceIp
     * @param deviceId
     * @return device ip address
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    std::string readDeviceIp( std::string deviceId ) throw( std::string );

    /**
     * @brief readDevicePort
     * @param deviceId
     * @return device port
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    int readDevicePort( std::string deviceId ) throw( std::string );

    /**
     * @brief readDeviceSlaveId
     * @param deviceId
     * @return device slave id
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    int readDeviceSlaveId( std::string deviceId ) throw( std::string );

    /**
     * @brief readDeviceResponseTimeout
     * @param deviceId
     * @return device response timeout
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    int readDeviceResponseTimeout( std::string deviceId ) throw( std::string );

    /**
     * @brief readDeviceConnectionTimeout
     * @param deviceId
     * @return device connection timeout
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    int readDeviceConnectionTimeout( std::string deviceId ) throw( std::string );

    /**
     * @brief readDeviceConnStatus
     * @param deviceId
     * @return device connection status
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device" -> bad device id
     */
    std::string readDeviceConnStatus( std::string deviceId ) throw( std::string );

    /**
     * @brief readBlockOffset
     * @param deviceId
     * @param blockId
     * @return block offset
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"    -> bad device id
     *      "bad_block"     -> bad block id
     */
    int readBlockOffset( std::string deviceId, std::string blockId ) throw( std::string );

    /**
     * @brief readBlockCount
     * @param deviceId
     * @param blockId
     * @return block count
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"    -> bad device id
     *      "bad_block"     -> bad block id
     */
    int readBlockCount( std::string deviceId, std::string blockId ) throw( std::string );

    /**
     * @brief readBlockCycleTime
     * @param deviceId
     * @param blockId
     * @return block cycle time
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"    -> bad device id
     *      "bad_block"     -> bad block id
     */
    int readBlockCycleTime( std::string deviceId, std::string blockId ) throw( std::string );

    /**
     * @brief readBlockRetries
     * @param deviceId
     * @param blockId
     * @return block retries
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"    -> bad device id
     *      "bad_block"     -> bad block id
     */
    int readBlockRetries( std::string deviceId, std::string blockId ) throw( std::string );

    /**
     * @brief readBlockError
     * @param deviceId
     * @param blockId
     * @return block error
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_device"    -> bad device id
     *      "bad_block"     -> bad block id
     */
    std::string readBlockError( std::string deviceId, std::string blockId ) throw( std::string );

};

}

#endif // MODBUSDRIVER_H
