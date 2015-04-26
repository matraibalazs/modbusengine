#ifndef DWORDTAG_H
#define DWORDTAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The DWordTag class
 *
 * Inherited class, represents a dword value.
 * 32 bit value [-2147483648,2147483647]
 * For more information see the abstract Tag class definition.
 */
class DWordTag : public Tag
{

public:
    DWordTag( int id,
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

#endif // DWORDTAG_H
