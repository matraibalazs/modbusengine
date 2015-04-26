#ifndef BYTETAG_H
#define BYTETAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The ByteTag class
 *
 * Inherited class, represents a byte value.
 * 8 bit value [-128,127]
 * For more information see the abstract Tag class definition.
 */
class ByteTag : public Tag
{

public:
    ByteTag( int id,
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

#endif // BYTETAG_H
