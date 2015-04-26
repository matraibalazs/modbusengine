#include "modbusdriver.h"

namespace ModbusEngine
{

ModbusDriver::ModbusDriver( MBPro* mbpro )
{
    this->mbpro = mbpro;
    this->build_the_tree();
}

void ModbusDriver::build_the_tree()
{
    std::vector<MBPro_Driver_Device>::iterator _it = this->mbpro->driver.devices.begin();
    for( ;_it != this->mbpro->driver.devices.end(); _it++ ) {
        MBPro_Driver_Device _d = *_it;
        ModbusDevice* _device = new ModbusDevice( _d.deviceId,
                                                  _d.ip,
                                                  _d.port,
                                                  _d.slaveId,
                                                  _d.responseTimeout,
                                                  _d.connectionTimeout );

        bool _first_block = true;
        std::vector<MBPro_Driver_Block>::iterator _it_2 = _d.blocks.begin();
        for( ;_it_2 != _d.blocks.end(); _it_2++ ) {
            MBPro_Driver_Block _b = *_it_2;
            ModbusBlock* _block = new ModbusBlock( _b.blockId,
                                                   _device->delegateConnection(),
                                                   _device->delegateConnectionMutex(),
                                                   _b.offset,
                                                   _b.count,
                                                   _b.cycleTime,
                                                   _b.retries,
                                                   3000 );

            if( _first_block ) {
                _first_block = false;
                _block->setMaster();
            }

            _device->addModbusBlock( _b.blockId, _block );
        }

       this->add_modbus_device( _d.deviceId, _device );
    }
}

void ModbusDriver::add_modbus_device( std::string deviceId, ModbusDevice* device )
{
    this->devices[ deviceId ] = device;
}

void ModbusDriver::startBlockThreads()
{
    std::map<std::string,ModbusDevice*>::iterator _it = this->devices.begin();
    for( ; _it != this->devices.end(); _it++ ) {
        std::pair<std::string,ModbusDevice*> _p = *_it;
        ModbusDevice* _device = _p.second;
        _device->startBlockThreads();
    }
}

bool ModbusDriver::readBit( std::string deviceId,
                            std::string blockId,
                            int nReg,
                            int nBit ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            bool _bit = _d->readBit( blockId, nReg, nBit );

            this->driverMutex.unlock();
            return _bit;
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

void ModbusDriver::writeBit( std::string deviceId,
                             std::string blockId,
                             int nReg,
                             int nBit,
                             bool bit ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            _d->writeBit( blockId, nReg, nBit, bit );
            this->driverMutex.unlock();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

uint8 ModbusDriver::readByte( std::string deviceId,
                              std::string blockId,
                              int nReg,
                              int nByte ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            uint8 _byte = _d->readByte( blockId, nReg, nByte );

            this->driverMutex.unlock();
            return _byte;
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

void ModbusDriver::writeByte( std::string deviceId,
                              std::string blockId,
                              int nReg,
                              int nByte,
                              uint8 byte ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            _d->writeByte( blockId, nReg, nByte, byte );
            this->driverMutex.unlock();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

uint16 ModbusDriver::readWord( std::string deviceId,
                               std::string blockId,
                               int nReg ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            uint16 _word = _d->readWord( blockId, nReg );

            this->driverMutex.unlock();
            return _word;
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

void ModbusDriver::writeWord( std::string deviceId,
                              std::string blockId,
                              int nReg,
                              uint16 word ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;
            _d->writeWord( blockId, nReg, word );
            this->driverMutex.unlock();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

void ModbusDriver::doWrite()
{
    this->driverMutex.lock();

    std::map<std::string,ModbusDevice*>::iterator _it = this->devices.begin();
    for( ; _it != this->devices.end(); _it++ )
    {
        std::pair<std::string,ModbusDevice*> p = *_it;
        ModbusDevice* d = p.second;
        d->doWrite();
    }

    this->driverMutex.unlock();
}

std::vector<std::string> ModbusDriver::getAllDeviceId()
{
    this->driverMutex.lock();

    std::vector<std::string> _device_ids;

    // get all device id
    for( std::map<std::string,ModbusDevice*>::iterator _it = this->devices.begin();
        _it != this->devices.end(); _it++ ) {
        std::pair<std::string,ModbusDevice*> _p = *_it;
        _device_ids.push_back( _p.first );
    }

    this->driverMutex.unlock();

    return _device_ids;
}

std::vector<std::string> ModbusDriver::getAllBlockId( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            std::vector<std::string> _block_ids;
            _block_ids = _d->getAllBlockId();

            this->driverMutex.unlock();
            return _block_ids;
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

std::string ModbusDriver::readDeviceIp( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            driverMutex.unlock();
            return _d->readIp();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readDevicePort( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readPort();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readDeviceSlaveId( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readSlaveId();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readDeviceResponseTimeout( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readResponseTimeout();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readDeviceConnectionTimeout( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readConnectionTimeout();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

std::string ModbusDriver::readDeviceConnStatus( std::string deviceId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readConnStatus();
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readBlockOffset( std::string deviceId, std::string blockId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readBlockOffset( blockId );
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readBlockCount( std::string deviceId, std::string blockId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readBlockCount( blockId );
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readBlockCycleTime( std::string deviceId, std::string blockId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readBlockCycleTime( blockId );
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

int ModbusDriver::readBlockRetries( std::string deviceId, std::string blockId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readBlockRetries( blockId );
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

std::string ModbusDriver::readBlockError( std::string deviceId, std::string blockId ) throw( std::string )
{
    this->driverMutex.lock();

    try
    {
        std::map<std::string,ModbusDevice*>::iterator _it = this->devices.find( deviceId );

        if( _it == this->devices.end() )
        {
            throw std::string( "bad_device" );
        }
        else
        {
            std::pair<std::string,ModbusDevice*> _p = *_it;
            ModbusDevice* _d = _p.second;

            this->driverMutex.unlock();
            return _d->readBlockError( blockId );
        }
    }
    catch( std::string ex )
    {
        this->driverMutex.unlock();
        throw std::string( ex );
    }
}

} // namespace ModbusEngine
