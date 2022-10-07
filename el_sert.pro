#-------------------------------------------------
#
# Project created by QtCreator 2013-07-01T08:39:38
#
#-------------------------------------------------

QT       += core gui sql printsupport xml network

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
    formship.cpp \
    formnormdoc.cpp \
    formved.cpp \
    formdoc.cpp \
    rels.cpp \
    modelstu.cpp \
    formmark.cpp \
    formpos.cpp \
    rtf/rtfcpp.cpp \
    lblcreator.cpp \
    export.cpp \
    qr/qrencode.cpp \
    qftp/qftp.cpp \
    qftp/qurlinfo.cpp \
    modeldoc.cpp \
    glabels/glabelslbl.cpp \
    dialogcopy.cpp \
    modelro.cpp \
    db/executor.cpp \
    db/dbcombobox.cpp \
    db/dbdateedit.cpp \
    db/dbrelationeditdialog.cpp \
    db/tablemodel.cpp

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
    formship.h \
    formnormdoc.h \
    formved.h \
    formdoc.h \
    rels.h \
    modelstu.h \
    formmark.h \
    formpos.h \
    rtf/rtfcpp.h \
    rtf/rtfdefs.h \
    lblcreator.h \
    export.h \
    qr/qrencode.h \
    qftp/qftp.h \
    qftp/qurlinfo.h \
    modeldoc.h \
    glabels/glabelslbl.h \
    dialogcopy.h \
    modelro.h \
    db/executor.h \
    db/dbcombobox.h \
    db/dbdateedit.h \
    db/dbrelationeditdialog.h \
    db/tablemodel.h

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
    formpos.ui \
    dialogcopy.ui \
    db/dbrelationeditdialog.ui

RESOURCES += \
    res.qrc

RC_FILE = ico.rc

DISTFILES += \
    images/logo2.png \
    images/otk.png \
    ico.rc \
    templates/glabels_szsm.xml
