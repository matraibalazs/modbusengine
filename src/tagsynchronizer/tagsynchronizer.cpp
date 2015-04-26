#include "tagsynchronizer.h"
#include "bittag.h"
#include "bytetag.h"
#include "ubytetag.h"
#include "wordtag.h"
#include "uwordtag.h"
#include "dwordtag.h"
#include "udwordtag.h"
#include "real16tag.h"
#include "../Core/conversion.hpp"

namespace ModbusEngine {

TagSynchronizer::TagSynchronizer( MBPro* mbpro, ModbusDriverDataInterface* driverInterface ) throw( std::string )
{
    this->mbpro = mbpro;
    this->driverInterface = driverInterface;
    this->cycleTime = 50;

    /// create driver object...
    try
    {
        this->mysqlDriver = new MySQLDriver( this->mbpro->db.dbUrl,
                                             this->mbpro->db.dbPort,
                                             this->mbpro->db.dbName,
                                             this->mbpro->db.dbUser,
                                             this->mbpro->db.dbPass );
    }
    catch( SQLDriverException ex )
    {
        throw ex.description;
    }

    /// build the tag map...
    this->build_tag_map();

    /// build SQL data tables....
    try
    {
        this->build_tables();
    }
    catch( std::string ex )
    {
        throw ex;
    }
}

void TagSynchronizer::build_tag_map()
{
    // build tagMap, writeFlags and tagCache maps
    for( std::vector<MBPro_Tag>::iterator _it = this->mbpro->taglist.tags.begin();
             _it != this->mbpro->taglist.tags.end(); _it++ )
    {
         MBPro_Tag _t = *_it;

         if( _t.type == "bit" )
         {
             BitTag* _tag = new BitTag( _t.id,
                                        _t.name,
                                        _t.deviceId,
                                        _t.blockId,
                                        _t.address,
                                        _t.subAddress,
                                        _t.multiple,
                                        _t.add,
                                        _t.wordSwap,
                                        _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "byte" )
         {
             ByteTag* _tag = new ByteTag( _t.id,
                                          _t.name,
                                          _t.deviceId,
                                          _t.blockId,
                                          _t.address,
                                          _t.subAddress,
                                          _t.multiple,
                                          _t.add,
                                          _t.wordSwap,
                                          _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "ubyte" )
         {
             UByteTag* _tag = new UByteTag( _t.id,
                                            _t.name,
                                            _t.deviceId,
                                            _t.blockId,
                                            _t.address,
                                            _t.subAddress,
                                            _t.multiple,
                                            _t.add,
                                            _t.wordSwap,
                                            _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "word" )
         {
             WordTag* _tag = new WordTag( _t.id,
                                          _t.name,
                                          _t.deviceId,
                                          _t.blockId,
                                          _t.address,
                                          _t.subAddress,
                                          _t.multiple,
                                          _t.add,
                                          _t.wordSwap,
                                          _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "uword" )
         {
             UWordTag* _tag = new UWordTag( _t.id,
                                            _t.name,
                                            _t.deviceId,
                                            _t.blockId,
                                            _t.address,
                                            _t.subAddress,
                                            _t.multiple,
                                            _t.add,
                                            _t.wordSwap,
                                            _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "dword" )
         {
             DWordTag* _tag = new DWordTag( _t.id,
                                            _t.name,
                                            _t.deviceId,
                                            _t.blockId,
                                            _t.address,
                                            _t.subAddress,
                                            _t.multiple,
                                            _t.add,
                                            _t.wordSwap,
                                            _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "udword" )
         {
             UDWordTag* _tag = new UDWordTag( _t.id,
                                              _t.name,
                                              _t.deviceId,
                                              _t.blockId,
                                              _t.address,
                                              _t.subAddress,
                                              _t.multiple,
                                              _t.add,
                                              _t.wordSwap,
                                              _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
         else if( _t.type == "real16" )
         {
             Real16Tag* _tag = new Real16Tag( _t.id,
                                              _t.name,
                                              _t.deviceId,
                                              _t.blockId,
                                              _t.address,
                                              _t.subAddress,
                                              _t.multiple,
                                              _t.add,
                                              _t.wordSwap,
                                              _t.divider );

             this->tagMap[ _t.id ] = _tag;
             this->tagValueCache[ _t.id ] = _tag->value;
             this->tagValidityCache[ _t.id ] = _tag->validity;
         }
    }
}

void TagSynchronizer::build_tables() throw( std::string )
{
    /// open the connection
    try
    {
        this->mysqlDriver->connect();
    }
    catch( SQLDriverException ex )
    {
        throw ex.description;
    }

    /// the sql stringstream
    std::stringstream sql;

    /// delete the existing datatables
    try
    {
        sql << "DROP TABLE tags;";
        this->mysqlDriver->execute( sql.str() );
        sql.str("");

        sql << "DROP TABLE control;";
        this->mysqlDriver->execute( sql.str() );
    }
    catch( SQLDriverException )
    {
        /// doing nothing :-)
    }

    /// create datatables
    try
    {
        /// create tags table
        sql.str("");
        sql << "CREATE TABLE tags";
        sql << "(";
        sql << "id int(11) NOT NULL,";
        sql << "name varchar(500) NOT NULL,";
        sql << "device_id varchar(500) NOT NULL,";
        sql << "block_id varchar(500) NOT NULL,";
        sql << "address int(11) NOT NULL,";
        sql << "type varchar(100) NOT NULL,";
        sql << "sub_address int(11) DEFAULT 0,";
        sql << "validity varchar(100) DEFAULT \"undefined\",";
        sql << "multiple varchar(500) DEFAULT \"1\",";
        sql << "_add varchar(500) DEFAULT \"0\",";
        sql << "word_swap int(11) DEFAULT 0,";
        sql << "divider int(11) DEFAULT 1,";
        sql << "value varchar(500) DEFAULT \"0\",";
        sql << "write_value varchar(500) DEFAULT \"0\",";
        sql << "write_flag int(11) DEFAULT 0,";
        sql << "PRIMARY KEY (id)";
        sql << ")";
        sql << "DEFAULT CHARSET=utf8 COLLATE=utf8_hungarian_ci ENGINE=MEMORY;";
        this->mysqlDriver->execute( sql.str() );

        /// create control table
        sql.str("");
        sql << "CREATE TABLE control";
        sql << "(";
        sql << "row_key int(11) NOT NULL,";
        sql << "write_flag int(11) NOT NULL,";
        sql << "heartbeat int(11),";
        sql << "PRIMARY KEY (row_key)";
        sql << ")";
        sql << "DEFAULT CHARSET=utf8 COLLATE=utf8_hungarian_ci ENGINE=MEMORY;";
        this->mysqlDriver->execute( sql.str() );

        /// insert tags to tagtable
        std::map<int,Tag*>::iterator it = this->tagMap.begin();
        for( ; it != this->tagMap.end(); it++ )
        {
            std::pair<int,Tag*> p = *it;
            Tag* _t = p.second;
            sql.str("");
            sql << "INSERT INTO tags VALUES(";
            sql << _t->id << ",";
            sql << "'" << _t->name << "',";
            sql << "'" << _t->deviceId << "',";
            sql << "'" << _t->blockId << "',";
            sql << _t->address << ",";
            sql << "'" << _t->type << "',";
            sql << _t->subAddress << ",";
            sql << "'" << _t->validity << "',";
            sql << "'" << _t->multiple << "',";
            sql << "'" << _t->add + "',";
            sql << _t->wordSwap << ",";
            sql << _t->divider << ",";
            sql << "'" << _t->value << "',";
            sql << "'" << _t->value << "',";
            sql << "0";
            sql << ");";

            this->mysqlDriver->execute( sql.str() );
        }

        /// insert row to control table
        sql.str("");
        sql << "INSERT INTO control VALUES(0,0,0);";
        this->mysqlDriver->execute( sql.str() );

    }
    catch( SQLDriverException ex )
    {
        this->mysqlDriver->close();
        throw ex.description;
    }

    /// close the connection
    this->mysqlDriver->close();
}

void TagSynchronizer::do_read()
{
    try
    {
        /// connect to DB
        this->mysqlDriver->connect();

        std::map<int,Tag*>::iterator it = this->tagMap.begin();
        for( ; it != this->tagMap.end(); it++ )
        {
            /// get all tag and refresh the values
            std::pair<int,Tag*> p = *it;
            Tag* t = p.second;
            t->readValueFromModbusDriver( this->driverInterface );

            /// if the value is not cached value, update in the sql table
            if( t->value != tagValueCache[ t->id ] || t->validity != tagValidityCache[ t->id ] )
            {
                std::stringstream sql;
                sql << "UPDATE tags SET value='" << t->value << "',validity='" << t->validity << "' ";
                sql << "WHERE id=" << t->id;
                this->mysqlDriver->execute( sql.str() );

                /// refresh cache...
                tagValueCache[ t->id ] = t->value;
                tagValidityCache[ t->id ] = t->validity;
            }
        }

        /// close the connection
        this->mysqlDriver->close();
    }
    catch( SQLDriverException )
    {
        this->mysqlDriver->close();
    }
}

void TagSynchronizer::do_write()
{
    try
    {
        /// open the connection
        this->mysqlDriver->connect();

        /// check global write flag
        std::stringstream sql;
        sql << "SELECT write_flag FROM control WHERE row_key=0;";
        SQLResult res = this->mysqlDriver->executeQuery( sql.str() );
        SQLRow row = res.getRow( 0 );
        int write_flag = row.getInt( "write_flag" );

        /// if write_flag = 0 -> exit
        if( write_flag == 0 )
        {
            this->mysqlDriver->close();
            return;
        }

        /// get tags for write
        sql.str("");
        sql << "SELECT id, write_value FROM tags WHERE write_flag=1;";
        SQLResult res_2 = this->mysqlDriver->executeQuery( sql.str() );
        for( int i = 0; i < res_2.getRowNum(); i++ )
        {
            /// refresh all values in the modbus driver
            SQLRow row_2 = res_2.getRow( i );
            Tag* t = tagMap[ row_2.getInt( "id" ) ];
            t->value = row_2.getString( "write_value" );
            t->writeValueToModbusDriver( this->driverInterface );
        }

        /// call doWrite()
        this->driverInterface->doWrite();

        /// reset write flags
        sql.str("");
        sql << "UPDATE control SET write_flag=0 WHERE row_key=0;";
        this->mysqlDriver->execute( sql.str() );
        sql.str("");
        sql << "UPDATE tags SET write_flag=0 WHERE write_flag=1;";
        this->mysqlDriver->execute( sql.str() );

        /// close connection
        this->mysqlDriver->close();
    }
    catch( SQLDriverException )
    {
        this->mysqlDriver->close();
    }
}

void TagSynchronizer::do_heartbeat()
{
    try
    {
        /// open the connection
        this->mysqlDriver->connect();

        /// do the heartbeat
        std::string sql = "UPDATE control SET heartbeat=0 WHERE row_key=0;";
        this->mysqlDriver->execute( sql );

        /// close connection
        this->mysqlDriver->close();
    }
    catch( SQLDriverException )
    {
        this->mysqlDriver->close();
    }
}

/**
 ############################################################################
 # Inherited functions from Thread.
 ############################################################################
*/

void TagSynchronizer::run()
{
    int k = 0;
    while( true ) {
        /// read...
         this->do_read();

        /// write
        this->do_write();

        /// heartbeat :-)
        if( ++k > 60 )
        {
            k = 0;
            this->do_heartbeat();
        }

        Thread::msleep( this->cycleTime );
    }
}

void TagSynchronizer::halt(){}

} // namespace ModbusEngine
