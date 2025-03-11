#include "formpartwire.h"
#include "ui_formpartwire.h"

FormPartWire::FormPartWire(QWidget *parent)
    : QWidget(parent)
    , ui(new Ui::FormPartWire)
{
    ui->setupUi(this);
    loadSettings();

    ui->dateEditBeg->setDate(QDate::currentDate().addDays(-QDate::currentDate().dayOfYear()+1));
    ui->dateEditEnd->setDate(ui->dateEditBeg->date().addYears(1));

    if (!Rels::instance()->relProvol->isInital()){
        Rels::instance()->relProvol->refreshModel();
    }
    ui->comboBoxMarka->setModel(Rels::instance()->relProvol->model());

    mapper = new QDataWidgetMapper(this);

    modelPartWire = new ModelPartWire(this);
    ui->tableViewPart->setModel(modelPartWire);

    mapper->setModel(modelPartWire);
    mapper->addMapping(ui->lineEditSrcPart,8);
    mapper->addMapping(ui->lineEditPodt,9);
    mapper->addMapping(ui->lineEditVol,10);
    mapper->addMapping(ui->lineEditLine,11);
    mapper->addMapping(ui->lineEditKvo,12);

    connect(ui->pushButtonUpd,SIGNAL(clicked(bool)),this,SLOT(upd()));
    connect(ui->tableViewPart->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(currentIndexChanged(QModelIndex)));
    connect(ui->checkBoxOnly,SIGNAL(clicked(bool)),ui->comboBoxMarka,SLOT(setEnabled(bool)));
    upd();
}

FormPartWire::~FormPartWire()
{
    saveSettings();
    delete ui;
}

void FormPartWire::loadSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    this->ui->splitter->restoreState(settings.value("wire_parti_splitter_width").toByteArray());
}

void FormPartWire::saveSettings()
{
    QSettings settings("szsm", QApplication::applicationName());
    settings.setValue("wire_parti_splitter_width",ui->splitter->saveState());
}

void FormPartWire::upd()
{
    modelPartWire->refresh(ui->dateEditBeg->date(),ui->dateEditEnd->date(),-1);
    ui->tableViewPart->setColumnHidden(0,true);
    for (int i=8; i<ui->tableViewPart->model()->rowCount(); i++){
        ui->tableViewPart->setColumnHidden(i,true);
    }
    ui->tableViewPart->resizeToContents();
    int n=ui->tableViewPart->model()->rowCount();
    if (n){
        ui->tableViewPart->setCurrentIndex(ui->tableViewPart->model()->index(n-1,1));
    } else {
        ui->lineEditSrcPart->clear();
        ui->lineEditPodt->clear();
        ui->lineEditVol->clear();
        ui->lineEditLine->clear();
        ui->lineEditKvo->clear();
    }
}

void FormPartWire::currentIndexChanged(QModelIndex index)
{
    mapper->setCurrentIndex(index.row());
}

ModelPartWire::ModelPartWire(QWidget *parent) : ModelRo(parent)
{
    setDecimal(1);
}

void ModelPartWire::refresh(QDate beg, QDate end, int id_provol)
{
    dateBeg=beg;
    dateEnd=end;
    refreshState();
    QString flt = (id_provol>0) ? ("and wpm.id_provol = "+QString::number(id_provol)+" "):"";
    QSqlQuery query;
    query.prepare("select wp.id, wpm.n_s, wpm.dat, ws.nam, p.nam, d.diam, "
                  "wpk.short ||' ('||wp2.mas_ed ||' кг)', ev.nam, "
                  "pb.str, wp3.str, we.snam, wl.snam, wpm.kvo, wp.id_m "
                  "from wire_parti wp "
                  "inner join wire_parti_m wpm on wpm.id = wp.id_m "
                  "inner join wire_source ws on ws.id = wpm.id_source "
                  "inner join provol p on p.id = wpm.id_provol "
                  "inner join diam d on d.id = wpm.id_diam "
                  "inner join wire_pack_kind wpk on wpk.id = wp.id_pack "
                  "inner join wire_pack wp2 on wp2.id = wp.id_pack_type "
                  "inner join elrtr_vars ev on ev.id = wp.id_var "
                  "inner join prov_buht pb on pb.id = wpm.id_buht "
                  "left join wire_podt wp3 on wp3.id = wpm.id_podt "
                  "left join wire_empl we on we.id = wpm.id_empl "
                  "inner join wire_line wl on wl.id = wpm.id_type "
                  "where wpm.dat between :d1 and :d2 "+flt+
                  "order by date_part('year',wpm.dat), wpm.n_s, wpm.dat, wpk.short");
    query.bindValue(":d1",beg);
    query.bindValue(":d2",end);
    if (execQuery(query)){
        setHeaderData(1,Qt::Horizontal,tr("Номер"));
        setHeaderData(2,Qt::Horizontal,tr("Дата"));
        setHeaderData(3,Qt::Horizontal,tr("Источник"));
        setHeaderData(4,Qt::Horizontal,tr("Марка"));
        setHeaderData(5,Qt::Horizontal,tr("Диам."));
        setHeaderData(6,Qt::Horizontal,tr("Носитель"));
        setHeaderData(7,Qt::Horizontal,tr("Вариант"));
    }
}

QVariant ModelPartWire::data(const QModelIndex &item, int role) const
{
    if((role == Qt::BackgroundRole)) {
        int area = colorState.value(data(this->index(item.row(),13),Qt::EditRole).toInt());
        if(area == 0) return QVariant(QColor(255,170,170)); else
            if(area == 1) return QVariant(QColor(Qt::yellow)); else
                if(area == 2) return QVariant(QColor(Qt::gray)); else
                    if(area == 3) return QVariant(QColor(170,255,170));
    } else return ModelRo::data(item,role);
}

void ModelPartWire::refreshState()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    query.prepare("select wire_parti_m.id, "
                  "(select case when exists(select id from wire_parti_chem where id_part=wire_parti_m.id) "
                  "then 1 else 0 end "
                  "+ "
                  "case when exists(select id from wire_parti_mech where id_part=wire_parti_m.id) "
                  "then 2 else 0 end "
                  "as r) from wire_parti_m where wire_parti_m.dat between :d1 and :d2");
    query.bindValue(":d1",dateBeg);
    query.bindValue(":d2",dateEnd);
    if (query.exec()){
        colorState.clear();
        while (query.next()){
            colorState[query.value(0).toInt()]=query.value(1).toInt();
        }
        if (this->rowCount()){
            emit dataChanged(this->index(0,0),this->index(this->rowCount()-1,this->columnCount()-1));
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Ok);
    }
}
