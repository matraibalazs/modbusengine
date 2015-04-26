#ifndef WORDTAG_H
#define WORDTAG_H

#include "tag.h"

namespace ModbusEngine
{

/**
 * @brief The WordTag class
 *
 * Inherited class, represents a word value.
 * 16 bit value [-32768,32767]
 * For more information see the abstract Tag class definition.
 */
class WordTag : public Tag
{

public:
    WordTag( int id,
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

#endif // WORDTAG_H
