#ifndef CHECKFORM_H
#define CHECKFORM_H

#include <QWidget>
#include <QtSql>
#include <QMessageBox>

namespace Ui {
class CheckForm;
}

class CheckForm : public QWidget
{
    Q_OBJECT

public:
    explicit CheckForm(QWidget *parent = 0);
    ~CheckForm();
    void closeEvent(QCloseEvent *e);
    void setCheckEl(bool b);

private:
    Ui::CheckForm *ui;
    QString checkEl(int id_part, int id_ship);
    QString checkWire(int id_part, int id_ship);
private slots:
    void go();

signals:
    void sigClose();
};

#endif // CHECKFORM_H
