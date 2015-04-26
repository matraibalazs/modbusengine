#ifndef UDWORDTAG_H
#define UDWORDTAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The UDWordTag class
 *
 * Inherited class, represents an unsigned dword value.
 * 32 bit value [0,4294967295]
 * For more information see the abstract Tag class definition.
 */
class UDWordTag : public Tag
{

public:
    UDWordTag( int id,
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

#endif // UDWORDTAG_H
