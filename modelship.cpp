#include "modelship.h"

ModelShip::ModelShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelShip::refresh(QDate begDate, QDate endDate)
{
    setQuery("select s.id, s.nom_s, s.dat_vid, p.short from sertifikat as s "
             "inner join poluch as p on p.id=s.id_pol "
             "inner join (select distinct id_sert from otpusk) as o on o.id_sert=s.id "
             "where s.dat_vid between '"+begDate.toString("yyyy-MM-dd")+"' and '"
             +endDate.toString("yyyy-MM-dd")+
             "' order by s.nom_s, s.dat_vid");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Номер"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Получатель"));
    }
}

QVariant ModelShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::DisplayRole) && index.column()==2){
        return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(index, role);
}


ModelDataShip::ModelDataShip(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelDataShip::refresh(int id_ship)
{
    setQuery("select o.id, p.n_s, p.dat_part, e.marka||' ф '||p.diam, o.massa, "
              "(select case when exists(select id_chem from sert_chem where id_part=p.id) "
                 "then 1 else 0 end "
                 "+ "
                 "case when exists(select id_mech from sert_mech where id_part=p.id) "
                 "then 2 else 0 end "
                 "as r) "
             "from otpusk o inner join parti p on o.id_part=p.id "
             "inner join elrtr e on e.id=p.id_el "
             "inner join istoch i on i.id=p.id_ist "
             "where o.id_sert ="+QString::number(id_ship)+" order by p.n_s, p.dat_part");
    if (lastError().isValid())
    {
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Масса, кг"));
    }
}

QVariant ModelDataShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundColorRole)&&(this->columnCount()>4)) {
    int area = record(index.row()).value(5).toInt();
    if(area == 0) return QVariant(QColor(255,170,170)); else
        if(area == 1) return QVariant(QColor(Qt::yellow)); else
            if(area == 2) return QVariant(QColor(Qt::gray)); else
                if(area == 3) return QVariant(QColor(170,255,170));
    }
    if (role == Qt::TextAlignmentRole){
        if((index.column() == 4))
            return int(Qt::AlignRight | Qt::AlignVCenter);
    }

    if (role == Qt::DisplayRole){
        if((index.column() == 2)){
            return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
        }
        if((index.column() == 4))
            return QLocale().toString(QSqlQueryModel::data(index,role).toDouble(),'f',1);
    }
    return QSqlQueryModel::data(index, role);
}


ModelPart::ModelPart(QObject *parent) :
    QSqlQueryModel(parent)
{
}

void ModelPart::refresh(QDate dbeg, QDate dend, int id_el)
{
    QString flt= (id_el==-1) ? "" : "and p.id_el= "+QString::number(id_el)+" ";
    setQuery("select p.id, p.n_s, p.dat_part, e.marka||' "+tr("ф")+" '||p.diam, i.nam as inam, r.nam, "
             "(select case when exists(select id_chem from sert_chem where id_part=p.id) "
             "then 1 else 0 end "
             "+ "
             "case when exists(select id_mech from sert_mech where id_part=p.id) "
             "then 2 else 0 end "
             "as r) "
             "from parti p "
             "inner join elrtr e on e.id=p.id_el "
             "inner join istoch i on i.id=p.id_ist "
             "left join rcp_nam r on r.id=p.id_rcp "
             "where p.dat_part between '"+dbeg.toString("yyyy-MM-dd")+"' and '"
             +dend.toString("yyyy-MM-dd")+"' "+flt+
             "order by p.n_s, p.dat_part");
    if (lastError().isValid())
    {
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Источник"));
        setHeaderData(5, Qt::Horizontal,tr("Рецептура"));
    }
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundColorRole)&&(this->columnCount()>6)) {
        int area = record(index.row()).value(6).toInt();
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    }
    if((role == Qt::DisplayRole) && index.column()==2){
        return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(index, role);
}
