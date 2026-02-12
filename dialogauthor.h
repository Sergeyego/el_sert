#ifndef DIALOGAUTHOR_H
#define DIALOGAUTHOR_H

#include <QDialog>
#include <QLineEdit>
#include <QSqlQuery>
#include <QSqlError>
#include <QMessageBox>
#include <QJsonDocument>
#include <QJsonObject>

namespace Ui {
class DialogAuthor;
}

class DialogAuthor : public QDialog
{
    Q_OBJECT

public:
    explicit DialogAuthor(QWidget *parent = nullptr);
    ~DialogAuthor();
    QByteArray getJson();

private:
    Ui::DialogAuthor *ui;
    QMap <int,QByteArray> mapSig;

private slots:
    void updInfo();
};

#endif // DIALOGAUTHOR_H
