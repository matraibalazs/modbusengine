#ifndef ENGINE_H
#define ENGINE_H

#include "ModbusDriver/modbusdriver.h"
#include "TagSynchronizer/tagsynchronizer.h"
#include "MonitorSynchronizer/monitorsynchronizer.h"

namespace ModbusEngine
{

class Engine
{

private:
    /// the url of the mbpro file
    std::string mbproXmlUrl;
    /// inner created mbpro file
    MBPro* mbpro;
    /// inner created modbus driver object
    ModbusDriver* driver;
    /// inner created tag synchronizer object
    TagSynchronizer* tagSynchronizer;
    /// inner created monitor synchronizer object
    MonitorSynchronizer* monitorSynchronizer;

    /**
     * @brief readMbproXml
     *
     * Reads the mbpro file.
     *
     * This function throws std::string exception.
     */
    void read_mpro() throw( std::string );

    /**
     * @brief writeToLog
     * @param message -> message string
     *
     * Writes message to log and to the standard output.
     */
    void write_to_log( std::string message );

    /**
     * @brief emptyLog
     *
     * Delete all content of the log file.
     */
    void empty_log();

    /**
     * @brief getDateTime
     * @return the current timestamp in string
     */
    std::string get_timestamp();

public:
    Engine( std::string mbproXmlUrl );

    /**
     * @brief buildEngine
     * @return the status of the building
     *
     * Creates the module objects.
     */
    bool buildEngine();

    /**
     * @brief startEngine
     * @return the status of the starting
     *
     * Starts the engine modules.
     */
    void startEngine();

    /**
     * @brief loop
     *
     * Empty loop for main function to stay in live.
     */
    void loop();

};

}

#endif // ENGINE_H
