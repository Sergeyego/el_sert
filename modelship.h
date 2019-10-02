#ifndef MODELSHIP_H
#define MODELSHIP_H

#include <QSqlQueryModel>
#include <QMessageBox>
#include <QDate>
#include <QSqlError>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QLocale>
#include <QPrintDialog>

class ModelShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelShip(QObject *parent = 0);
    void refresh(QDate begDate, QDate endDate);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
    
signals:
    
public slots:
    
};

class ModelDataShip : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelDataShip(QObject *parent = 0);
    void refresh(int id_ship);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

signals:

public slots:

};

class ModelPart : public QSqlQueryModel
{
    Q_OBJECT
public:
    explicit ModelPart (QObject *parent = 0);
    void refresh(QDate dbeg, QDate dend, int id_el);
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;
};

#endif // MODELSHIP_H
