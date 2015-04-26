#include <iostream>
#include <fstream>
#include <ctime>

#include "engine.h"
#include "Core/lib/rapidxml/rapidxml.hpp"

namespace ModbusEngine
{

Engine::Engine( std::string mbproXmlUrl )
{
    this->mbproXmlUrl = mbproXmlUrl;
}

void Engine::read_mpro() throw( std::string )
{
    mbpro = new MBPro( mbproXmlUrl );

    try {
        mbpro->readMBPro();
    } catch( std::string ex ) {
        throw ex;
    }
}

bool Engine::buildEngine()
{
    /// open mbpro file
    try
    {
        std::cout << "Open file: '" << this->mbproXmlUrl << "'...";
        read_mpro();
        std::cout << "DONE." << std::endl;
    }
    catch( std::string ex )
    {
        std::cout << std::endl;
        std::cout << "ERROR: " << ex << std::endl;
        return false;
    }

    /// create modbus driver
    std::cout << "Build Modbus Driver module...";
    driver = new ModbusDriver( mbpro );
    std::cout << "DONE." << std::endl;

    /// create tagsynchronizer module
    try
    {
        std::cout << "Build Tag Synchronizer module...";
        tagSynchronizer = new TagSynchronizer( mbpro, driver );
        std::cout << "DONE." << std::endl;
    }
    catch( std::string ex )
    {
        std::cout << std::endl;
        std::cout << "ERROR: " << ex << std::endl;
        return false;
    }

    /// create monitor synchronizer object
    try
    {
        std::cout << "Build Monitor Synchronizer module...";
        monitorSynchronizer = new MonitorSynchronizer( mbpro, driver );
        std::cout << "DONE." << std::endl;
    }
    catch( std::string ex )
    {
        std::cout << std::endl;
        std::cout << "ERROR: " << ex << std::endl;
        return false;
    }

    return true;
}

void Engine::startEngine()
{
    /// start modbus driver
    driver->startBlockThreads();

    /// start tag synchronizer
    tagSynchronizer->startThread();

    /// start monitor synchronizer
    monitorSynchronizer->startThread();
}

void Engine::loop()
{
    while( true )
    {
        Thread::sleep( 120 );
    }
}

void Engine::write_to_log( std::string message )
{
    std::cout << message;

    std::string logUrl = "/opt/modbusengine/log/modbusengine.log";

    /// opening log file...
    std::ofstream logFile;
    logFile.open( logUrl, logFile.out | logFile.app );
    if( !logFile.is_open() ) return;

    /// write...
    std::cout << message;
    logFile.write( message.c_str(), message.length() );

    /// close file
    logFile.close();
}

void Engine::empty_log()
{
    std::string logUrl = "/opt/modbusengine/log/modbusengine.log";

    /// opening log file...
    std::ofstream logFile;
    logFile.open( logUrl, logFile.out | logFile.trunc );
    if( !logFile.is_open() ) return;

    /// emptying
    logFile.write( "", 0 );

    /// close file
    logFile.close();
}

std::string Engine::get_timestamp()
{
    std::time_t dt = std::time( NULL );
    std::string asciiTime = std::asctime( std::localtime( &dt ) );
    return asciiTime.substr( 0, asciiTime.length() - 1 );
}

} // namespace ModbusEngine
