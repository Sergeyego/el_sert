#-------------------------------------------------
#
# Project created by QtCreator 2013-07-01T08:39:38
#
#-------------------------------------------------

QT       += core gui sql printsupport xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = el_sert
TEMPLATE = app

SOURCES += main.cpp\
        formpart.cpp \
    pgdialog.cpp \
    editor.cpp \
    modelship.cpp \
    sertbuild.cpp \
    checkform.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    mainwindow.cpp \
    tabwidget.cpp \
    formship.cpp \
    formnormdoc.cpp \
    formved.cpp \
    formdoc.cpp \
    rels.cpp \
    modelstu.cpp \
    formmark.cpp \
    formtypes.cpp \
    formpos.cpp \
    rtf/rtfcpp.cpp \
    lblcreator.cpp \
    export.cpp

HEADERS  += formpart.h \
    pgdialog.h \
    editor.h \
    modelship.h \
    sertbuild.h \
    checkform.h \
    db/dbviewer.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    mainwindow.h \
    tabwidget.h \
    formship.h \
    formnormdoc.h \
    formved.h \
    formdoc.h \
    rels.h \
    modelstu.h \
    formmark.h \
    formtypes.h \
    formpos.h \
    rtf/rtfcpp.h \
    rtf/rtfdefs.h \
    lblcreator.h \
    export.h

FORMS    += formpart.ui \
    pgdialog.ui \
    editor.ui \
    checkform.ui \
    db/dblogin.ui \
    mainwindow.ui \
    formship.ui \
    formnormdoc.ui \
    formved.ui \
    formdoc.ui \
    formmark.ui \
    formtypes.ui \
    formpos.ui

RESOURCES += \
    res.qrc

LIBS += -lqrencode