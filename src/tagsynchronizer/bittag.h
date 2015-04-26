#ifndef BITTAG_H
#define BITTAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The BitTag class
 *
 * Inherited class, represents a bit value.
 * 1 bit value [0,1]
 * For more information see the abstract Tag class definition.
 */
class BitTag : public Tag
{

public:
    BitTag( int id,
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

#endif // BITTAG_H
