#include <stdio.h>
#include <stdlib.h>
#include <string>

namespace ModbusEngine
{

/**
 * @brief The NetworkTester class
 *
 * It is a library class for test if the host is reachable or not.
 */
class NetworkTester
{

public:
    /**
     * @brief ping
     * @param ip        -> host's ip address
     * @param timeout   -> ping timeout in millisecs
     * @return the availability on the network
     */
    static bool ping( std::string ip, int timeout )
    {
        FILE * pipe;
        char str[1035];

        if( timeout < 1000 ) {
            timeout = 1000;
        }

        std::string command = "/bin/ping";
        std::string parameters = "-c 1 -W " + std::to_string( timeout / 1000 );
        std::string final_command = command + " " + parameters + " " + ip;

        /// Open the command pipe for reading.
        pipe = popen( final_command.c_str(), "r" );
        if ( pipe == NULL ) {
            return false;
        }

        std::string output = "";

        /// Get the output
        while ( fgets( str, sizeof(str)-1, pipe ) != NULL ) {
            output += str;
        }

        /// Close command pipe
        pclose( pipe );

        /// Processing...
        std::string pos_answer = "1 received";
        std::string neg_answer = "0 received";

        std::size_t pos = output.find( pos_answer );
        if( pos != std::string::npos ) {
            return true;
        }

        pos = output.find( neg_answer );
        if( pos != std::string::npos ) {
            return false;
        }

        return false;
    }
};

}
