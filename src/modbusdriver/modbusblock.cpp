#include "modbusblock.h"

#include <iostream>

namespace ModbusEngine {

ModbusBlock::ModbusBlock( std::string id,
                          MBTCPMasterConnection* conn,
                          std::mutex* connMutex,
                          int offset,
                          int count,
                          int cycleTime,
                          int retries,
                          int errorSleep )
{
    this->id = id;
    this->conn = conn;
    this->connMutex = connMutex;
    this->offset = offset;
    this->count = count;
    this->cycleTime = cycleTime;
    this->retries = retries;
    this->errorSleep = errorSleep;
    this->master = false;
    this->writeFlag = false;
    this->writeReq = false;
    this->error = "error_init";

    for( int i = 0; i < count; i++ )
    {
        this->readList.push_back( 0 );
    }
}

bool ModbusBlock::read()
{
    /// Connecting...
    if( !this->conn->isConnected() )
    {
        if( !this->master )
        {
            this->error = "host_not_reachable";
            return false;
        }

        try
        {
            this->blockMutex.unlock();
            this->conn->connect();
            this->blockMutex.lock();
            this->master = false;
        }
        catch( std::string ex )
        {
            this->blockMutex.lock();
            this->error = ex;
            return false;
        }
    }

    /// Reading...
    try
    {
        this->readList = this->conn->readHoldingRegisters( this->offset, this->count );
        this->error = "no_error";
    }
    catch ( std::string ex )
    {
        this->error = ex;
        if( this->error == "server_fail" ||
            this->error == "gateway_path_exception" ||
            this->error == "gateway_respond_exception" ||
            this->error == "undefined_exception" )
        {
            this->conn->disconnect();
            this->master = true;
        }

        return false;
    }

    return true;
}

bool ModbusBlock::write()
{
    /// Connecting...
    if( !this->conn->isConnected() )
    {
        if( !this->master )
        {
            this->error = "host_not_reachable";
            return false;
        }

        try
        {
            this->blockMutex.unlock();
            this->conn->connect();
            this->blockMutex.lock();
            this->master = false;
        }
        catch( std::string ex )
        {
            this->blockMutex.lock();
            this->error = ex;
            return false;
        }
    }

    /// Merge & write...
    try
    {
        if( !this->writeReq ) return true;

        this->merge();
        this->conn->writeMultipleRegisters( this->offset, this->count, this->readList );
        this->error = "no_error";
        this->writeList.clear();
        this->writeReq = false;
    }
    catch ( std::string ex )
    {
        this->error = ex;
        if( this->error == "server_fail" ||
            this->error == "gateway_path_exception" ||
            this->error == "gateway_respond_exception" ||
            this->error == "undefined_exception" )
        {
            this->conn->disconnect();
            this->master = true;
        }

        return false;
    }

    return true;
}

void ModbusBlock::merge()
{
    for( std::vector<DataItem>::iterator _it = writeList.begin();
         _it != this->writeList.end(); _it++ )
    {
        DataItem _d = *_it;

        if( _d.type == ITEM_TYPE_BIT )
        {
            uint16 _pow2[] = { 1, 2, 4, 8, 16, 32, 64, 128,
                          256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };

            if( _d.value )
            {
                uint16 _mask = _pow2[ _d.subAddress ];
                this->readList[ _d.address ] = this->readList[ _d.address ] | _mask;
            }
            else
            {
                uint16 _mask = 65535 - _pow2[ _d.subAddress ];
                readList[ _d.address ] = readList[ _d.address ] & _mask;
            }

        }
        else if( _d.type == ITEM_TYPE_BYTE )
        {
            uint8 _lowerByte = (uint8)( this->readList[ _d.address ] & 0xff );
            uint8 _upperByte = (uint8)( this->readList[ _d.address ] >> 0x08 );

            if ( _d.subAddress == 0 )
            {
                _lowerByte = (uint8)_d.value;
            } else
            {
                _upperByte = (uint8)_d.value;
            }

            this->readList[ _d.address ] = (uint16)( (uint16)( _upperByte << 0x08 ) +
                                              (uint16)_lowerByte );

        }
        else if( _d.type == ITEM_TYPE_WORD )
        {
            this->readList[ _d.address ] = _d.value;
        }
    }
}

void ModbusBlock::setMaster()
{
    this->master = true;
}

void ModbusBlock::run()
{
    int _time_block = 50;       /// normal sleep time
    int _time;                  /// time counter
    bool _read_ok = false;      /// local communication ok flag for read
    bool _write_ok = false;     /// local communication ok flag for write
    int _rsum = 0;              /// summary retries for write
    bool read_flag = true;      /// read flag

    while( true )
    {
        /// Lock the block :-)
        this->blockMutex.lock();

        /// Writing mechanism...
        if( this->writeFlag )
        {
            this->connMutex->lock();
            _write_ok = this->write();
            this->connMutex->unlock();
            if( _write_ok )
            {
                read_flag = true;
                this->writeFlag = false;
                _rsum = 0;
            }
            else if( _rsum <= this->retries )
            {
                _rsum++;
                this->blockMutex.unlock();
                Thread::msleep( this->errorSleep );
                this->blockMutex.lock();
            }
            else
            {
                _rsum = 0;
                this->writeFlag = false;
                this->writeList.clear();
            }
        }

       /// Reading mechanism
        if( read_flag )
        {
            this->connMutex->lock();
            _read_ok = this->read();
            this->connMutex->unlock();
            _time = 0;
            if( _read_ok )
            {
                read_flag = false;
            }
            else
            {
                this->blockMutex.unlock();
                Thread::msleep( this->errorSleep );
                this->blockMutex.lock();
            }
        }

        /// Inner trigger time calculation...
        if( ( _time >= this->cycleTime ) && _read_ok && ( this->cycleTime > 0 ) )
        {
            read_flag = true;
        }
        else
        {
            _time = _time + _time_block;
        }

        this->blockMutex.unlock();
        Thread::msleep( _time_block );
    }
} // run()

void ModbusBlock::halt(){}

bool ModbusBlock::readBit( int nReg, int nBit ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( nBit > 15 )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_bit_number" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    uint16 _pow2[] = { 1, 2, 4, 8, 16, 32, 64, 128,
                      256, 512, 1024, 2048, 4096, 8192, 16384, 32768 };

    uint16 _word = this->readList[ nReg ];
    uint16 _mask = _pow2[ nBit ];

    if( ( _word & _mask ) > 0 )
    {
        this->blockMutex.unlock();
        return 1;
    }

    this->blockMutex.unlock();
    return 0;
}

void ModbusBlock::writeBit( int nReg, int nBit, bool bit ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( nBit > 15 )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_bit_number" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    DataItem _d;
    _d.type = this->ITEM_TYPE_BIT;
    _d.address = nReg;
    _d.subAddress = nBit;
    _d.value = bit;

    this->writeList.push_back( _d );

    this->writeReq = true;

    this->blockMutex.unlock();
}

uint8 ModbusBlock::readByte( int nReg, int nByte ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    uint16 _word = this->readList[ nReg ];
    uint8 _upper_byte = _word >> 0x08;
    uint8 _lower_byte = _word & 0xff;

    if( nByte == 0 )
    {
        this->blockMutex.unlock();
        return _lower_byte;
    }
    else
    {
        this->blockMutex.unlock();
        return _upper_byte;
    }
}

void ModbusBlock::writeByte( int nReg, int nByte, uint8 byte ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    DataItem _d;
    _d.type = this->ITEM_TYPE_BYTE;
    _d.address = nReg;
    _d.subAddress = nByte;
    _d.value = byte;

    this->writeList.push_back( _d );

    this->writeReq = true;

    this->blockMutex.unlock();
}

uint16 ModbusBlock::readWord( int nReg ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    uint16 _r = this->readList[ nReg ];

    this->blockMutex.unlock();

    return _r;
}

void ModbusBlock::writeWord( int nReg, uint16 word ) throw( std::string )
{
    this->blockMutex.lock();

    if( nReg >= this->count )
    {
        this->blockMutex.unlock();
        throw std::string( "bad_register" );
    }

    if( this->error != "no_error" )
    {
        this->blockMutex.unlock();
        throw std::string( "block_error" );
    }

    DataItem _d;
    _d.type = this->ITEM_TYPE_WORD;
    _d.address = nReg;
    _d.subAddress = 0;
    _d.value = word;

    this->writeList.push_back( _d );

    this->writeReq = true;

    this->blockMutex.unlock();
}

void ModbusBlock::doWrite()
{
    this->blockMutex.lock();
    this->writeFlag = true;
    this->blockMutex.unlock();;
}

std::string ModbusBlock::readId()
{
    this->blockMutex.lock();
    std::string _r = this->id;
    this->blockMutex.unlock();

    return _r;
}

int ModbusBlock::readOffset()
{
    this->blockMutex.lock();
    int _r = this->offset;
    this->blockMutex.unlock();

    return _r;
}

int ModbusBlock::readCount()
{
    this->blockMutex.lock();
    int _r = this->count;
    this->blockMutex.unlock();

    return _r;
}

int ModbusBlock::readCycleTime()
{
    this->blockMutex.lock();
    int _r = this->cycleTime;
    this->blockMutex.unlock();

    return _r;
}

int ModbusBlock::readRetries()
{
    this->blockMutex.lock();
    int _r = this->retries;
    this->blockMutex.unlock();

    return _r;
}

int ModbusBlock::readErrorSleep()
{
    this->blockMutex.lock();
    int _r = this->errorSleep;
    this->blockMutex.unlock();

    return _r;
}

std::string ModbusBlock::readError()
{
    this->blockMutex.lock();
    std::string _r = this->error;
    this->blockMutex.unlock();

    return _r;
}

} // namespace ModbusEngine
