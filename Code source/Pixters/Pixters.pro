QT       += core gui \
            multimedia \
            sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++17

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

INCLUDEPATH += "C:\Qt\6.2.4\mingw_64\plugins\sqldrivers"
LIBS += "C:\Qt\6.2.4\mingw_64\bin\libmysql.lib"

SOURCES += \
    Health.cpp \
    Hunger.cpp \
    Meter.cpp \
    Pet.cpp \
    Satisfaction.cpp \
    Session.cpp \
    main.cpp

HEADERS += \
    Health.h \
    Hunger.h \
    Meter.h \
    Pet.h \
    Satisfaction.h \
    Session.h

FORMS +=

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    resources.qrc
