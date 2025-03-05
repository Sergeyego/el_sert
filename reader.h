#ifndef READER_H
#define READER_H

#include <QWidget>
#include <QWebEngineView>
#include <QWebEngineSettings>
#include <QPrintDialog>
#include <QPrinter>
#include "rels.h"

namespace Ui {
class Reader;
}

class Reader : public QWidget
{
    Q_OBJECT

public:
    explicit Reader(QWidget *parent = nullptr);
    ~Reader();
    void setCurrentIdShip(int id);
    QString getCurrentLang();

public slots:
    void reload();

private:
    Ui::Reader *ui;
    QWebEngineView *view;
    int id_ship;

private slots:
    void print();
};

#endif // READER_H
