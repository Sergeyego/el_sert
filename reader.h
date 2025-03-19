#ifndef READER_H
#define READER_H

#include <QWidget>
#include <QPrintDialog>
#include <QPrinter>
#include <QPainter>
#include "rels.h"
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
    void setCurrentIdShip(int id, QString fname, QString prefix);
    QString getCurrentLang();

public slots:
    void reload();
    void setLang(QString lang);
    void clear();

private:
    Ui::Reader *ui;
    int id_ship;
    QString name;
    QString spref;
    QNetworkAccessManager *manager;

private slots:
    void print();
    void save();
    void replyFinished();
    void setLock(bool b);
};

#endif // READER_H
