#ifndef MODBUSDEVICE_H
#define MODBUSDEVICE_H

#include <map>

#include "modbusblock.h"

namespace ModbusEngine
{

/**
 * @brief The ModbusDevice class
 *
 * Define the modbus device.
 *
 * Features:
 *   - store ModbusBlocks in map
 *   - store connection
 *   - multithread design
 *   - build functions for add ModbusBlock
 */

class ModbusDevice
{

private:
    /// Device main parameters
    std::string id;
    std::string ip;
    int port;
    int slaveId;
    int responseTimeout;
    int connectionTimeout;

    /// The modbus tcp connection
    MBTCPMasterConnection* conn;

    /// Map when we store modbus blocks
    std::map<std::string,ModbusBlock*> blocks;

    /// required mutexes for multithreading support
    std::mutex connMutex;
    std::mutex deviceMutex;

public:
    /**
     * @brief ModbusDevice
     * @param id                -> uinque std::string id
     * @param ip                -> ip address of device
     * @param port              -> port of device
     * @param slaveId           -> slaveId of device
     * @param responseTimeout   -> timeout for response
     * @param connectionTimeout -> timeout for connection
     *
     * Creates the full object but not fill this->blocks map.
     */
    ModbusDevice( std::string id,
                  std::string ip,
                  int port,
                  int slaveId,
                  int responseTimeout,
                  int connectionTimeout );

    /**
      * @brief ~ModbusDevice
      *
      * Deletes connection.
      */
    ~ModbusDevice();

    /**
     * @brief addModbusBlock
     * @param blockId   -> block id :-)
     * @param block     -> the block :-)
     *
     * Adds block to this->blocks map.
     */
    void addModbusBlock( std::string blockId, ModbusBlock* block );

    /**
     * @brief delegateConnection
     * @return pointer to this->conn
     */
    MBTCPMasterConnection* delegateConnection();

    /**
     * @brief delegateConnectionMutex
     * @return pointer to this->connMutex.
     */
    std::mutex* delegateConnectionMutex();

    /**
     * @brief startBlockThreads
     *
     * Starts all of blocks in this->blocks map.
     */
    void startBlockThreads();

    /**
     * @brief readBit
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nBit      -> bit position (bit offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    bool readBit( std::string blockId, int nReg, int nBit ) throw( std::string );

    /**
     * @brief writeBit
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nBit      -> bit position (bit offset inside register)
     * @param bit       -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    void writeBit( std::string blockId, int nReg, int nBit, bool bit ) throw( std::string );

    /**
     * @brief readByte
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nByte     -> byte position (byte offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint8 readByte( std::string blockId, int nReg, int nByte ) throw( std::string );

    /**
     * @brief writeByte
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param nByte     -> byte position (byte offset inside register)
     * @param byte      -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeByte( std::string blockId, int nReg, int nByte, uint8 byte ) throw( std::string );

    /**
     * @brief readWord
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint16 readWord( std::string blockId, int nReg ) throw( std::string );

    /**
     * @brief writeWord
     * @param blockId   -> block id
     * @param nReg      -> register position (offset)
     * @param word      -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block"         -> bad block id
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeWord( std::string blockId, int nReg, uint16 word ) throw( std::string );

    /**
     * @brief doWrite
     *
     * Calls all blocks doWrite() function.
     */
    void doWrite();

    /**
     * @brief readId
     * @return device id
     */
    std::string readId();

    /**
     * @brief readIp
     * @return device ip
     */
    std::string readIp();

    /**
     * @brief readPort
     * @return device port
     */
    int readPort();

    /**
     * @brief readSlaveId
     * @return device slaveid
     */
    int readSlaveId();

    /**
     * @brief readResponseTimeout
     * @return device timeout
     */
    int readResponseTimeout();

    /**
     * @brief readConnectionTimeout
     * @return device connection timeout
     */
    int readConnectionTimeout();

    /**
     * @brief readConnStatus
     * @return device conn status
     */
    std::string readConnStatus();

    /**
     * @brief getAllBlockId
     * @return device all block id
     */
    std::vector<std::string> getAllBlockId();

    /**
     * @brief readBlockId
     * @param blockId -> block id
     * @return the block id
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    std::string readBlockId( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockOffset
     * @param blockId -> block id
     * @return block offset
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    int readBlockOffset( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockCount
     * @param blockId -> block id
     * @return block count
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    int readBlockCount( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockCycleTime
     * @param blockId -> block id
     * @return block cycletime
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    int readBlockCycleTime( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockRetries
     * @param blockId -> block id
     * @return block retries
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    int readBlockRetries( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockErrorSleep
     * @param blockId -> block id
     * @return block error sleep
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    int readBlockErrorSleep( std::string blockId ) throw( std::string );

    /**
     * @brief readBlockError
     * @param blockId -> block id
     * @return block error status
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_block" -> bad block id
     */
    std::string readBlockError( std::string blockId ) throw( std::string );

};

} // namespace ModbusEngine

#endif // MODBUSDEVICE_H
