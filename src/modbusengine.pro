##############################################
# Qt app definitions
##############################################

TEMPLATE = app
CONFIG += console
CONFIG -= app_bundle
CONFIG -= qt

##############################################
# project header files
##############################################

# Core headers
HEADERS += core/conversion.hpp
HEADERS += core/mbtcpmasterconnection.h
HEADERS += core/networktester.hpp
HEADERS += core/thread.hpp
HEADERS += core/types.h

# Modbus Driver modul headers
HEADERS += modbusdriver/modbusblock.h
HEADERS += modbusdriver/modbusdevice.h
HEADERS += modbusdriver/modbusdriver.h
HEADERS += modbusdriver/modbusdriverdatainterface.h
HEADERS += modbusdriver/modbusdrivermonitorinterface.h

# Tag Synchronizer modul headers
HEADERS += tagsynchronizer/bittag.h
HEADERS += tagsynchronizer/bytetag.h
HEADERS += tagsynchronizer/dwordtag.h
HEADERS += tagsynchronizer/real16tag.h
HEADERS += tagsynchronizer/tag.h
HEADERS += tagsynchronizer/tagsynchronizer.h
HEADERS += tagsynchronizer/ubytetag.h
HEADERS += tagsynchronizer/udwordtag.h
HEADERS += tagsynchronizer/uwordtag.h
HEADERS += tagsynchronizer/wordtag.h

# SQL Driver modul headers
HEADERS += sqldriver/mysqldriver.h
HEADERS += sqldriver/sqldriver.h
HEADERS += sqldriver/sqldriverexception.hpp
HEADERS += sqldriver/sqlresult.hpp

# Monitor Synchronizer modul headers
HEADERS += monitorsynchronizer/monitorsynchronizer.h

# mbpro file and engine headers
HEADERS += engine.h
HEADERS += mbpro.h

##############################################
# project source files
##############################################

# Core source files
SOURCES += core/mbtcpmasterconnection.cpp

# Modbus Driver modul sources
SOURCES += modbusdriver/modbusblock.cpp
SOURCES += modbusdriver/modbusdevice.cpp
SOURCES += modbusdriver/modbusdriver.cpp

# Tag Synchronizer modul sources
SOURCES += tagsynchronizer/bittag.cpp
SOURCES += tagsynchronizer/bytetag.cpp
SOURCES += tagsynchronizer/dwordtag.cpp
SOURCES += tagsynchronizer/real16tag.cpp
SOURCES += tagsynchronizer/tag.cpp
SOURCES += tagsynchronizer/tagsynchronizer.cpp
SOURCES += tagsynchronizer/ubytetag.cpp
SOURCES += tagsynchronizer/udwordtag.cpp
SOURCES += tagsynchronizer/uwordtag.cpp
SOURCES += tagsynchronizer/wordtag.cpp

# SQL Driver modul sources
SOURCES += sqldriver/mysqldriver.cpp

# Monitor Synchronizer modul sources
SOURCES += monitorsynchronizer/monitorsynchronizer.cpp

# mbpro and engine sources and the main
SOURCES += engine.cpp
SOURCES += mbpro.cpp
SOURCES += main.cpp

##############################################
# dynamic libraries
##############################################

# linking init
unix: CONFIG += link_pkgconfig

# libmodbus
unix: PKGCONFIG += libmodbus

# libmysqlcppconn
unix: PKGCONFIG += libmysqlcppconn

##############################################
# other compile options
##############################################

# C++11 and thread support
QMAKE_CXXFLAGS += -std=c++11
LIBS += -pthread

# wtf???
include(deployment.pri)
qtcAddDeployment()
