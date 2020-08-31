#include "mainwindow.h"
#include <QApplication>
#include "pgdialog.h"
#include <QDir>
#include <QFileInfo>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    a.setWindowIcon(QIcon(":/images/ico.ico"));

    QString key;
    if (argc>1){
        key=QString(argv[1]);
    }

    PgDialog d(QString::fromUtf8("Сертификаты качества"),key);
    if (d.exec()!=QDialog::Accepted) exit(1);

    QDir dir(":fonts");
    QFileInfoList list = dir.entryInfoList();
    foreach (QFileInfo i, list) {
        QFontDatabase::addApplicationFont(i.absoluteFilePath());
    }

    MainWindow w(d.key()!=QString("srt"));
    w.show();

    return a.exec();
}
