#include "udwordtag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine {

UDWordTag::UDWordTag( int id,
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
    this->type = "udword";
    this->validity = "valid";
}

void UDWordTag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
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

        unsigned long long int _value = Conversion::convert<uint32,unsigned long long int>( (uint32)(_v_1*65536 + _v_2) );

        // multiple and add operations before set this->value
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        int _add = Conversion::convert<std::string,int>( this->add );
        _value *= _multiple;
        _value += _add;

        if( _value > 4294967295 )
        {
            _value = 4294967295;
        }

        // refresh value
        this->value = Conversion::convert<unsigned long long int,std::string>( _value );

        // set validity flag
        this-> validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

void UDWordTag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get value to numeric data
        long long int _value = Conversion::convert<std::string,long long int>( this->value );

        // if the input is a string we returns...
        if( _value == 0 && this->value != "0" )
        {
            return;
        }

        // add and multiple reverse operations
        uint32 _multiple = Conversion::convert<std::string,uint32>( this->multiple );
        uint32 _add = Conversion::convert<std::string,uint32>( this->add );
        _value -= _add;
        _value /= _multiple;

        // set limits
        if( _value < 0 )
        {
            _value = 0;
        }

        if( _value > 4294967295 )
        {
            _value = 4294967295;
        }

        // write to modbus driver
        uint16 _v_1 = _value/65536;
        uint16 _v_2 = _value%65536;

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
