#include "dwordtag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine
{

DWordTag::DWordTag( int id,
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
    this->type = "dword";
    this->validity = "valid";
}

void DWordTag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get data from interface
        uint16 _v_1;
        uint16 _v_2;

        if( this->wordSwap )
        {
            _v_1 = interface->readWord( deviceId, blockId, address );
            _v_2 = interface->readWord( deviceId, blockId, address + 1 );
        }
        else
        {
            _v_1 = interface->readWord( deviceId, blockId, address + 1 );
            _v_2 = interface->readWord( deviceId, blockId, address );
        }

        uint32 _value = _v_1*65536 + _v_2;
        long long int __value = 0;

        // format to signed value
        if( _value > 2147483647 )
        {
            __value = 2147483647 - Conversion::convert<uint32,long long int>( _value );
        }
        else
        {
            __value = Conversion::convert<uint32,long long int>( _value );
        }

        // multiple and add operations before set this->value
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        int _add = Conversion::convert<std::string,int>( this->add );
        __value *= _multiple;
        __value += _add;

        // set limits
        if( __value < -2147483648 )
        {
            __value = -2147483648;
        }

        if( __value > 2147483647 )
        {
            __value = 2147483647;
        }

        // refresh value
        this->value = Conversion::convert<long long int,std::string>( __value );

        // set validity flag
        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

void DWordTag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get value to numeric data
        long long int _value = Conversion::convert<std::string,long long int>( this->value );

        // if the input is a string we return...
        if( _value == 0 && this->value != "0" )
        {
            return;
        }

        // add and multiple reverse operations
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        int _add = Conversion::convert<std::string,int>( this->add );
        _value = _value - _add;
        _value /= _multiple;

        // set limits
        if( _value < -2147483648 )
        {
            _value = -2147483648;
        }

        if( _value > 2147483647 )
        {
            _value = 2147483647;
        }

        // format to unsigned value for the modbus driver
        uint32 __value = 0;
        if( _value < 0 )
        {
            __value = 2147483647 - _value;
        }
        else
        {
            __value = _value;
        }

        // write to modbus driver
        uint16 _v_1 = __value/65536;
        uint16 _v_2 = __value%65536;

        if( this->wordSwap )
        {
            interface->writeWord( deviceId, blockId, address, _v_1 );
            interface->writeWord( deviceId, blockId, address + 1, _v_2 );
        }
        else
        {
            interface->writeWord( deviceId, blockId, address + 1, _v_1 );
            interface->writeWord( deviceId, blockId, address, _v_2 );
        }

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
