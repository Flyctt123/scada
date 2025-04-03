QT       += core gui widgets xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

TARGET = scada
TEMPLATE = app

# Windows 特定设置
win32 {
    # 获取 QTDIR 环境变量
    QTDIR = $$(QTDIR)
    isEmpty(QTDIR) {
        # 如果环境变量未设置，使用默认路径
        QTDIR = C:/Qt/Qt5.12.12/5.12.12/msvc2017_64
    }
    
    # 添加 Qt 头文件路径
    INCLUDEPATH += $$QTDIR/include
    INCLUDEPATH += $$QTDIR/include/QtCore
    INCLUDEPATH += $$QTDIR/include/QtGui
    INCLUDEPATH += $$QTDIR/include/QtWidgets
    INCLUDEPATH += $$QTDIR/include/QtXml
    
    # 添加库文件路径
    LIBS += -L$$QTDIR/lib
}

# Unix 系统特定设置
unix {
    # 添加动态库支持
    LIBS += -ldl
}

SOURCES += \
    main.cpp \
    mainwindow.cpp \
    customview.cpp \
    xmlconfig.cpp \
    variablebindingdialog.cpp \
    componentfactory.cpp \
    componentdesigner.cpp

HEADERS += \
    mainwindow.h \
    customview.h \
    xmlconfig.h \
    variablebindingdialog.h \
    componentfactory.h \
    componentdesigner.h

FORMS += \
    mainwindow.ui

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

INCLUDEPATH += $$PWD/../common
