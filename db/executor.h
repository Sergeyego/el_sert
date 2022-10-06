#ifndef EXECUTOR_H
#define EXECUTOR_H

#include <QThread>
#include <QMessageBox>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlError>
#include <QSqlRecord>
#include <QDebug>
#include <QReadWriteLock>

class ModelPrg;

class Executor : public QThread
{
    Q_OBJECT
public:
    explicit Executor(QObject *parent=nullptr);
    ~Executor();
    void run();
    void setQuery(QString qu);
    QVector<QVector<QVariant>> getData();

private:
    QString query;
    QVector<QVector<QVariant>> data;
    QString databaseName;
    QString hostName;
    int port;
    QString userName;
    QString password;
    QReadWriteLock dataLock;
    QReadWriteLock queryLock;

private slots:
    void showError(QString text);
signals:
    void sigError(QString mes);
};

#endif // EXECUTOR_H
