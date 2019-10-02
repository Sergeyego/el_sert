#include "mainwindow.h"
#include <QApplication>
#include "pgdialog.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/el.png"));

    QString key;
    if (argc>1){
        key=QString(argv[1]);
    }

    PgDialog d(QString::fromUtf8("Сертификаты качества"),key);
    if (d.exec()!=QDialog::Accepted) exit(1);

    MainWindow w(d.key()!=QString("srt"));
    w.show();

    return a.exec();
}
