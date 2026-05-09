QT += widgets network

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Testing
QT += testlib
DEFINES += GTEST_HAS_PTHREAD=0
INCLUDEPATH += $$PWD/googletest/googletest/include
INCLUDEPATH += $$PWD/googletest/googlemock/include
INCLUDEPATH += $$PWD/googletest/googletest
INCLUDEPATH += $$PWD/googletest/googlemock

SOURCES += \
    $$PWD/googletest/googletest/src/gtest-all.cc \
    $$PWD/googletest/googlemock/src/gmock-all.cc \



SOURCES += \
    test_login_chat.cpp\
    GroupManager.cpp \
    groupchatwindow.cpp \
    NetworkClient.cpp \
    chatmanager.cpp \
    chatwindow.cpp \
    groupwindow.cpp \
    loginmanager.cpp \
    loginwindow.cpp \
    main.cpp \
    mainwindow.cpp \
    test_groupmanager.cpp \
    userlist.cpp

HEADERS += \
    GroupManager.h \
    INetworkClient.h \
    NetworkClient.h \
    groupchatwindow.h \
    chatmanager.h \
    chatwindow.h \
    groupwindow.h \
    loginmanager.h \
    loginwindow.h \
    mainwindow.h \
    main.cpp \
    userlist.h

FORMS += \
    groupchatwindow.ui \
    chatwindow.ui \
    groupwindow.ui \
    loginwindow.ui \
    mainwindow.ui \
    userlist.ui

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target
