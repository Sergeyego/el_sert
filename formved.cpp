#include "formved.h"
#include "ui_formved.h"

FormVed::FormVed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVed)
{
    ui->setupUi(this);

    modelVed = new DbTableModel("zvd_ved",this);
    modelVed->addColumn("id","id");
    modelVed->addColumn("nam",QString::fromUtf8("Название"));
    modelVed->addColumn("short",QString::fromUtf8("Кратеое название"));
    modelVed->addColumn("fnam",QString::fromUtf8("Полное название"));
    modelVed->addColumn("fnam_en",QString::fromUtf8("Полное название анг."));
    modelVed->addColumn("short_en",QString::fromUtf8("Кратеое название анг."));
    modelVed->setSort("zvd_ved.nam");
    modelVed->select();

    ui->tableViewVed->setModel(modelVed);
    ui->tableViewVed->setColumnHidden(0,true);
    ui->tableViewVed->setColumnWidth(1,170);
    ui->tableViewVed->setColumnWidth(2,170);
    ui->tableViewVed->setColumnWidth(3,530);
    ui->tableViewVed->setColumnWidth(4,530);
    ui->tableViewVed->setColumnWidth(5,170);

    modelDoc = new DbTableModel("zvd_doc",this);
    modelDoc->addColumn("id","id");
    modelDoc->addColumn("nam",QString::fromUtf8("Название"));
    modelDoc->addColumn("fnam",QString::fromUtf8("Полное название"));
    modelDoc->addColumn("id_doc_type",QString::fromUtf8("Тип"),Rels::instance()->relDocType);
    modelDoc->addColumn("fnam_en",QString::fromUtf8("Полное название анг."));
    modelDoc->setSort("zvd_doc.nam");
    modelDoc->select();
    ui->tableViewDocVid->setModel(modelDoc);
    ui->tableViewDocVid->setColumnHidden(0,true);
    ui->tableViewDocVid->setColumnWidth(1,230);
    ui->tableViewDocVid->setColumnWidth(2,480);
    ui->tableViewDocVid->setColumnWidth(3,150);
    ui->tableViewDocVid->setColumnWidth(4,470);

    connect(ui->tableViewVed->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(loadImg(QModelIndex)));
    connect(ui->cmdBrowse,SIGNAL(clicked(bool)),this,SLOT(newImg()));
    connect(ui->cmdDel,SIGNAL(clicked(bool)),this,SLOT(delImg()));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));

    if (ui->tableViewVed->model()->rowCount()){
        ui->tableViewVed->selectRow(0);
    }
}

FormVed::~FormVed()
{
    delete ui;
}

void FormVed::viewPix(const QPixmap &pix)
{
    if (pix.isNull()){
        ui->labelSimb->setPixmap(pix);
    } else {
        ui->labelSimb->setPixmap(pix.scaled(ui->labelSimb->size(),Qt::KeepAspectRatio));
    }
}

void FormVed::loadImg(QModelIndex index)
{
    ui->labelSimb->setPixmap(QPixmap());
    int id=ui->tableViewVed->model()->data(ui->tableViewVed->model()->index(index.row(),0),Qt::EditRole).toInt();
    QPixmap pix;
    pix.loadFromData(Rels::instance()->mapVedPix.value(id));
    if (!pix.isNull()){
        viewPix(pix);
    }
    ui->cmdDel->setEnabled(!pix.isNull());
}

void FormVed::newImg()
{
    QDir dir(QDir::homePath());
    QString filename = QFileDialog::getOpenFileName(nullptr,QString::fromUtf8("Выберите изображение"),
                                                    dir.path(),
                                                    QString::fromUtf8("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
    QImage img(filename);
    if (!img.isNull()){
        int row=ui->tableViewVed->currentIndex().row();
        int id=ui->tableViewVed->model()->data(ui->tableViewVed->model()->index(row,0),Qt::EditRole).toInt();
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer,"PNG");
        buffer.close();

        QSqlQuery query;
        query.prepare("update zvd_ved set simb = :data where id= :id");
        query.bindValue(":id",id);
        query.bindValue(":data",arr);
        if (query.exec()){
            viewPix(QPixmap::fromImage(img));
            ui->cmdDel->setEnabled(!img.isNull());
            Rels::instance()->refreshVedPix();
        } else {
            QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormVed::delImg()
{
    int row=ui->tableViewVed->currentIndex().row();
    int id=ui->tableViewVed->model()->data(ui->tableViewVed->model()->index(row,0),Qt::EditRole).toInt();
    QSqlQuery query;
    query.prepare("update zvd_ved set simb = NULL where id= :id");
    query.bindValue(":id",id);
    if (query.exec()){
        viewPix(QPixmap());
        Rels::instance()->refreshVedPix();
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormVed::upd()
{
    Rels::instance()->refreshVedPix();
    modelVed->select();
    modelDoc->select();
    modelDoc->refreshRelsModel();
    if (ui->tableViewVed->model()->rowCount()){
        ui->tableViewVed->selectRow(0);
    }
}
