#ifndef EXPORT_H
#define EXPORT_H

#include <QObject>
#include <QDomDocument>
#include <QFile>
#include <QTextCodec>
#include <QTextStream>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QDate>
#include <QFileDialog>
#include <QDebug>

class Export: public QObject
{
public:
    Export(QObject *parent = nullptr);
    void createXml();
private:
    QDomElement getMark(int id_el,  QDomDocument *doc);
    QDomElement getWire(int id_pr,  QDomDocument *doc);
    QDomElement getSert(int id_sert,  QDomDocument *doc);
    QDomElement getSertMark(int id_sert,  QDomDocument *doc);
    QDomElement getSertWire(int id_sert,  QDomDocument *doc);
    QDomElement newElement(QString nam, QString val,  QDomDocument *doc);
    QDomElement getTu(int id_el,  QDomDocument *doc);
    QDomElement getWireTu(int id_pr,  QDomDocument *doc);
    QDomElement getAmp(int id_el,  QDomDocument *doc);
    QDomElement getPlav(int id_el,  QDomDocument *doc);
    QDomElement getChem(int id_pr,  QDomDocument *doc);
    QDomElement getWireChem(int id_el,  QDomDocument *doc);
    QDomElement getMech(int id_el,  QDomDocument *doc);
    QString testStr(const QString &s);
    QString fromDouble(const QVariant &v, int d=1);
    QString fromDate(const QDate &d);
};

#endif // EXPORT_H
