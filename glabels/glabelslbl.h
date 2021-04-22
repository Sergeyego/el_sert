#ifndef GLABELSLBL_H
#define GLABELSLBL_H

#include <QObject>
#include <QMessageBox>
#include <QVariant>
#include <QDate>
#include <QDomDocument>
#include <QFile>
#include <QDebug>
#include <QBuffer>
#include <QTextDocument>
#include <QDir>

class GlabelsLbl : public QObject
{
    Q_OBJECT
public:
    explicit GlabelsLbl(QObject *parent = nullptr);
    bool createLbl(QString templates, bool rotate);
    void newImage(double x, double y, double w, double h, QString fnam);
    void newBarcode(double x, double y, double w, double h, QString code);
    void newText(double x, double y, double w, double h, QString text, int size, bool bold, Qt::Alignment al = (Qt::AlignLeft | Qt::AlignVCenter));
    void newRect(double x, double y, double w, double h);
    void newLine(double x, double y, double dx, double dy);
    bool saveFile(QString name);
private:
    QDomDocument doc;
    QDomElement root;
    QDomElement obj;
    QMap <QString,QDomElement> map;

signals:
public slots:
};

#endif // GLABELSLBL_H
