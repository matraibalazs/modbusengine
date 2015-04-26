#include "real16tag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine
{

Real16Tag::Real16Tag( int id,
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
    this->value = "0.0";
    this->type = "real16";
    this->validity = "valid";
}

void Real16Tag::readValueFromModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get data from interface
        uint16 _v = interface->readWord( deviceId, blockId, address );
        float _value = 0.0;

        // format to signed value
        if( _v > 32767 )
        {
            _value = (float)(32767 - _v);
        }
        else
        {
            _value = (float)_v;
        }

        // Convert to fake float
        _value = (float)(_value / (float)this->divider);

        // multiple and add operations before set this->value
        float _multiple = Conversion::convert<std::string,float>( this->multiple );
        float _add = Conversion::convert<std::string,float>( this->add );
        _value *= _multiple;
        _value += _add;

        // calculate precision
        int precision;
        if( this->divider == 10 )
        {
            precision = 1;
        }
        else if( this->divider == 100 )
        {
            precision = 2;
        }
        else if( this->divider == 1000 )
        {
            precision = 3;
        }
        else if( this->divider == 10000 )
        {
            precision = 4;
        }

        // refresh value
        this->value = Conversion::toString( _value, precision );

        // set validity flag
        this->validity = "valid";
    }
    catch( std::string _ex )
    {
        this->validity = _ex;
        this->value = "#";
    }
}

void Real16Tag::writeValueToModbusDriver( ModbusDriverDataInterface* interface )
{
    try
    {
        // get value to numeric data
        float _value = Conversion::convert<std::string,float>( this->value );

        // if the input is a string we returns...
        if( _value == 0 && this->value != "0" )
        {
            return;
        }

        // add and multiple reverse operations
        float _fv = 0.0;
        float _multiple = Conversion::convert<std::string,float>( this->multiple );
        float _add = Conversion::convert<std::string,float>( this->add );
        _fv = _value - _add;
        _fv /= _multiple;

        // set limits
        if( this->divider == 10 )
        {
            if( _fv < -3276.8 )
            {
                _fv = -3276.8;
            }

            if( _fv > 3276.7 )
            {
                _fv = 3276.7;
            }
        }
        else if( this->divider == 100 )
        {
            if( _fv < -327.68 )
            {
                _fv = -327.68;
            }

            if( _fv > 327.67 )
            {
                _fv = 327.67;
            }
        }
        else if( this->divider == 1000 )
        {
            if( _fv < -32.768 )
            {
                _fv = -32.768;
            }

            if( _fv > 32.767 )
            {
                _fv = 32.767;
            }
        }
        else if( this->divider == 10000 )
        {
            if( _fv < -3.2768 )
            {
                _fv = -3.2768;
            }

            if( _fv > 3.2767 )
            {
                _fv = 3.2767;
            }
        }

        // format to unsigned value for the modbus driver
        int16 _v = 0;
        uint16 _uv = 0;

        _fv = (float)(_fv*(float)this->divider);

        _v = (int16)_fv;

        if( _v < 0 ) {
            _uv = 32767 - _v;
        } else {
            _uv = _v;
        }

        // write to modbus driver
        interface->writeWord( deviceId, blockId, address, _uv );

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
