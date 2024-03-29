#include "formpos.h"
#include "ui_formpos.h"

FormPos::FormPos(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::FormPos)
{
    ui->setupUi(this);

    modelPos = new DbTableModel("pics",this);
    modelPos->addColumn("id","id");
    modelPos->addColumn("descr",QString::fromUtf8("Описание"));
    modelPos->addColumn("dt",QString::fromUtf8("Код"));
    modelPos->setSort("pics.descr");
    modelPos->select();
    ui->tableViewPos->setModel(modelPos);
    ui->tableViewPos->setColumnHidden(0,true);
    ui->tableViewPos->setColumnWidth(1,400);
    ui->tableViewPos->setColumnWidth(2,80);

    connect(ui->tableViewPos->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(loadImg(QModelIndex)));
    connect(ui->cmdLoad,SIGNAL(clicked(bool)),this,SLOT(newImg()));
    connect(ui->cmdDel,SIGNAL(clicked(bool)),this,SLOT(delImg()));
    connect(modelPos,SIGNAL(sigUpd()),Rels::instance(),SLOT(refreshPolPix()));
    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));

    if (ui->tableViewPos->model()->rowCount()){
        ui->tableViewPos->selectRow(0);
    }
}

FormPos::~FormPos()
{
    delete ui;
}

void FormPos::viewPix(const QPixmap &pix)
{
    if (pix.isNull()){
        ui->labelPos->setPixmap(pix);
    } else {
        ui->labelPos->setPixmap(pix.scaled(ui->labelPos->size(),Qt::KeepAspectRatio));
    }
}

void FormPos::loadImg(QModelIndex index)
{
    ui->labelPos->setPixmap(QPixmap());
    int id=ui->tableViewPos->model()->data(ui->tableViewPos->model()->index(index.row(),0),Qt::EditRole).toInt();
    QPixmap pix;
    pix.loadFromData(Rels::instance()->mapPolPix.value(id));
    if (!pix.isNull()){
        viewPix(pix);
    }
    ui->cmdDel->setEnabled(!pix.isNull());
}

void FormPos::newImg()
{
    QDir dir(QDir::homePath());
    QString filename = QFileDialog::getOpenFileName(nullptr,QString::fromUtf8("Выберите изображение"),
                                                    dir.path(),
                                                    QString::fromUtf8("Images (*.png *.xpm *.jpg *.jpeg *.bmp)"));
    QImage img(filename);
    if (!img.isNull()){
        int row=ui->tableViewPos->currentIndex().row();
        int id=ui->tableViewPos->model()->data(ui->tableViewPos->model()->index(row,0),Qt::EditRole).toInt();
        QByteArray arr;
        QBuffer buffer(&arr);
        buffer.open(QIODevice::WriteOnly);
        img.save(&buffer,"PNG");
        buffer.close();

        QSqlQuery query;
        query.prepare("update pics set data = :data where id= :id");
        query.bindValue(":id",id);
        query.bindValue(":data",arr);
        if (query.exec()){
            viewPix(QPixmap::fromImage(img));
            ui->cmdDel->setEnabled(!img.isNull());
            Rels::instance()->refreshPolPix();
        } else {
            QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
        }
    }
}

void FormPos::delImg()
{
    int row=ui->tableViewPos->currentIndex().row();
    int id=ui->tableViewPos->model()->data(ui->tableViewPos->model()->index(row,0),Qt::EditRole).toInt();
    QSqlQuery query;
    query.prepare("update pics set data = NULL where id= :id");
    query.bindValue(":id",id);
    if (query.exec()){
        viewPix(QPixmap());
        Rels::instance()->refreshPolPix();
    } else {
        QMessageBox::critical(this,"Error",query.lastError().text(),QMessageBox::Cancel);
    }
}

void FormPos::upd()
{
    Rels::instance()->refreshPolPix();
    modelPos->select();
    if (ui->tableViewPos->model()->rowCount()){
        ui->tableViewPos->selectRow(0);
    }
}
