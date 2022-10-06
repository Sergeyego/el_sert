#include "executor.h"

Executor::Executor(QObject *parent) :
    QThread(parent)
{
    QSqlDatabase db=QSqlDatabase::database();
    databaseName=db.databaseName();
    hostName=db.hostName();
    port=db.port();
    userName=db.userName();
    password=db.password();
    connect(this,SIGNAL(sigError(QString)),this,SLOT(showError(QString)));
}

Executor::~Executor()
{

}

void Executor::run()
{
    int randInt;
    QString randomName;
    for(int i=0; i<5; i++){
        randInt = rand()%('Z'-'A'+1)+'A';
        randomName.append(randInt);
    }
    //qDebug() << randomName;
    {
        QSqlDatabase db=QSqlDatabase::addDatabase("QPSQL",randomName);
        db.setDatabaseName(databaseName);
        db.setHostName(hostName);
        db.setPort(port);
        db.setUserName(userName);
        db.setPassword(password);
        if (db.open()){
            QSqlQuery qu(db);
            queryLock.lockForRead();
            qu.prepare(query);
            queryLock.unlock();
            if (qu.exec()){
                int colCount=qu.record().count();
                QWriteLocker locker(&dataLock);
                data.clear();
                while (qu.next()){
                    QVector<QVariant> dt;
                    for (int i=0; i<colCount; i++){
                        dt.push_back(qu.value(i));
                    }
                    data.push_back(dt);
                }
            } else {
                emit sigError(qu.lastError().text());
            }
        } else {
            emit sigError(db.lastError().text());
        }
        if (db.isOpen()) db.close();
    }
    QSqlDatabase::removeDatabase(randomName);
}

void Executor::setQuery(QString qu)
{
    QWriteLocker locker(&queryLock);
    query=qu;
}

QVector<QVector<QVariant> > Executor::getData()
{
    QReadLocker locker(&dataLock);
    return data;
}

void Executor::showError(QString text)
{
    QMessageBox::critical(nullptr,tr("Ошибка"),text,QMessageBox::Cancel);
}
