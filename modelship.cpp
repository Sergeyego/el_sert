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
    current_id_ship=-1;
}

void ModelDataShip::refresh(int id_ship)
{
    current_id_ship=id_ship;
    setQuery("select o.id, p.n_s||'-'||date_part('year',p.dat_part), e.marka||' "+tr("ф")+" '||cast(p.diam as varchar(3))||"
             "CASE WHEN p.id_var <> 1 THEN (' /'::text || ev.nam::text) || '/'::text ELSE ''::text END AS mark, "
             "o.massa, o.ds_status, "
              "(select case when exists(select id_chem from sert_chem where id_part=p.id) "
                 "then 1 else 0 end "
                 "+ "
                 "case when exists(select id_mech from sert_mech where id_part=p.id) "
                 "then 2 else 0 end "
                 "as r), p.id "
             "from otpusk o inner join parti p on o.id_part=p.id "
             "inner join elrtr e on e.id=p.id_el "
             "inner join istoch i on i.id=p.id_ist "
             "inner join elrtr_vars ev on ev.id=p.id_var "
             "where o.id_sert ="+QString::number(id_ship)+" order by p.n_s, p.dat_part");
    if (lastError().isValid())
    {
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Марка"));
        setHeaderData(3, Qt::Horizontal,tr("Масса, кг"));
        setHeaderData(4, Qt::Horizontal,tr("ЭЦП"));
        setHeaderData(5, Qt::Horizontal,tr("Облако"));
    }
}

void ModelDataShip::refresh()
{
    refresh(current_id_ship);
}

QVariant ModelDataShip::data(const QModelIndex &index, int role) const
{
    if((role == Qt::BackgroundRole)&&(this->columnCount()>3)) {
    int area = record(index.row()).value(5).toInt();
    if(area == 0) return QVariant(QColor(255,170,170)); else
        if(area == 1) return QVariant(QColor(Qt::yellow)); else
            if(area == 2) return QVariant(QColor(Qt::gray)); else
                if(area == 3) return QVariant(QColor(170,255,170));
    }
    if (role == Qt::TextAlignmentRole){
        if((index.column() == 3))
            return int(Qt::AlignRight | Qt::AlignVCenter);
    }
    if (role == Qt::DisplayRole){
        if((index.column() == 3))
            return QLocale().toString(QSqlQueryModel::data(index,role).toDouble(),'f',1);
    }
    return QSqlQueryModel::data(index, role);
}

ModelPart::ModelPart(QObject *parent) :
    QSqlQueryModel(parent)
{
    stateExecutor = new Executor(this);
    connect(stateExecutor,SIGNAL(finished()),this,SLOT(refreshStateFinished()));
}

void ModelPart::refresh(QDate dbeg, QDate dend, int id_el)
{
    dateBeg = dbeg;
    dateEnd = dend;
    QString flt= (id_el==-1) ? "" : "and p.id_el= "+QString::number(id_el)+" ";
    setQuery("select p.id, p.n_s, p.dat_part, e.marka||' "+tr("ф")+" '||cast(p.diam as varchar(3)), i.nam as inam, r.nam, ev.nam, p.id_el "
             "from parti p "
             "inner join elrtr e on e.id=p.id_el "
             "inner join istoch i on i.id=p.id_ist "
             "left join rcp_nam r on r.id=p.id_rcp "
             "inner join elrtr_vars ev on ev.id = p.id_var "
             "where p.dat_part between '"+dbeg.toString("yyyy-MM-dd")+"' and '"
             +dend.toString("yyyy-MM-dd")+"' "+flt+
             "order by p.n_s, p.dat_part");
    if (lastError().isValid()){
        QMessageBox::critical(NULL,"Error",lastError().text(),QMessageBox::Cancel);
    } else {
        setHeaderData(1, Qt::Horizontal,tr("Партия"));
        setHeaderData(2, Qt::Horizontal,tr("Дата"));
        setHeaderData(3, Qt::Horizontal,tr("Марка"));
        setHeaderData(4, Qt::Horizontal,tr("Источник"));
        setHeaderData(5, Qt::Horizontal,tr("Рецептура"));
        setHeaderData(6, Qt::Horizontal,tr("Вариант"));
    }
    refreshState();
}

QVariant ModelPart::data(const QModelIndex &index, int role) const
{
    if (role == Qt::BackgroundRole) {
        int id_part=this->data(this->index(index.row(),0),Qt::EditRole).toInt();
        int area = mapStat.value(id_part);
        if (area==7 || area==6){
            return QVariant(QColor(170,255,170));
        } else if (area==2 || area==3){
            return QVariant(QColor(Qt::gray));
        } else if (area==4 || area==5){
            return QVariant(QColor(Qt::yellow));
        } else if (area==1){
            return QVariant(QColor(255,200,100));
        } else {
            return QVariant(QColor(255,170,170));
        }
    }
    if((role == Qt::DisplayRole) && index.column()==2){
        return QSqlQueryModel::data(index,role).toDate().toString("dd.MM.yy");
    }
    return QSqlQueryModel::data(index, role);
}

void ModelPart::refreshStateFinished()
{
    QVector<QVector<QVariant>> data=stateExecutor->getData();
    mapStat.clear();
    for (QVector<QVariant> row : data){
        mapStat.insert(row.at(0).toInt(),row.at(1).toInt());
    }
    if (this->rowCount()){
        emit dataChanged(this->index(0,0),this->index(this->rowCount()-1,this->columnCount()-1));
    }
}

void ModelPart::refreshState()
{
    stateExecutor->setQuery(QString("select p.id, ( "
                                    "(case when p.ok then 1 else 0 end) + "
                                    "(select case when exists(select id_chem from sert_chem where id_part=p.id) then 2 else 0 end ) + "
                                    "(case when exists(select id_mech from sert_mech where id_part=p.id) then 4 else 0 end ) "
                                    ") as r "
                                    "from parti p where p.dat_part between '%1' and '%2'").arg(dateBeg.toString("yyyy-MM-dd")).arg(dateEnd.toString("yyyy-MM-dd")));
    stateExecutor->start();
}
