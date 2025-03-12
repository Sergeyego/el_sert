#include "checkform.h"
#include "ui_checkform.h"

CheckForm::CheckForm(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::CheckForm)
{
    ui->setupUi(this);
    connect(ui->cmdGo,SIGNAL(clicked()),this,SLOT(go()));
}

CheckForm::~CheckForm()
{
    delete ui;
}

void CheckForm::closeEvent(QCloseEvent *e)
{
    QWidget::closeEvent(e);
    emit sigClose();
}

void CheckForm::setCheckEl(bool b)
{
    if (b){
        ui->radioButtonEl->setChecked(b);
    } else {
        ui->radioButtonWire->setChecked(true);
    }
}

QString CheckForm::checkEl(int id_part, int id_ship)
{
    QString str;
    if (id_part!=0){
        QSqlQuery query;
        if (id_ship!=0){
            query.prepare("select o.id_part, o.massa, s.nom_s, s.dat_vid, p.n_s, p.yea, p.dat_part, "
                          "el.marka, p.diam, pol.naim "
                          "from otpusk as o "
                          "inner join sertifikat as s on o.id_sert=s.id "
                          "inner join parti as p on o.id_part=p.id "
                          "inner join elrtr as el on p.id_el=el.id "
                          "inner join poluch as pol on s.id_pol=pol.id "
                          "where o.id = :id_sert ");
            query.bindValue(":id_sert",id_ship);
            if (query.exec()){
                while(query.next()){
                    str+=tr("ОТГРУЗКА № ")+query.value(2).toString()+tr(" от ")+query.value(3).toDate().toString("dd.MM.yy")+tr(" грузополучатель: ")+query.value(9).toString();
                    str+="\n";
                    str+=tr("Марка ")+query.value(7).toString()+"\n";
                    str+=tr("Диаметр ")+query.value(8).toString()+"\n";
                    str+=tr("Партия ")+query.value(4).toString()+"\n";
                    str+=tr("Дата производства ")+query.value(6).toDate().toString("dd.MM.yy")+"\n";
                    if (query.value(0).toInt()!=id_part) str+=tr("ПАРТИЯ НЕ ПРИНАДЛЕЖИТ ДАННОЙ ОТГРУЗКЕ! КОД НЕВЕРНЫЙ!")+"\n";
                }
            } else {
                QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        } else {
            str+=tr("СЕРТИФИКАТ НА ПАРТИЮ");
            str+="\n";
            query.prepare("select p.n_s, p.dat_part, el.marka, p.diam "
                          "from parti as p "
                          "inner join elrtr as el on p.id_el=el.id "
                          "where p.id= :id_part");
            query.bindValue(":id_part",id_part);
            if (query.exec()){
                while (query.next()) {
                    str+=tr("Марка ")+query.value(2).toString()+"\n";
                    str+=tr("Диаметр ")+query.value(3).toString()+"\n";
                    str+=tr("Партия ")+query.value(0).toString()+"\n";
                    str+=tr("Дата производства ")+query.value(1).toDate().toString("dd.MM.yy")+"\n";
                }
            } else {
                QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        }
    }
    if (str.isEmpty())
        str+=tr("КОД НЕВЕРНЫЙ!");
    return str;
}

QString CheckForm::checkWire(int id_part, int id_ship)
{
    QString str;
    if (id_part!=0){
        QSqlQuery query;
        if (id_ship!=0){
            query.prepare("select w.id_wparti, w.m_netto, s.nom_s, s.dat_vid, m.n_s, date_part('year',m.dat), m.dat, "
                          "b.n_plav, pprov.nam, d.diam, k.nam, pol.naim "
                          "from wire_shipment_consist as w "
                          "inner join sertifikat as s on w.id_ship=s.id "
                          "inner join wire_parti as p on w.id_wparti=p.id "
                          "inner join wire_parti_m as m on p.id_m=m.id "
                          "inner join prov_buht as b on m.id_buht=b.id "
                          "inner join prov_prih as pr on b.id_prih=pr.id "
                          "inner join provol as pprov on m.id_provol=pprov.id "
                          "inner join diam as d on m.id_diam=d.id "
                          "inner join poluch as pol on s.id_pol=pol.id "
                          "inner join wire_pack_kind as k on p.id_pack=k.id "
                          "where w.id = :id_sert ");
            query.bindValue(":id_sert",id_ship);
            if (query.exec()){
                while(query.next()){
                    str+=tr("ОТГРУЗКА № ")+query.value(2).toString()+tr(" от ")+query.value(3).toDate().toString("dd.MM.yy")+tr(" грузополучатель: ")+query.value(11).toString();
                    str+="\n";
                    str+=tr("Марка ")+query.value(8).toString()+"\n";
                    str+=tr("Диаметр ")+query.value(9).toString()+"\n";
                    str+=tr("Намотка ")+query.value(10).toString()+"\n";
                    str+=tr("Плавка ")+query.value(7).toString()+"\n";
                    str+=tr("Партия ")+query.value(4).toString()+"\n";
                    str+=tr("Дата производства ")+query.value(6).toDate().toString("dd.MM.yy")+"\n";
                    if (query.value(0).toInt()!=id_part) str+=tr("ПАРТИЯ НЕ ПРИНАДЛЕЖИТ ДАННОЙ ОТГРУЗКЕ! КОД НЕВЕРНЫЙ!")+"\n";
                }
            } else {
                QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        } else {
            str+=tr("СЕРТИФИКАТ НА ПАРТИЮ");
            str+="\n";
            query.prepare("select m.n_s, date_part('year',m.dat), m.dat, "
                          "b.n_plav, prov.nam, d.diam, k.nam "
                          "from wire_parti as p "
                          "inner join wire_parti_m as m on p.id_m=m.id "
                          "inner join prov_buht as b on m.id_buht=b.id "
                          "inner join prov_prih as pr on b.id_prih=pr.id "
                          "inner join provol as prov on m.id_provol=prov.id "
                          "inner join diam as d on m.id_diam=d.id "
                          "inner join wire_pack_kind as k on p.id_pack=k.id "
                          "where p.id= :id_part");
            query.bindValue(":id_part",id_part);
            if (query.exec()){
                while (query.next()) {
                    str+=tr("Марка ")+query.value(4).toString()+"\n";
                    str+=tr("Диаметр ")+query.value(5).toString()+"\n";
                    str+=tr("Намотка ")+query.value(6).toString()+"\n";
                    str+=tr("Плавка ")+query.value(3).toString()+"\n";
                    str+=tr("Партия ")+query.value(0).toString()+"\n";
                    str+=tr("Дата производства ")+query.value(2).toDate().toString("dd.MM.yy")+"\n";
                }
            } else {
                QMessageBox::critical(this,tr("Ошибка"),query.lastError().text(),QMessageBox::Ok);
            }
        }
    }
    if (str.isEmpty())
        str+=tr("КОД НЕВЕРНЫЙ!");
    return str;
}

void CheckForm::go()
{
    ui->textEdit->clear();
    quint64 cod=ui->lineEditCod->text().toULongLong();
    quint32 id_p=0;
    quint32 id_s=0;
    memcpy((quint8*)&id_s,(quint8*)&cod,4);
    memcpy((quint8*)&id_p,(quint8*)&cod+4,4);
    //qDebug()<<id_s<<" "<<id_p;
    QString str;
    if (ui->radioButtonEl->isChecked()){
        str=checkEl(id_p,id_s);
    } else {
        str=checkWire(id_p,id_s);
    }
    ui->textEdit->setText(str);
}
