#ifndef FORMDOC_H
#define FORMDOC_H

#include <QWidget>
#include "db/dbmapper.h"
#include "db/dbviewer.h"
#include "rels.h"
#include <QNetworkAccessManager>
#include <QNetworkReply>
#include <QFileDialog>

namespace Ui {
class FormDoc;
}

class FormDoc : public QWidget
{
    Q_OBJECT

public:
    explicit FormDoc(QWidget *parent = 0);
    ~FormDoc();

private:
    Ui::FormDoc *ui;
    DbTableModel *modelDoc;
    DbTableModel *modelElTypes;
    DbTableModel *modelEl;
    DbTableModel *modelElDim;
    DbMapper *mapper;
    QNetworkAccessManager *uploadmanager;
    QNetworkAccessManager *downloadmanager;
    void loadsettings();
    void savesettings();
    void setOk(bool ok);
    int currentId();
    QString currentDocNumber();
    QString currentFname;

private slots:
    void refreshData(int index);
    void selectDoc(bool active);
    void updElDim();
    void upload();
    void download();
    void viewSert();
    void uploadFinished(QNetworkReply *reply);
    void downloadFinished(QNetworkReply *reply);
    void saveAs();
};

#endif // FORMDOC_H
