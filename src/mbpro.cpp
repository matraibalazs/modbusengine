#include <iostream>
#include <sstream>
#include <fstream>

#include "mbpro.h"
#include "Core/lib/rapidxml/rapidxml.hpp"

namespace ModbusEngine {

MBPro::MBPro( std::string filename )
{
    this->filename = filename;
}

void MBPro::readMBPro() throw ( std::string )
{
    // sstream for converts
    std::stringstream ss;

    // opening MBPro file...
    std::ifstream mbproFile;
    mbproFile.open( filename );

    if( !mbproFile.is_open() ) {
        throw "Error: MBPro file cannot read.( " + filename + " )";
    }

    // read the MBPro file...
    mbproFile.seekg( 0, mbproFile.end );
    int size = mbproFile.tellg();
    mbproFile.seekg( 0, mbproFile.beg );

    char* mbproFileContent = new char[ size ];
    mbproFile.read( mbproFileContent, size );

    mbproFile.close();

    // parsing the xml file...
    rapidxml::xml_document<> doc;

    try {
        doc.parse<0>( mbproFileContent );
    } catch( rapidxml::parse_error ex ){
        throw "Error: mbpro file not valid.( " + filename + " )\n" + ex.what();
    }

    // get the root element...
    rapidxml::xml_node<>* _root = doc.first_node( "mbpro" );

    if( _root == NULL ) {
        throw "Error: missing mbpro tag in mbpro file.( " + filename + " )";
    }

    // read MBPro_Project...
    rapidxml::xml_node<>* _project = _root->first_node( "project" );

    if( _project == NULL ) {
        throw "Error: missing project tag in mbpro file.( " + filename + " )";
    }

    rapidxml::xml_node<>* _project_name = _project->first_node( "name" );

    if( _project_name == NULL ) {
        throw "Error: missing name tag in mbpro file.( " + filename + " )";
    }

    project.name = std::string( _project_name->value() );

    // read MBPro_DB...
    db.dbType = "null";
    db.dbUrl = "null";
    db.dbPort = "null";
    db.dbName = "null";
    db.dbUser = "null";
    db.dbPass = "null";

    rapidxml::xml_node<>* _db = _root->first_node( "db" );
    for( rapidxml::xml_node<>* n = _db->first_node();
         n; n = n->next_sibling() ) {
        if( std::string( n->name() ) == "dbType" ) {
            db.dbType = std::string( n->value() );
        } else if( std::string( n->name() ) == "dbUrl" ) {
            db.dbUrl = std::string( n->value() );
        } else if( std::string( n->name() ) == "dbPort" ) {
            db.dbPort = std::string( n->value() );
        } else if( std::string( n->name() ) == "dbName" ) {
            db.dbName = std::string( n->value() );
        } else if( std::string( n->name() ) == "dbUser" ) {
            db.dbUser = std::string( n->value() );
        } else if( std::string( n->name() ) == "dbPass" ) {
            db.dbPass = std::string( n->value() );
        }
    }

    // Testing...
    if( db.dbType == "null" ) {
        throw "Error: missing dbType tag in mbpro file.( " + filename + " )";
    }

    if( db.dbUrl == "null" ) {
        throw "Error: missing dbUrl tag in mbpro file.( " + filename + " )";
    }

    if( db.dbPort == "null" ) {
        throw "Error: missing dbPort tag in mbpro file.( " + filename + " )";
    }

    if( db.dbName == "null" ) {
        throw "Error: missing dbName tag in mbpro file.( " + filename + " )";
    }

    if( db.dbUser == "null" ) {
        throw "Error: missing dbUser tag in mbpro file.( " + filename + " )";
    }

    if( db.dbPass == "null" ) {
        throw "Error: missing dbPass tag in mbpro file.( " + filename + " )";
    }

    // read MBPro_Driver
    rapidxml::xml_node<>* _modbusdriver = _root->first_node( "modbusdriver" );

    if( _modbusdriver == NULL ) {
        throw "Error: missing modbusdriver tag in mbpro file.( " + filename + " )";
    }

    rapidxml::xml_node<>* _devices = _modbusdriver->first_node( "devices" );

    if( _devices == NULL ) {
        throw "Error: missing devices tag in mbpro file.( " + filename + " )";
    }

    // create all MBPro Device and MBPRo Block
    for( rapidxml::xml_node<>* d = _devices->first_node( "device" );
         d; d = d->next_sibling( "device" ) ) {
        MBPro_Driver_Device device;
        device.deviceId = "null";
        device.ip = "null";
        device.port = 502;
        device.slaveId = 1;
        device.responseTimeout = 1000;
        device.connectionTimeout = 3000;

        rapidxml::xml_node<>* blocks;

        for( rapidxml::xml_node<>* n = d->first_node();
             n; n = n->next_sibling() ) {
            if( std::string( n->name() ) == "deviceId" ) {
                device.deviceId = std::string( n->value() );
            } else if( std::string( n->name() ) == "ip" ) {
                device.ip = std::string( n->value() );
            } else if( std::string( n->name() ) == "port" ) {
                ss.str("");
                ss.clear();
                ss << std::string( n->value() );
                ss >> device.port;
            } else if( std::string( n->name() ) == "slaveId" ) {
                ss.str("");
                ss.clear();
                ss << std::string( n->value() );
                ss >> device.slaveId;
            } else if( std::string( n->name() ) == "responseTimeout" ) {
                ss.str("");
                ss.clear();
                ss << std::string( n->value() );
                ss >> device.responseTimeout;
            } else if( std::string( n->name() ) == "connectionTimeout" ) {
                ss.str("");
                ss.clear();
                ss << std::string( n->value() );
                ss >> device.connectionTimeout;
            } else if( std::string( n->name() ) == "blocks" ) {
                blocks = n;
            }
        }

        if( device.deviceId == "null" ) {
            throw "Error: missing deviceId tag in mbpro file.( " + filename + " )";
        }

        if( device.ip == "null" ) {
            throw "Error: missing ip tag in mbpro file.( " + filename + " )";
        }

        if( blocks == NULL ) {
            throw "Error: missing blocks tag in mbpro file.( " + filename + " )";
        }

        // create all blocks
        for( rapidxml::xml_node<>* b = blocks->first_node( "block" );
             b; b = b->next_sibling( "block" ) ) {
            MBPro_Driver_Block block;
            block.blockId = "null";
            block.offset = -1;
            block.count = -1;
            block.cycleTime = 1000;
            block.retries = 3;

            for( rapidxml::xml_node<>* n1 = b->first_node();
                 n1; n1 = n1->next_sibling() ) {
                if( std::string( n1->name() ) == "blockId" ) {
                    block.blockId = std::string( n1->value() );
                } else if( std::string( n1->name() ) == "offset" ) {
                    ss.str("");
                    ss.clear();
                    ss << std::string( n1->value() );
                    ss >> block.offset;
                } else if( std::string( n1->name() ) == "count" ) {
                    ss.str("");
                    ss.clear();
                    ss << std::string( n1->value() );
                    ss >> block.count;
                } else if( std::string( n1->name() ) == "cycleTime" ) {
                    ss.str("");
                    ss.clear();
                    ss << std::string( n1->value() );
                    ss >> block.cycleTime;
                } else if( std::string( n1->name() ) == "retries" ) {
                    ss.str("");
                    ss.clear();
                    ss << std::string( n1->value() );
                    ss >> block.retries;
                }
            }

            if( block.blockId == "null" ) {
                throw "Error: missing blockId tag in mbpro file.( " + filename + " )";
            }

            if( block.offset == -1 ) {
                throw "Error: missing offset tag in mbpro file.( " + filename + " )";
            }

            if( block.count == -1 ) {
                throw "Error: missing count tag in mbpro file.( " + filename + " )";
            }

            device.blocks.push_back( block );
        }

        driver.devices.push_back( device );
    }

    // read MBPro Taglist
    rapidxml::xml_node<>* _taglist = _root->first_node( "taglist" );

    if( _taglist == NULL ) {
        throw "Error: missing taglist tag in mbpro file.( " + filename + " )";
    }

    int i = 0;

    for( rapidxml::xml_node<>* t = _taglist->first_node( "tag" );
         t; t = t->next_sibling( "tag" ) ) {
        MBPro_Tag tag;
        tag.id = i++;
        tag.name = "null";
        tag.deviceId = "null";
        tag.blockId = "null";
        tag.address = -1;
        tag.subAddress = 0;
        tag.type = "null";
        tag.multiple = "1";
        tag.add = "0";
        tag.divider = 1;
        tag.wordSwap = 0;

        for( rapidxml::xml_attribute<>* attr = t->first_attribute();
             attr; attr = attr->next_attribute() ) {
            if( std::string( attr->name() ) == "name" ) {
                tag.name = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "deviceId" ) {
                tag.deviceId = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "blockId" ) {
                tag.blockId = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "address" ) {
                ss.str( "" );
                ss.clear();
                ss << std::string( attr->value() );
                ss >> tag.address;
            } else if( std::string( attr->name() ) == "subAddress" ) {
                ss.str( "" );
                ss.clear();
                ss << std::string( attr->value() );
                ss >> tag.subAddress;
            } else if( std::string( attr->name() ) == "type" ) {
                tag.type = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "multiple" ) {
                tag.multiple = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "_add" ) {
                tag.add = std::string( attr->value() );
            } else if( std::string( attr->name() ) == "divider" ) {
                ss.str( "" );
                ss.clear();
                ss << std::string( attr->value() );
                ss >> tag.divider;
            } else if( std::string( attr->name() ) == "wordSwap" ) {
                ss.str( "" );
                ss.clear();
                ss << std::string( attr->value() );
                ss >> tag.wordSwap;
            }
        }

        if( tag.name == "null" ) {
            throw "Error: missing name attribute at tags in mbpro file.( " + filename + " )";
        }

        if( tag.deviceId == "null" ) {
            throw "Error: missing deviceId attribute at tags in mbpro file.( " + filename + " )";
        }

        if( tag.blockId == "null" ) {
            throw "Error: missing blockId attribute at tags in mbpro file.( " + filename + " )";
        }

        if( tag.address == -1 ) {
            throw "Error: missing address attribute at tags in mbpro file.( " + filename + " )";
        }

        if( tag.type == "null" ) {
            throw "Error: missing type attribute at tags in mbpro file.( " + filename + " )";
        }

        taglist.tags.push_back( tag );
    }

    delete mbproFileContent;
}

}


