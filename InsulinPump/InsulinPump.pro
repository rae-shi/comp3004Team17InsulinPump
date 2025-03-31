QT += core gui widgets testlib

# Local SDK fix (only for macOS 15.2+):
# COMMENT THIS OUT IF YOU ARE NOT USING MACOS
QMAKE_CXXFLAGS += -I/Library/Developer/CommandLineTools/SDKs/MacOSX15.2.sdk/usr/include/c++/v1

CONFIG += c++11

SOURCES += \
    insulinpump.cpp \
    main.cpp \
    mainwindow.cpp \
    tests.cpp

HEADERS += \
    insulinpump.h \
    mainwindow.h

FORMS += \
    mainwindow.ui

DISTFILES += \
    .gitignore
