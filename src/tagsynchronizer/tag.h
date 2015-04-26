#ifndef TAG_H
#define TAG_H

#include "../ModbusDriver/modbusdriverdatainterface.h"

namespace ModbusEngine
{

/**
 * @brief The Tag class
 *
 * Represents an abstract Tag.
 */

class Tag
{

public:
    int id;                     /// the tag's id
    std::string name;           /// the tag's unique name
    std::string deviceId;       /// the target device ID
    std::string blockId;        /// the target block ID of the target device
    int address;                /// the target block's offset (word position)
    int subAddress;             /// the target block's subAddress
    std::string multiple;       /// multiple value
    std::string add;            /// add value
    bool wordSwap;              /// wordSwap for dwords
    int divider;                /// divider for real16 tags
    std::string value;          /// the tag's value
    std::string type;           /// datatype string
    std::string validity;       /// this string describes the valdity status

public:
    /**
     * @brief Tag
     * @param id
     * @param name
     * @param deviceId
     * @param blockId
     * @param address
     * @param subAddress
     * @param multiple
     * @param add
     * @param wordSwap
     * @param divider
     *
     * Constructs a Tag object.
     */
    Tag( int id,
         std::string name,
         std::string deviceId,
         std::string blockId,
         int address,
         int subAddress,
         std::string multiple,
         std::string add,
         bool wordSwap,
         int divider );

    /**
     * @brief readValueFromModbusDriver
     * @param interface -> delegated modbus driver data interface object
     *
     * Refreshs the tag's value and the validity member.
     */
    virtual void readValueFromModbusDriver( ModbusDriverDataInterface* interface ) = 0;

    /**
     * @brief writeValueToModbusDriver
     * @param interface -> delegated modbus driver data interface object
     *
     * Writes the tag's value to the modbus driver and refreshs the validity value.
     */
    virtual void writeValueToModbusDriver( ModbusDriverDataInterface* interface ) = 0;

};

} // namespace ModbusEngine

#endif // TAG_H
