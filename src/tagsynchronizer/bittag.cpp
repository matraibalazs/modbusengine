#include "bittag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine
{

BitTag::BitTag( int id,
                std::string name,
                std::string deviceId,
                std::string blockId,
                int address,
                int subAddress,
                std::string multiple,
                std::string add,
                bool wordSwap,
                int divider ) : Tag( id, name, deviceId, blockId, address, subAddress, multiple, add, wordSwap, divider )
{
    this->value = "0";
    this->type = "bit";
    this->validity = "valid";
}

void BitTag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get data from modbus driver
        int _value = interface->readBit( deviceId, blockId, address, subAddress );

        // convert data to string
        this->value = Conversion::convert<int,std::string>( _value );

        // set validity
        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";

    }
}

void BitTag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get own data to numeric value
        int _value = Conversion::convert<std::string, int>( this->value );

        // if the input is a string we returns...
        if( _value == 0 && this->value != "0" )
        {
            return;
        }

        // limits
        if( _value < 0 )
        {
            _value = 0;
        }

        if( _value > 1 )
        {
            _value = 1;
        }

        // write to modbus driver
        interface->writeBit( deviceId, blockId, address, subAddress, _value );

        // set validity
        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

}
