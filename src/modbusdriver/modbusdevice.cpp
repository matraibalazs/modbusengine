#include <utility>

#include "modbusdevice.h"

namespace ModbusEngine
{

ModbusDevice::ModbusDevice( std::string id,
                            std::string ip,
                            int port,
                            int slaveId,
                            int responseTimeout,
                            int connectionTimeout )
{
    this->id = id;
    this->ip = ip;
    this->port = port;
    this->slaveId = slaveId;
    this->responseTimeout = responseTimeout;
    this->connectionTimeout = connectionTimeout;
    this->conn =  new MBTCPMasterConnection( ip, port, slaveId, responseTimeout, connectionTimeout );
}

ModbusDevice::~ModbusDevice()
{
    delete this->conn;
}

void ModbusDevice::addModbusBlock( std::string blockId, ModbusBlock* block )
{
    this->blocks[ blockId ] = block;
}

MBTCPMasterConnection* ModbusDevice::delegateConnection()
{
    return this->conn;
}

std::mutex* ModbusDevice::delegateConnectionMutex()
{
    return &(this->connMutex);
}

void ModbusDevice::startBlockThreads()
{
    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.begin();
    for( ; _it != this->blocks.end(); _it++ )
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;
        _b->startThread();
    }
}

bool ModbusDevice::readBit( std::string blockId, int nReg, int nBit ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readBit( nReg, nBit );
    }
}

void ModbusDevice::writeBit( std::string blockId, int nReg, int nBit, bool bit ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        _b->writeBit( nReg, nBit, bit );
    }
}

uint8 ModbusDevice::readByte( std::string blockId, int nReg, int nByte ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readByte( nReg, nByte );
    }
}

void ModbusDevice::writeByte( std::string blockId, int nReg, int nByte, uint8 byte ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        _b->writeByte( nReg, nByte, byte );
    }
}

uint16 ModbusDevice::readWord( std::string blockId, int nReg ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readWord( nReg );
    }
}

void ModbusDevice::writeWord( std::string blockId, int nReg, uint16 word ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        _b->writeWord( nReg, word );
    }
}

void ModbusDevice::doWrite()
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.begin();
    for( ; _it != this->blocks.end(); _it++ )
    {
        std::pair<std::string,ModbusBlock*> p = *_it;
        ModbusBlock* b = p.second;
        b->doWrite();
    }

    this->deviceMutex.unlock();
}

std::string ModbusDevice::readId()
{
    this->deviceMutex.lock();
    std::string _r = this->id;
    this->deviceMutex.unlock();

    return _r;
}

std::string ModbusDevice::readIp()
{
    this->deviceMutex.lock();
    std::string _r = this->ip;
    this->deviceMutex.unlock();

    return _r;
}

int ModbusDevice::readPort()
{
    this->deviceMutex.lock();
    int _r = this->port;
    this->deviceMutex.unlock();

    return _r;
}

int ModbusDevice::readSlaveId()
{
    this->deviceMutex.lock();
    int _r = this->slaveId;
    this->deviceMutex.unlock();

    return _r;
}

int ModbusDevice::readResponseTimeout()
{
    this->deviceMutex.lock();
    int _r = this->responseTimeout;
    this->deviceMutex.unlock();

    return _r;
}

int ModbusDevice::readConnectionTimeout()
{
    this->deviceMutex.lock();
    int _r = this->connectionTimeout;
    this->deviceMutex.unlock();

    return _r;
}

std::string ModbusDevice::readConnStatus()
{
    this->deviceMutex.lock();

    if( this->conn->isConnected() )
    {
        this->deviceMutex.unlock();
        return std::string( "connected" );
    }
    else
    {
        this->deviceMutex.unlock();
        return std::string( "disconnected" );
    }
}

std::vector<std::string> ModbusDevice::getAllBlockId()
{
    this->deviceMutex.lock();

    std::vector<std::string> _block_ids;

    // get all block id
    for( std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.begin();
        _it != this->blocks.end(); _it++ ) {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        _block_ids.push_back( _p.first );
    }

    this->deviceMutex.unlock();

    return _block_ids;
}

std::string ModbusDevice::readBlockId( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readId();
    }
}

int ModbusDevice::readBlockOffset( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readOffset();
    }
}

int ModbusDevice::readBlockCount( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readCount();
    }
}

int ModbusDevice::readBlockCycleTime( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readCycleTime();
    }
}

int ModbusDevice::readBlockRetries( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readRetries();
    }
}

int ModbusDevice::readBlockErrorSleep( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readErrorSleep();
    }
}

std::string ModbusDevice::readBlockError( std::string blockId ) throw( std::string )
{
    this->deviceMutex.lock();

    std::map<std::string,ModbusBlock*>::iterator _it = this->blocks.find( blockId );

    if( _it == this->blocks.end() )
    {
        this->deviceMutex.unlock();
        throw std::string( "bad_block" );
    }
    else
    {
        std::pair<std::string,ModbusBlock*> _p = *_it;
        ModbusBlock* _b = _p.second;

        this->deviceMutex.unlock();
        return _b->readError();
    }
}

} // namespace ModbusEngine
