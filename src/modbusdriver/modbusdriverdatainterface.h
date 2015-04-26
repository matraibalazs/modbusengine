#ifndef MODBUSDRIVERDATAINTERFACE_H
#define MODBUSDRIVERDATAINTERFACE_H

#include <string>

#include "../Core/types.h"

namespace ModbusEngine
{

/**
 * @brief The ModbusDriverDataInterface class
 *
 * Define the data interface of the modbus driver.
 *
 * DO NOT ADD MORE DATATYPE SUPPORT HERE. IT'S A FUNDAMENTAL DESIGN IDEA.
 */

class ModbusDriverDataInterface
{

public:
    bool virtual readBit( std::string deviceId,
                          std::string blockId,
                          int nReg,
                          int nBit ) throw( std::string ) = 0;

    void virtual writeBit( std::string deviceId,
                           std::string blockId,
                           int nReg,
                           int nBit,
                           bool bit ) throw( std::string ) = 0;

    uint8 virtual readByte( std::string deviceId,
                             std::string blockId,
                             int nReg,
                             int nByte ) throw( std::string ) = 0;

    void virtual writeByte( std::string deviceId,
                            std::string blockId,
                            int nReg,
                            int nByte,
                            uint8 byte ) throw( std::string ) = 0;

    uint16 virtual readWord( std::string deviceId,
                              std::string blockId,
                              int nReg ) throw( std::string ) = 0;

    void virtual writeWord( std::string deviceId,
                            std::string blockId,
                            int nReg,
                            uint16 word ) throw( std::string ) = 0;

    void virtual doWrite() = 0;

};

}

#endif // MODBUSDRIVERDATAINTERFACE_H
