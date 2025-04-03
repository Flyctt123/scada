QT       += core gui widgets xml mqtt

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = runtime
TEMPLATE = app

# 使用 qmake 内置变量获取 Qt 安装路径
QTDIR = $$[QT_INSTALL_PREFIX]

# 添加 Qt 头文件路径
INCLUDEPATH += $$[QT_INSTALL_HEADERS]
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtCore
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtGui
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtWidgets
INCLUDEPATH += $$[QT_INSTALL_HEADERS]/QtXml

# 确保使用正确的库文件
LIBS += -L$$QTDIR/lib

SOURCES += \
    main.cpp \
    runtimeviewer.cpp \
    mqttcomm.cpp

HEADERS += \
    runtimeviewer.h \
    mqttcomm.h

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated
DEFINES += QT_DEPRECATED_WARNINGS

# You can make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target 