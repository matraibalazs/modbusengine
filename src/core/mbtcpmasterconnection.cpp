#include <cerrno>
#include <stdint.h>

#include "mbtcpmasterconnection.h"
#include "networktester.hpp"

namespace ModbusEngine
{

MBTCPMasterConnection::MBTCPMasterConnection( std::string ip,
                                              int port,
                                              int slaveId,
                                              int responseTimeout,
                                              int connectionTimeout ) throw( std::string )
{
    this->ip = ip;
    this->port = port;
    this->slaveId = slaveId;
    this->responseTimeout = responseTimeout;
    this->connectionTimeout = connectionTimeout;
    this->connected = false;

    this->context = modbus_new_tcp( ip.c_str(), port );

    if( this->context == NULL )
    {
        throw std::string( "no_enough_memory" );
    }

    modbus_set_slave( this->context, this->slaveId );

    struct timeval _tv;
    _tv.tv_sec = this->responseTimeout/1000;
    _tv.tv_usec = (this->responseTimeout%1000)*1000;
    modbus_set_response_timeout( context, &_tv );
}

MBTCPMasterConnection::~MBTCPMasterConnection()
{
    modbus_free( this->context );
}

void MBTCPMasterConnection::connect() throw( std::string )
{
    if( !NetworkTester::ping( this->ip, this->connectionTimeout ) )
    {
        throw std::string( "host_not_reachable" );
    }

    if( modbus_connect( this->context ) == -1 )
    {
        throw std::string( "connection_failed" );
    }

    this->connected = true;
}

void MBTCPMasterConnection::disconnect()
{
    modbus_close( this->context );
    this->connected = false;
}

void MBTCPMasterConnection::flush()
{
    modbus_flush( this->context );
}

bool MBTCPMasterConnection::isConnected()
{
    return this->connected;
}

std::vector<uint16> MBTCPMasterConnection::readHoldingRegisters( int offset,
                                                                 int count )
                                                                 throw( std::string )
{
    std::vector<uint16> _values;
    uint16_t* _lib_std_values = new uint16_t[ count ];

    if( modbus_read_registers( this->context,
                               offset,
                               count,
                               _lib_std_values ) != -1 )
    {
        _values.resize( count );
        for( int i = 0; i < count; i++ )
        {
           _values[ i ] = (uint16)_lib_std_values[ i ];
        }

        delete[] _lib_std_values;
        return _values;
    }
    else
    {
        delete[] _lib_std_values;
        switch( errno )
        {
            case 112345679 :
                throw std::string( "illegal_function_code" );
                break;

            case 112345680 :
                throw std::string( "illegal_data_address" );
                break;

            case 112345681 :
                throw std::string( "illegal_data_value" );
                break;

            case 112345682 :
                throw std::string( "server_fail" );
                break;

            case 112345683 :
                throw std::string( "error_ack" );
                break;

            case 112345684 :
                throw std::string( "server_busy" );
                break;

            case 112345688 :
                throw std::string( "gateway_path_exception" );
                break;

            case 112345689 :
                throw std::string( "gateway_respond_exception" );
                break;

            case 112345694 :
                throw std::string( "too_many_data" );
                break;

            default :
                throw std::string( "undefined_exception" );
                break;
        }

        return _values;
    }

} // readHoldingRegister

void MBTCPMasterConnection::writeMultipleRegisters( int offset,
                                                    int count,
                                                    std::vector<uint16> values )
                                                    throw( std::string )
{
    uint16_t* _lib_std_values = new uint16_t[ count ];

    for( int i = 0; i < count; i++ )
    {
        _lib_std_values[ i ] = (uint16_t)values[ i ];
    }

    if( modbus_write_registers( this->context,
                                offset,
                                count,
                                _lib_std_values ) != -1 )
    {
        delete[] _lib_std_values;
    }
    else
    {
        delete[] _lib_std_values;
        switch( errno )
        {
            case 112345679 :
                throw std::string( "illegal_function_code" );
                break;

            case 112345680 :
                throw std::string( "illegal_data_address" );
                break;

            case 112345681 :
                throw std::string( "illegal_data_value" );
                break;

            case 112345682 :
                throw std::string( "server_fail" );
                break;

            case 112345683 :
                throw std::string( "error_ack" );
                break;

            case 112345684 :
                throw std::string( "server_busy" );
                break;

            case 112345688 :
                throw std::string( "gateway_path_exception" );
                break;

            case 112345689 :
                throw std::string( "gateway_respond_exception" );
                break;

            case 112345694 :
                throw std::string( "too_many_data" );
                break;

            default :
                throw std::string( "undefined_exception" );
                break;
        }
    }

} // writeMultipleRegisters


} // namespace ModbusEngine
