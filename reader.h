#ifndef READER_H
#define READER_H

#include <QWidget>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include "rels.h"
//#include <poppler/qt5/poppler-qt5.h>
#include <QApplication>
#include <QDesktopWidget>
#include "httpsyncmanager.h"
#include <QFile>
#include <QSettings>
#include <QFileDialog>
#include <QDir>
#include <QProgressDialog>

namespace Ui {
class Reader;
}

class Reader : public QWidget
{
    Q_OBJECT

public:
    explicit Reader(QWidget *parent = nullptr);
    ~Reader();
    void setCurrentIdShip(int id, QString fname);
    QString getCurrentLang();

public slots:
    void reload();
    void setLang(QString lang);

private:
    Ui::Reader *ui;
    int id_ship;
    QString name;
    QNetworkAccessManager *manager;

private slots:
    void print();
    void save();
    void replyFinished();
    void setLock(bool b);
};

#endif // READER_H
