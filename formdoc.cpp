#include "formdoc.h"
#include "ui_formdoc.h"

FormDoc::FormDoc(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormDoc)
{
    ui->setupUi(this);

    loadsettings();

    uploadmanager = new QNetworkAccessManager(this);
    downloadmanager = new QNetworkAccessManager(this);
    currentFname=QString("currentsert.pdf");

    modelElTypes = new DbTableModel("zvd_types",this);
    modelElTypes->addColumn("id_sert","id_sert");
    modelElTypes->addColumn("id_tip",QString::fromUtf8("Тип"),NULL,Rels::instance()->relElTypes);
    modelElTypes->setSort("zvd_types.id_tip");
    ui->tableViewVid->setModel(modelElTypes);
    ui->tableViewVid->setColumnHidden(0,true);
    ui->tableViewVid->setColumnWidth(1,120);

    modelEl = new DbTableModel("zvd_els",this);
    modelEl->addColumn("id_sert","id_sert");
    modelEl->addColumn("id_el",QString::fromUtf8("Марка"),NULL,Rels::instance()->relElMark);
    modelEl->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelEl->setSuffix("inner join elrtr as e on e.id=zvd_els.id_el");
    modelEl->setSort("e.marka");
    ui->tableViewEl->setModel(modelEl);
    ui->tableViewEl->setColumnHidden(0,true);
    ui->tableViewEl->setColumnWidth(1,110);
    ui->tableViewEl->setColumnWidth(2,200);

    modelElDim = new DbTableModel("zvd_eldim",this);
    modelElDim->addColumn("id_sert","id_sert");
    modelElDim->addColumn("id_eldr",QString::fromUtf8("Марка / диам."),NULL,Rels::instance()->relElDim);
    modelElDim->addColumn("id_grade",QString::fromUtf8("Категория"),NULL,Rels::instance()->relGrade);
    modelElDim->setSuffix("inner join dry_els as d on d.ide=zvd_eldim.id_eldr");
    modelElDim->setSort("d.fnam");
    ui->tableViewElDim->setModel(modelElDim);
    ui->tableViewElDim->setColumnHidden(0,true);
    ui->tableViewElDim->setColumnWidth(1,130);
    ui->tableViewElDim->setColumnWidth(2,200);

    ui->tableVieewGrade->setModel(Rels::instance()->modelGrade);
    ui->tableVieewGrade->setColumnHidden(0,true);
    ui->tableVieewGrade->setColumnWidth(1,250);

    modelDoc = new DbTableModel("zvd_sert",this);
    modelDoc->addColumn("id","id");
    modelDoc->addColumn("nom_doc",QString::fromUtf8("Номер документа"));
    modelDoc->addColumn("nazv",QString::fromUtf8("Название документа"));
    modelDoc->addColumn("dat_doc",QString::fromUtf8("Дата документа"));
    modelDoc->addColumn("dat_beg",QString::fromUtf8("Дата начала"));
    modelDoc->addColumn("dat_end",QString::fromUtf8("Дата конца"));
    modelDoc->addColumn("nom_bl",QString::fromUtf8("Номер бланка"));
    modelDoc->addColumn("nom_sl",QString::fromUtf8("Номер наклейки"));
    modelDoc->addColumn("txt",QString::fromUtf8("Текст документа"));
    modelDoc->addColumn("gr_tech_ust",QString::fromUtf8("ГТУ"));
    modelDoc->addColumn("id_ved",QString::fromUtf8("Ведомство"),NULL,Rels::instance()->relVed);
    modelDoc->addColumn("id_doc",QString::fromUtf8("Тип документа"),NULL,Rels::instance()->relVidDoc);

    modelDoc->setSort("zvd_sert.nom_doc, zvd_sert.dat_doc");
    selectDoc(ui->checkBoxActive->isChecked());

    ui->tableViewDoc->setModel(modelDoc);
    ui->tableViewDoc->setColumnHidden(0,true);
    ui->tableViewDoc->setColumnWidth(1,200);
    for (int i=2; i<modelDoc->rowCount(); i++){
        ui->tableViewDoc->setColumnHidden(i,true);
    }

    mapper = new DbMapper(ui->tableViewDoc,this);
    mapper->addMapping(ui->lineEditDoc,1);
    mapper->addMapping(ui->lineEditNam,2);
    mapper->addMapping(ui->dateEditDoc,3);
    mapper->addMapping(ui->dateEditBeg,4);
    mapper->addMapping(ui->dateEditEnd,5);
    mapper->addMapping(ui->lineEditNumBl,6);
    mapper->addMapping(ui->lineEditNumNak,7);
    mapper->addMapping(ui->textEditDoc,8);
    mapper->addMapping(ui->lineEditGtu,9);
    mapper->addMapping(ui->comboBoxVed,10);
    mapper->addMapping(ui->comboBoxVid,11);

    mapper->addEmptyLock(ui->tableViewVid);
    mapper->addEmptyLock(ui->tableViewEl);
    mapper->addEmptyLock(ui->tableViewElDim);
    ui->horizontalLayoutMap->insertWidget(0,mapper);

    connect(mapper,SIGNAL(currentIndexChanged(int)),this,SLOT(refreshData(int)));
    connect(ui->checkBoxActive,SIGNAL(clicked(bool)),this,SLOT(selectDoc(bool)));
    connect(ui->cmdUpd,SIGNAL(clicked(bool)),this,SLOT(updElDim()));

    connect(ui->pushButtonUpload,SIGNAL(clicked(bool)),this,SLOT(upload()));
    connect(ui->pushButtonView,SIGNAL(clicked(bool)),this,SLOT(viewSert()));
    connect(uploadmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(uploadFinished(QNetworkReply*)));
    connect(downloadmanager,SIGNAL(finished(QNetworkReply*)),this,SLOT(downloadFinished(QNetworkReply*)));
    connect(ui->pushButtonSaveAs,SIGNAL(clicked(bool)),this,SLOT(saveAs()));


    if (ui->tableViewDoc->model()->rowCount()){
        ui->tableViewDoc->selectRow(0);
    }
}

FormDoc::~FormDoc()
{
    savesettings();
    delete ui;
}

void FormDoc::loadsettings()
{
    QSettings settings("szsm", "el_sert");
    ui->splitter->restoreState(settings.value("doc_splitter").toByteArray());
}

void FormDoc::savesettings()
{
    QSettings settings("szsm", "el_sert");
    settings.setValue("doc_splitter", ui->splitter->saveState());
}

void FormDoc::setOk(bool ok)
{
    ui->pushButtonSaveAs->setEnabled(ok);
    ui->pushButtonView->setEnabled(ok);
    QString pixname= ok ? QString(":/images/ok.png") : QString(":/images/notok.png");
    ui->labelSert->setPixmap(QPixmap(pixname));
}

int FormDoc::currentId()
{
    return ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(ui->tableViewDoc->currentIndex().row(),0),Qt::EditRole).toInt();
}

QString FormDoc::currentDocNumber()
{
    return ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(ui->tableViewDoc->currentIndex().row(),1),Qt::EditRole).toString();
}

void FormDoc::refreshData(int index)
{
    int id=ui->tableViewDoc->model()->data(ui->tableViewDoc->model()->index(index,0),Qt::EditRole).toInt();

    modelElTypes->setDefaultValue(0,id);
    modelElTypes->setFilter("zvd_types.id_sert = "+QString::number(id));
    modelElTypes->select();

    modelEl->setDefaultValue(0,id);
    modelEl->setFilter("zvd_els.id_sert = "+QString::number(id));
    modelEl->select();

    modelElDim->setDefaultValue(0,id);
    modelElDim->setFilter("zvd_eldim.id_sert = "+QString::number(id));
    modelElDim->select();

    download();
}

void FormDoc::selectDoc(bool active)
{
    if (active){
        modelDoc->setFilter("COALESCE(zvd_sert.dat_end,'3000-01-01'::date) >= '"+QDate::currentDate().toString("yyyy-MM-dd")+"'");
    } else {
        modelDoc->setFilter("");
    }
    modelDoc->select();
}

void FormDoc::updElDim()
{
    QSqlQuery query;
    query.prepare("select * from rx_els()");
    if (query.exec()){
        Rels::instance()->relElDim->refreshModel();
    } else {
        QMessageBox::critical(this,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}

void FormDoc::upload()
{
    QSettings settings("szsm", QApplication::applicationName());
    QDir dir(settings.value("savePath",QDir::homePath()).toString());
    QString filename=QFileDialog::getOpenFileName(this, QString::fromUtf8("Открыть файл"),dir.path(),tr("pdf (*.pdf)"));
    QFile *file = new QFile(filename);
    if (file->open(QIODevice::ReadOnly)){
        QFileInfo info(*file);
        settings.setValue("savePath",info.path());

        QUrl url(QString("ftp://192.168.1.10/pub/sert/")+QString::number(currentId())+QString(".pdf"));
        url.setUserName("sert");
        url.setPassword("sert");
        url.setPort(21);
        QNetworkReply *reply = uploadmanager->put(QNetworkRequest(url),file);
        connect(reply,&QNetworkReply::finished,[this,file](){
            qDebug()<<"delete_file!";
            file->close();
            QFile f(this->currentFname);
            if (f.exists()){
                f.remove();
            }
            file->copy(f.fileName());
            file->deleteLater();
        });
    }
}

void FormDoc::download()
{
    QFile file(currentFname);
    if (file.exists()){
        file.remove();
    }
    QUrl url(QString("ftp://192.168.1.10/pub/sert/")+QString::number(currentId())+QString(".pdf"));
    url.setUserName("sert");
    url.setPassword("sert");
    url.setPort(21);
    downloadmanager->get(QNetworkRequest(url));

}

void FormDoc::viewSert()
{
    QFile f(currentFname);
    if (f.exists()){
        QFileInfo fileInfo(f);
        QDesktopServices::openUrl((QUrl(QUrl::fromLocalFile(fileInfo.absoluteFilePath()))));
    }
}

void FormDoc::uploadFinished(QNetworkReply *reply)
{
    //qDebug()<<"upload finished! ";
    bool ok=reply->error()==QNetworkReply::NoError;
    if (!ok){
        QMessageBox::critical(this,tr("Error"),reply->errorString(),QMessageBox::Ok);
        qDebug()<<reply->errorString();
    }
    setOk(ok);
    reply->deleteLater();
}

void FormDoc::downloadFinished(QNetworkReply *reply)
{
    //qDebug()<<"download finished! ";
    bool ok=reply->error()==QNetworkReply::NoError;
    if (ok){
        QFile file(currentFname);
        if (file.open(QIODevice::WriteOnly)){
            file.write(reply->readAll());
            file.close();
        }
    }
    setOk(ok);
    reply->deleteLater();
}

void FormDoc::saveAs()
{
    QString filename=QFileDialog::getSaveFileName(this, QString::fromUtf8("Сохранить файл"),QDir::homePath()+QString("/")+currentDocNumber()+".pdf",tr("pdf (*.pdf)"));
    if (!filename.isEmpty()){
        QFile file(currentFname);
        file.copy(filename);
    }
}
