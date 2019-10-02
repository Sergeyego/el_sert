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

private:
    Ui::CheckForm *ui;
private slots:
    void go();

signals:
    void sigClose();
};

#endif // CHECKFORM_H
