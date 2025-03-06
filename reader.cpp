#include "reader.h"
#include "ui_reader.h"

Reader::Reader(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::Reader)
{
    ui->setupUi(this);
    id_ship=-1;
    doc=nullptr;

    connect(ui->radioButtonRus,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonEng,SIGNAL(clicked(bool)),this,SLOT(reload()));
    connect(ui->radioButtonMix,SIGNAL(clicked(bool)),this,SLOT(reload()));

    connect(ui->pushButtonPrint,SIGNAL(clicked(bool)),this,SLOT(print()));
    connect(ui->pushButtonPDF,SIGNAL(clicked(bool)),this,SLOT(save()));
    connect(ui->spinBoxScale,SIGNAL(valueChanged(int)),this,SLOT(reRender()));
}

Reader::~Reader()
{
    if (doc){
        delete doc;
    }
    delete ui;
}



void Reader::setCurrentIdShip(int id, QString fname)
{
    id_ship=id;
    name=fname;
    reload();
}

QString Reader::getCurrentLang()
{
    QString lang="mix";
    if (ui->radioButtonRus->isChecked()){
        lang="ru";
    } else if (ui->radioButtonEng->isChecked()){
        lang="en";
    }
    return lang;
}

void Reader::reload()
{
    data.clear();
    HttpSyncManager::sendGet(Rels::instance()->appServer()+"/s3/local/"+QString::number(id_ship)+"/"+getCurrentLang(),data);
    if (doc){
        delete doc;
        doc=nullptr;
    }
    if (data.size()){
        doc = Poppler::Document::loadFromData(data);
        if (doc){
            doc->setRenderHint(Poppler::Document::TextAntialiasing);
        }
    }
    reRender();
}

void Reader::print()
{
    if(!doc){
        return;
    }
    QPrinter printer(QPrinter::HighResolution);
    QPrintDialog printDialog(&printer, this);
    if (printDialog.exec()) {
        QPainter painter(&printer);
        Poppler::Page *page = doc->page(0);
        if (page){
            QImage img = page->renderToImage(300,300);
            painter.drawImage(painter.window(),img);
            delete page;
        }
    }
}

void Reader::save()
{
    if (!data.size()){
        return;
    }
    QSettings settings("szsm", QApplication::applicationName());
    QDir dir(settings.value("sertPath",QDir::homePath()).toString());
    QString exportname = QFileDialog::getSaveFileName(this,tr("Сохранить PDF"),dir.path()+"/"+name+".pdf", "*.pdf");
    if (!exportname.isEmpty()) {
        QFile file(exportname);
        QFileInfo info(file);
        settings.setValue("sertPath",info.path());
        if (file.open(QFile::WriteOnly)){
            file.write(data);
            file.close();
        }
    }
}

void Reader::reRender()
{
    if (doc){
        Poppler::Page *page = doc->page(0);
        if (page){
            double scale=ui->spinBoxScale->value()/100.0;
            QImage img = page->renderToImage(QApplication::desktop()->physicalDpiX()*scale,QApplication::desktop()->physicalDpiY()*scale);
            ui->label->setPixmap(QPixmap::fromImage(img));
            delete page;
        } else {
            ui->label->clear();
        }
    } else {
        ui->label->clear();
    }
}
