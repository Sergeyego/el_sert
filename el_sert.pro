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
    dialogauthor.cpp \
    dialogsignature.cpp \
        formpart.cpp \
    formpartwire.cpp \
    formsigpasport.cpp \
    httpsyncmanager.cpp \
    editor.cpp \
    pasportmanager.cpp \
    progressreportdialog.cpp \
    reader.cpp \
    sertbuild.cpp \
    checkform.cpp \
    db/dbdelegate.cpp \
    db/dblogin.cpp \
    db/dbmapper.cpp \
    db/dbtablemodel.cpp \
    db/dbviewer.cpp \
    mainwindow.cpp \
    formship.cpp \
    formved.cpp \
    formdoc.cpp \
    rels.cpp \
    modelstu.cpp \
    formmark.cpp \
    formpos.cpp \
    rtf/rtfcpp.cpp \
    lblcreator.cpp \
    export.cpp \
    qftp/qftp.cpp \
    qftp/qurlinfo.cpp \
    modeldoc.cpp \
    dialogcopy.cpp \
    modelro.cpp \
    db/executor.cpp \
    db/dbcombobox.cpp \
    db/dbdateedit.cpp \
    db/dbrelationeditdialog.cpp \
    db/tablemodel.cpp \
    tableview.cpp

HEADERS  += formpart.h \
    dialogauthor.h \
    dialogsignature.h \
    formpartwire.h \
    formsigpasport.h \
    httpsyncmanager.h \
    editor.h \
    pasportmanager.h \
    progressreportdialog.h \
    reader.h \
    sertbuild.h \
    checkform.h \
    db/dbviewer.h \
    db/dbdelegate.h \
    db/dblogin.h \
    db/dbmapper.h \
    db/dbtablemodel.h \
    mainwindow.h \
    formship.h \
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
    qftp/qftp.h \
    qftp/qurlinfo.h \
    modeldoc.h \
    dialogcopy.h \
    modelro.h \
    db/executor.h \
    db/dbcombobox.h \
    db/dbdateedit.h \
    db/dbrelationeditdialog.h \
    db/tablemodel.h \
    tableview.h

FORMS    += formpart.ui \
    dialogauthor.ui \
    dialogsignature.ui \
    editor.ui \
    checkform.ui \
    db/dblogin.ui \
    formpartwire.ui \
    formsigpasport.ui \
    mainwindow.ui \
    formship.ui \
    formved.ui \
    formdoc.ui \
    formmark.ui \
    formpos.ui \
    dialogcopy.ui \
    db/dbrelationeditdialog.ui \
    progressreportdialog.ui \
    reader.ui

RESOURCES += \
    res.qrc

win32:RC_FILE = ico.rc

DISTFILES += \
    ico.rc

macx:ICON = ico.icns
