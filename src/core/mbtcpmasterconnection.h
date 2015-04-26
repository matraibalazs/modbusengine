#ifndef MBTCPMASTERCONNECTION_H
#define MBTCPMASTERCONNECTION_H

#include <modbus.h>
#include <string>
#include <vector>

#include "types.h"

namespace ModbusEngine
{

/**
 * @brief The MBTCPMasterConnection class
 *
 * Represents a modbus TCP master connection with master operations:
 *   - connecting to server
 *   - disconnect from server
 *   - flush
 *   - Modbus Read Holding Registers - FC 0x03
 *   - Write Multiple Registers - FC 0x10
 */

class MBTCPMasterConnection
{

private:
    /// main parameters
    std::string ip;
    int port;
    int slaveId;
    int responseTimeout;
    int connectionTimeout;

    /// connection status indicator
    bool connected;

    /// the context delivered by libmodbus library
    modbus_t* context;

public:

    /**
     * @brief MBTCPMasterConnection
     * @param ip                    -> device ip
     * @param port                  -> device port
     * @param slaveId               -> device slaveId
     * @param responseTimeout       -> the timeout of the modbus question in millisecs
     * @param connectionTimeout     -> the connecting timeout in millisecs
     *
     * The constructor creates the full object.
     * The constructor throws std::string exceptions when error happens:
     *
     *      "no_enough_memory" -> no enough memory for allocate object :-)
     */
    MBTCPMasterConnection( std::string ip,
                           int port,
                           int slaveId,
                           int responseTimeout,
                           int connectionTimeout ) throw( std::string );

    /**
      * @brief ~MBTCPMasterConnection
      *
      * Destructs context object.
      */
    ~MBTCPMasterConnection();

    /**
     * @brief connect
     *
     * Connects to a modbus device.
     * The function throws std::string exception when error happens:
     *
     *      "host_not_reachable"    -> host not reachable on network
     *      "connection_failed"     -> connecting failed
     */
    void connect() throw( std::string );

    /**
     * @brief disconnect
     *
     * Closes the connection.
     */
    void disconnect();

    /**
     * @brief flush
     *
     * Flush the context.
     */
    void flush();

    /**
     * @brief isConnected
     * @return the connection status
     */
    bool isConnected();

    /**
     * @brief MBTCPMasterConnection::readHoldingRegisters
     * @param offset    -> the modbus register offset
     * @param count     -> number of registers
     * @return the vector of the registers
     *
     * The function uses the FC 0x03 for reading.
     * The function throws std::string exception when error happens:
     *
     *      "illegal_function_code"     -> see modbus protocol definition
     *      "illegal_data_address"      -> see modbus protocol definition
     *      "illegal_data_value"        -> see modbus protocol definition
     *      "server_fail"               -> see modbus protocol definition
     *      "error_ack"                 -> see modbus protocol definition
     *      "server_busy"               -> see modbus protocol definition
     *      "gateway_path_exception"    -> see modbus protocol definition
     *      "gateway_respond_exception" -> see modbus protocol definition
     *      "too_many_data"             -> see modbus protocol definition
     *      "undefined_exception"       -> not modbus defined exception
     */
    std::vector<uint16> readHoldingRegisters( int offset,
                                              int count ) throw( std::string );

    /**
     * @brief MBTCPMasterConnection::writeMultipleRegisters
     * @param offset    -> the modbus register offset
     * @param count     -> number of registers
     * @param values    -> the registers we want to write
     *
     * The function uses the FC 0x10 for writing.
     * The function throws exception when error happens:
     *
     *      "illegal_function_code"     -> see modbus protocol definition
     *      "illegal_data_address"      -> see modbus protocol definition
     *      "illegal_data_value"        -> see modbus protocol definition
     *      "server_fail"               -> see modbus protocol definition
     *      "error_ack"                 -> see modbus protocol definition
     *      "server_busy"               -> see modbus protocol definition
     *      "gateway_path_exception"    -> see modbus protocol definition
     *      "gateway_respond_exception" -> see modbus protocol definition
     *      "too_many_data"             -> see modbus protocol definition
     *      "undefined_exception"       -> not modbus defined exception
     */
    void writeMultipleRegisters( int offset,
                                 int count,
                                 std::vector<uint16> values ) throw( std::string );

};

} //namespace ModbusEngine

#endif // MBTCPMASTERCONNECTION_H
