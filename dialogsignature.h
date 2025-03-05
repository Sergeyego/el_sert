#ifndef DIALOGSIGNATURE_H
#define DIALOGSIGNATURE_H

#include <QDialog>
#include <QJsonDocument>
#include <QJsonObject>
#include <QJsonArray>
#include "httpsyncmanager.h"

namespace Ui {
class DialogSignature;
}

class DialogSignature : public QDialog
{
    Q_OBJECT

public:
    explicit DialogSignature(QString srv, QWidget *parent = nullptr);
    ~DialogSignature();
    int exec();
    QString getSN();

private:
    Ui::DialogSignature *ui;
    QJsonArray certs;
    QString server;

private slots:
    void setCurrentCert(int i);
};

#endif // DIALOGSIGNATURE_H
