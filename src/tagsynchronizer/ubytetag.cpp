#include "ubytetag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine {

UByteTag::UByteTag( int id,
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
    this->type = "ubyte";
    this->validity = "valid";
}

void UByteTag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
         // get data from interface
        int _value = interface->readByte( deviceId, blockId, address, subAddress );

        // multiple and add operations before set this->value
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        int _add = Conversion::convert<std::string,int>( this->add );
        _value *= _multiple;
        _value += _add;

        // set limits
        if( _value > 255 )
        {
            _value = 255;
        }

        if( _value < 0 )
        {
            _value = 0;
        }

        // refresh value
        this->value = Conversion::convert<int,std::string>( _value );

        // set validity flag
        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

void UByteTag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get value to numeric data
        int _value = Conversion::convert<std::string,int>( this->value );

        // if the input is a string we returns...
        if( _value == 0 && this->value != "0" )
        {
            return;
        }

        // add and multiple reverse operations
        int _add = Conversion::convert<std::string,int>( this->add );
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        _value = _value - _add;
        _value = _value / _multiple;

        // set limits
        if( _value > 255 )
        {
            _value = 255;
        }

        if( _value < 0 )
        {
            _value = 0;
        }

        // write to modbus driver
        interface->writeByte( deviceId, blockId, address, subAddress, _value );

        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

}
