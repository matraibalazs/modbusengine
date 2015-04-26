#include "tag.h"

namespace ModbusEngine
{

Tag::Tag( int id,
          std::string name,
          std::string deviceId,
          std::string blockId,
          int address,
          int subAddress,
          std::string multiple,
          std::string add,
          bool wordSwap,
          int divider )
{
    this->id = id;
    this->name = name;
    this->deviceId = deviceId;
    this->blockId = blockId;
    this->address = address;
    this->subAddress = subAddress;
    this->multiple = multiple;
    this->add = add;
    this->wordSwap = wordSwap;
    this->divider = divider;
}

}
