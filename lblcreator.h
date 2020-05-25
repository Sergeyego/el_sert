#ifndef LBLCREATOR_H
#define LBLCREATOR_H

#include <QObject>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QFileInfo>
#include <QDesktopServices>
#include <QUrl>
#include "rtf/rtfcpp.h"
#include "rels.h"

class LblCreator : public QObject
{
    Q_OBJECT
public:
    explicit LblCreator(QObject *parent = nullptr);
    bool createLbl(int id_part, bool shortAmp=true);
    bool createLbl(int id_el, int id_diam, QString ibco=QString(), QDate date=QDate::currentDate(), bool shortAmp=true, bool order=false);

private:
    QString orgNam;
    QString adres;
    QString otk;
    QString kach;
    void sysCommand(QString fname);

signals:

public slots:
};

#endif // LBLCREATOR_H
