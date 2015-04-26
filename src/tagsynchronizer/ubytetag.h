#ifndef UBYTETAG_H
#define UBYTETAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The UByteTag class
 *
 * Inherited class, represents an unsigned byte value.
 * 8 bit value [0,255]
 * For more information see the abstract Tag class definition.
 */
class UByteTag : public Tag
{

public:
    UByteTag( int id,
              std::string name,
              std::string deviceId,
              std::string blockId,
              int address,
              int subAddress,
              std::string multiple,
              std::string add,
              bool wordSwap,
              int divider );

    void readValueFromModbusDriver( ModbusDriverDataInterface* );
    void writeValueToModbusDriver( ModbusDriverDataInterface* );

};

}

#endif // UBYTETAG_H
