#include "formved.h"
#include "ui_formved.h"

FormVed::FormVed(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormVed)
{
    ui->setupUi(this);

    modelDocType = new DbTableModel("zvd_doc_type",this);
    modelDocType->addColumn("id","id");
    modelDocType->addColumn("nam",QString::fromUtf8("Наименование"));
    modelDocType->setSort("zvd_doc_type.nam");
    modelDocType->select();

    ui->tableViewDocType->setModel(modelDocType);
    ui->tableViewDocType->setColumnHidden(0,true);
    ui->tableViewDocType->setColumnWidth(1,140);

    ui->tableViewVed->setModel(Rels::instance()->modelVed);
    ui->tableViewVed->setColumnHidden(0,true);
    ui->tableViewVed->setColumnWidth(1,170);
    ui->tableViewVed->setColumnWidth(2,170);
    ui->tableViewVed->setColumnWidth(3,530);
    ui->tableViewVed->setColumnWidth(4,530);
    ui->tableViewVed->setColumnWidth(5,170);

    ui->tableViewDocVid->setModel(Rels::instance()->modelDoc);
    ui->tableViewDocVid->setColumnHidden(0,true);
    ui->tableViewDocVid->setColumnWidth(1,180);
    ui->tableViewDocVid->setColumnWidth(2,370);
    ui->tableViewDocVid->setColumnWidth(3,110);
    ui->tableViewDocVid->setColumnWidth(4,360);

    connect(ui->tableViewVed->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(loadImg(QModelIndex)));
    connect(ui->cmdBrowse,SIGNAL(clicked(bool)),this,SLOT(newImg()));
    connect(ui->cmdDel,SIGNAL(clicked(bool)),this,SLOT(delImg()));
    connect(modelDocType,SIGNAL(sigUpd()),Rels::instance()->relDocType,SLOT(refreshModel()));

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
    pix.loadFromData(Rels::instance()->relVedPix->data(QString::number(id)).toByteArray());
    if (!pix.isNull()){
        viewPix(pix);
    }
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
            Rels::instance()->relVedPix->refreshModel();
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
        Rels::instance()->relVedPix->refreshModel();
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}
