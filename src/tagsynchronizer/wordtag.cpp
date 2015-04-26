#include "wordtag.h"
#include "../Core/conversion.hpp"


namespace ModbusEngine {

WordTag::WordTag( int id,
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
    this->type = "word";
    this->validity = "valid";
}

void WordTag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get data from interface
        int _value = interface->readWord( deviceId, blockId, address );

        // format to signed value
        if( _value > 32767 )
        {
            _value = 32767 - _value;
        }

        // multiple and add operations before set this->value
        int _multiple = Conversion::convert<std::string,int>( this->multiple );
        int _add = Conversion::convert<std::string,int>( this->add );
        _value *= _multiple;
        _value += _add;

        // set limits
        if( _value > 32767 )
        {
            _value = 32767;
        }

        if( _value < -32768 )
        {
            _value = -32768;
        }

        // refresh value
        this->value = Conversion::convert<int,std::string>( _value );

        // set validity flag
        validity = "valid";
    }
    catch( std::string _ex )
    {
        validity = _ex;
        this->value = "#";
    }
}

void WordTag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
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
        _value /= _multiple;

        // set limits
        if( _value < -32768 )
        {
            _value = -32768;
        }

        if( _value > 32767 )
        {
            _value = 32767;
        }

        // format to unsigned value for the modbus driver
        if( _value < 0 )
        {
            _value = 32767 - _value;
        }

        // write to modbus driver
        interface->writeWord( deviceId, blockId, address, _value );

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
