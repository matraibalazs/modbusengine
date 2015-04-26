#ifndef MBPRO_H
#define MBPRO_H

#include <vector>
#include <string>

namespace ModbusEngine {

class MBPro_Driver_Block
{
public:
    std::string blockId;
    int offset;
    int count;
    int cycleTime;
    int retries;
};

class MBPro_Driver_Device
{
public:
    std::string deviceId;
    std::string ip;
    int port;
    int slaveId;
    int responseTimeout;
    int connectionTimeout;
    std::vector<MBPro_Driver_Block> blocks;
};

class MBPro_Driver
{
public:
    std::vector<MBPro_Driver_Device> devices;
};

class MBPro_Tag
{
public:
    int id;
    std::string name;
    std::string deviceId;
    std::string blockId;
    int address;
    int subAddress;
    std::string type;
    std::string multiple;
    std::string add;
    int divider;
    bool wordSwap;
};

class MBPro_Taglist
{
public:
    std::vector<MBPro_Tag> tags;
};

class MBPro_DB
{
public:
    std::string dbType;
    std::string dbUrl;
    std::string dbPort;
    std::string dbName;
    std::string dbUser;
    std::string dbPass;
};

class MBPro_Project
{
public:
    std::string name;
};

class MBPro
{

public:
    MBPro_Project project;
    MBPro_DB db;
    MBPro_Driver driver;
    MBPro_Taglist taglist;
    std::string filename;

public:
    MBPro( std::string filename );
    void readMBPro() throw ( std::string );

};

}

#endif // MBPRO_H
