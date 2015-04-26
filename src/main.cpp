#include <iostream>
#include <sys/types.h>
#include <sys/stat.h>
#include <stdio.h>
#include <stdlib.h>
#include <fcntl.h>
#include <errno.h>
#include <unistd.h>
#include <syslog.h>
#include <string.h>

#include "engine.h"

using namespace std;

int main( int argc, char* argv[] )
{
    /// Read parameters
    std::string projectXMLPath;

    if( argc == 1 || argc > 2 ) {
        std::cout << "Usage:" << std::endl;
        std::cout << "modbusengine <path-to-project-xml-file>" << std::endl;
        return -1;
    } else {
        projectXMLPath = std::string( argv[1] );
    }

    /// build the modbus engine
    ModbusEngine::Engine* engine = new ModbusEngine::Engine( projectXMLPath );
    if( !engine->buildEngine() )
    {
        return -1;
    }

    /// Fork the process...
    pid_t pid, sid;
    pid = fork();

    if( pid < 0 ) { exit( EXIT_FAILURE ); }

    /// We got a good pid, Close the Parent Process
    if( pid > 0 ) { exit( EXIT_SUCCESS ); }

    /// Change File Mask
    umask( 0 );

    /// Create a new Signature Id for our child
    sid = setsid();
    if( sid < 0 ) { exit( EXIT_FAILURE ); }

    /// Close Standard File Descriptors
    close( STDIN_FILENO );
    close( STDOUT_FILENO );
    close( STDERR_FILENO );

    /// start the engine
    engine->startEngine();

    /// Keep the main thread in live...
    engine->loop();

    return 0;
}
