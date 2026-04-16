QT += core network
QT -= gui
CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += server.cpp

target.path = /tmp/$$TARGET
INSTALLS += target
