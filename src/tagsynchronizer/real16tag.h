#ifndef REAL16TAG_H
#define REAL16TAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The Real16Tag class
 *
 * Inherited class, represents a real16 value.
 * 16 bit value [-32768,32767]
 * For more information see the abstract Tag class definition.
 */
class Real16Tag : public Tag
{

public:
    Real16Tag( int id,
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

#endif // REAL16TAG_H
