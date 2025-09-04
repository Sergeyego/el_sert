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
    void insRab(QString pattern, QLineEdit *title, QLineEdit *name);
};

#endif // DIALOGAUTHOR_H
