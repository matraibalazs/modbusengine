#ifndef UWORDTAG_H
#define UWORDTAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The UWordTag class
 *
 * Inherited class, represents an unsigned word value.
 * 16 bit value [0,65535]
 * For more information see the abstract Tag class definition.
 */
class UWordTag : public Tag
{

public:
    UWordTag( int id,
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

#endif // UWORDTAG_H
