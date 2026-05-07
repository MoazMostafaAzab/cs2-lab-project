QT += core network
QT -= gui
CONFIG += c++17 console
CONFIG -= app_bundle

SOURCES += \
    server.cpp \
    MessageRouter.cpp

HEADERS += \
    MessageRouter.h

target.path = /tmp/$$TARGET
INSTALLS += target
