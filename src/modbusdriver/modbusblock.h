#ifndef MODBUSBLOCK_H
#define MODBUSBLOCK_H

#include <list>
#include <mutex>

#include "../Core/mbtcpmasterconnection.h"
#include "../Core/thread.hpp"

namespace ModbusEngine
{

/**
 * @brief The ModbusBlock class
 *
 * Define a modbus block.
 *
 * Features:
 *   - automatic loop working ( write-read-wait )
 *   - read from modbus device to readList via MBTCPMasterConnection
 *   - write to modbus device from writeList via MBTCPMasterConnection
 *   - full multithread design
 *   - error monitor flags
 *   - data interface for read and write data
 *
 * DO NOT ADD MORE DATATYPE SUPPORT HERE. IT'S A FUNDAMENTAL DESIGN IDEA.
 */

class ModbusBlock : public Thread
{

private:
    /// defined class constants
    static const int ITEM_TYPE_BIT = 0;
    static const int ITEM_TYPE_BYTE = 1;
    static const int ITEM_TYPE_WORD = 2;

    /// DataItem object for writing
    class DataItem
    {
    public:
        int type;
        int address;
        int subAddress;
        int value;
    };

    /// block main parameters
    std::string id;
    int offset;
    int count;
    int cycleTime;
    int retries;
    int errorSleep;
    std::string error;

    /// master is a status (the master tries to connect to device)
    bool master;
    /// this flag indicates the write-request (setted by doWrite() function )
    bool writeFlag;
    /// this flag indicates when the block has a writing process and we must to write the changes
    bool writeReq;

    /// the modbus connection (by device)
    MBTCPMasterConnection* conn;

    /// read and write lists (contains registers and DataItem objects)
    std::vector<uint16> readList;
    std::vector<DataItem> writeList;

    /// required mutexes for multi threading support
    std::mutex* connMutex;
    std::mutex blockMutex;

    /**
     * @brief read
     * @return the success of reading
     *
     * Reads the block defined registers.
     */
    bool read();

    /**
     * @brief write
     * @return the success of writing
     *
     * Writes the registers.
     */
    bool write();

    /**
     * @brief merge
     *
     * Merge DataItem objects into readList before writing.
     */
    void merge();

public:
    /**
     * @brief ModbusBlock
     * @param id            -> unique std::string id
     * @param conn          -> delivered connection
     * @param connMutex     -> delivered mutex for connection
     * @param offset        -> modbus question offset
     * @param count         -> modbus question count
     * @param cycleTime     -> reading cycletime
     * @param retries       -> retries after unsuccesfully writing
     * @param errorSleep    -> sleep after unsuccessfully reading/writing
     *
     * Creates the full object.
     */
    ModbusBlock( std::string id,
                 MBTCPMasterConnection* conn,
                 std::mutex* connMutex,
                 int offset,
                 int count,
                 int cycleTime,
                 int retries,
                 int errorSleep );

    /**
     * @brief run
     *
     * Thread class defined function. Called by startThread().
     */
    void run();

    /**
     * @brief halt
     *
     * Thread class defined function. Called by stopThread().
     */
    void halt();

    /**
     * @brief setMaster
     *
     * Sets the master flag.
     */
    void setMaster();

    /**
     * @brief readBit
     * @param nReg -> register position (offset)
     * @param nBit -> bit position (bit offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    bool readBit( int nReg, int nBit ) throw( std::string );

    /**
     * @brief writeBit
     * @param nReg -> register position (offset)
     * @param nBit -> bit position (bit offset inside register)
     * @param bit  -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "bad_bit_number"    -> bad bit address
     *      "block_error"       -> block communication error
     */
    void writeBit( int nReg, int nBit, bool bit ) throw( std::string );

    /**
     * @brief readByte
     * @param nReg  -> register position (offset)
     * @param nByte -> byte position (byte offset inside register)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint8 readByte( int nReg, int nByte ) throw( std::string );

    /**
     * @brief writeByte
     * @param nReg  -> register position (offset)
     * @param nByte -> byte position (byte offset inside register)
     * @param byte  -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeByte( int nReg, int nByte, uint8 byte ) throw( std::string );

    /**
     * @brief readWord
     * @param nReg -> register position (offset)
     * @return the stored value
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    uint16 readWord( int nReg ) throw( std::string );

    /**
     * @brief writeWord
     * @param nReg -> register position (offset)
     * @param word -> the value to write
     *
     * The function throws std::string exception when error happens:
     *
     *      "bad_register"      -> bad register address
     *      "block_error"       -> block communication error
     */
    void writeWord( int nReg, uint16 word ) throw( std::string );

    /**
     * @brief doWrite
     *
     * Sets the writeFlag.
     */
    void doWrite();

    /**
     * @brief readId
     * @return block id
     */
    std::string readId();

    /**
     * @brief readOffset
     * @return block offset
     */
    int readOffset();

    /**
     * @brief readCount
     * @return block count
     */
    int readCount();

    /**
     * @brief readCycleTime
     * @return block cycletime
     */
    int readCycleTime();

    /**
     * @brief readRetries
     * @return block retries
     */
    int readRetries();

    /**
     * @brief readErrorSleep
     * @return block error sleep
     */
    int readErrorSleep();

    /**
     * @brief readError
     * @return block error status
     */
    std::string readError();

};

} // namespace ModbusEngine

#endif // MODBUSBLOCK_H
