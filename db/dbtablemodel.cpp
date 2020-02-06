#include "dbtablemodel.h"


DbTableModel::DbTableModel(QString table, QObject *parent) :
    QAbstractTableModel(parent)
{
    tableName=table;
    modelData = new MData(this);
    editor = new DataEditor(modelData,this);
    block=false;
    pkList=QSqlDatabase::database().driver()->primaryIndex(tableName);
    defaultRecord=QSqlDatabase::database().driver()->record(tableName);
}

Qt::ItemFlags DbTableModel::flags(const QModelIndex &index) const
{
    return modelData->column(index.column())->flags;
}

QVariant DbTableModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid()) return QVariant();
    QVariant value;
    QVariant origVal=modelData->value(index.row(),index.column());
    QVariant::Type type=columnType(index.column());
    switch (role) {
        case Qt::DisplayRole:
            if (modelData->column(index.column())->relation){
                value = modelData->value(index.row(),index.column()).isNull() ? QString() : modelData->column(index.column())->relation->data(modelData->value(index.row(),index.column()).toString());
            } else {
                if (type==QMetaType::QDate){
                    value=origVal.toDate().toString("dd.MM.yy");
                } else if (type==QMetaType::QDateTime){
                    value=origVal.toDateTime().toString("dd.MM.yy hh:mm");
                } else if (type==QMetaType::Double || type==QMetaType::Float){
                    int dec=3;
                    if (modelData->column(index.column())->validator){
                        QDoubleValidator *doublevalidator = qobject_cast<QDoubleValidator*>(modelData->column(index.column())->validator);
                        if (doublevalidator) dec=doublevalidator->decimals();
                    }
                    value=(origVal.isNull() || origVal.toString().isEmpty())? QString("") : QLocale().toString(origVal.toDouble(),'f',dec);
                } else if (type==QMetaType::Int) {
                    value=(origVal.isNull() || origVal.toString().isEmpty())? QString("") : QLocale().toString(origVal.toInt());
                } else if (type==QMetaType::Bool){
                    value=origVal.toBool()? QString(QString::fromUtf8("Да")) : QString(QString::fromUtf8("Нет"));
                } else {
                    value=origVal;
                }
            }
            break;

        case Qt::EditRole:
            value=origVal;
            break;

        case Qt::TextAlignmentRole:
            value=((type==QMetaType::Int || type==QMetaType::Double || type==QMetaType::Float
                    || type==QMetaType::Short || type==QMetaType::Long || type==QMetaType::LongLong) && !modelData->column(index.column())->relation)?
            int(Qt::AlignRight | Qt::AlignVCenter) : int(Qt::AlignLeft | Qt::AlignVCenter);
            break;

        case Qt::CheckStateRole:
            if (type==QMetaType::Bool){
                value=(origVal.toBool())? Qt::Checked :  Qt::Unchecked;
            } else value=QVariant();
            break;

        default:
            value=QVariant();
            break;
    }
    return value;
}

int DbTableModel::rowCount(const QModelIndex& /*parent*/) const
{
    return modelData->rowCount();
}

int DbTableModel::columnCount(const QModelIndex& /*parent*/) const
{
    return modelData->columnCount();
}

bool DbTableModel::setData(const QModelIndex &index, const QVariant &value, int /*role*/)
{
    if (!(this->flags(index) & Qt::ItemIsEditable)) return false;
    QVariant setVal;
    if (columnType(index.column())==QMetaType::Bool){
        setVal=value.toBool();
    } else if(!data(index,Qt::CheckStateRole).isNull() && columnType(index.column())==QMetaType::Int){
        setVal=value.toBool()? 1 : 0;
    } else {
        setVal=value;
    }

    bool ok=false;
    ok=editor->edt(index.row(),index.column(),setVal);
    emit dataChanged(index,index);
    emit headerDataChanged(Qt::Vertical,index.row(),index.row());
    return ok;
}

QVariant DbTableModel::headerData(int section, Qt::Orientation orientation, int role) const
{
    if (orientation == Qt::Vertical && role == Qt::DisplayRole) {
        if (editor->isEdt() && section==editor->currentPos()) return QString("|");
        return (editor->isAdd() && (section==editor->currentPos()))? QString("*"): QString("  ");
    }
    if (orientation == Qt::Horizontal && role == Qt::DisplayRole) {
        return modelData->column(section)->display;
    }
    return QAbstractTableModel::headerData(section, orientation, role);
}

bool DbTableModel::addColumn(QString name, QString display, QValidator *validator, DbRelation *relation)
{
    QVariant emptyval=defaultRecord.value(name);
    if (!validator){
        if (emptyval.type()==QMetaType::Int || emptyval.type()==QMetaType::LongLong){
            validator = new QIntValidator(this);
        } else if (emptyval.type()==QMetaType::Double){
            QDoubleValidator *v = new QDoubleValidator(this);
            v->setDecimals(1);
            validator = v;
        }
    }
    col tmpColumn;
    tmpColumn.name=name;
    tmpColumn.display=display;
    tmpColumn.validator=validator;
    tmpColumn.data.resize(rowCount());
    tmpColumn.relation=relation;
    tmpColumn.flags=Qt::ItemIsEditable | Qt::ItemIsSelectable |Qt::ItemIsUserCheckable | Qt::ItemIsEnabled;
    modelData->addColumn(tmpColumn);
    if (validator){
        validator->setLocale(QLocale::English);
    }
    QVariant defaultval;
    if (emptyval.type()==QMetaType::QDate){
        defaultval=QDate::currentDate();
    } else {
        defaultval=emptyval;
    }

    defaultTmpRow.resize(modelData->columnCount());
    setDefaultValue(modelData->columnCount()-1,defaultval);
    return true;
}

bool DbTableModel::removeRow(int row, const QModelIndex& parent)
{
    revert();
    if (!rowCount() || row<0 || row>=rowCount() || (editor->isAdd() && rowCount()==1)) return false;
    QString dat;
    for(int i=0; i<columnCount(); i++) dat+=data(this->index(row,i)).toString()+", ";
    dat.truncate(dat.count()-2);
    int n=QMessageBox::question(NULL,QString::fromUtf8("Подтвердите удаление"),
                                QString::fromUtf8("Подтветждаете удаление ")+dat+QString::fromUtf8("?"),QMessageBox::Yes| QMessageBox::No);
    bool ok=false;
    if (n==QMessageBox::Yes) {
        if (deleteDb(row)) {
            beginRemoveRows(parent,row,row);
            modelData->delRow(row);
            endRemoveRows();
            ok=true;
            emit sigUpd();
        }
    }
    if (ok && modelData->rowCount()<1) {
        this->insertRow(0);
    }
    return ok;
}

void DbTableModel::setFilter(QString s)
{
    filter=s;
}

void DbTableModel::setSort(QString s)
{
    sort=s;
}

void DbTableModel::setSuffix(QString s)
{
    suffix=s;
}

bool DbTableModel::isAdd()
{
    return editor->isAdd();
}

bool DbTableModel::isEdt()
{
    return (editor->isEdt());
}

bool DbTableModel::insertRow(int /*row*/, const QModelIndex& /*parent*/)
{
    if (block) return false;
    bool ok=false;
    if (!editor->isAdd() && !editor->isEdt()){
        beginInsertRows(QModelIndex(),rowCount(),rowCount());
        ok=editor->add(rowCount(),defaultTmpRow);
        endInsertRows();
    }
    return ok;
}

DbRelation *DbTableModel::relation(int column) const
{
    return modelData->column(column)->relation;
}

QVariant::Type DbTableModel::columnType(int column) const
{
    return nullVal(column).type();
}

QVariant DbTableModel::nullVal(int column) const
{
    return defaultRecord.value(modelData->column(column)->name);
}

int DbTableModel::currentEdtRow()
{
    return editor->currentPos();
}

QValidator *DbTableModel::validator(int column) const
{
    return modelData->column(column)->validator;
}

void DbTableModel::setDefaultValue(int column, QVariant value)
{
    defaultTmpRow[column]=value;
}

void DbTableModel::setColumnFlags(int column, Qt::ItemFlags flags)
{
    modelData->column(column)->flags=flags;
}

bool DbTableModel::insertDb()
{
    QSqlQuery query;
    QString qu;
    QString rows, vals, rets;
    QVector<QVariant> tmpRow=editor->newRow();

    for (int i=0; i<modelData->columnCount(); i++){
        if(!tmpRow[i].isNull()) {
            if (!rows.isEmpty()){
                rows+=", ";
            }
            rows+=modelData->column(i)->name;

            if (!vals.isEmpty()){
                vals+=", ";
            }
            vals+=(":"+modelData->column(i)->name);
        }
        if (!rets.isEmpty()){
            rets+=", ";
        }
        rets+=modelData->column(i)->name;
    }

    qu="INSERT INTO "+tableName+" ( "+rows+" ) VALUES ( "+vals+" ) RETURNING "+rets;

    query.prepare(qu);
    for (int i=0; i<modelData->columnCount(); i++){
        if (!tmpRow[i].isNull()){
            query.bindValue(":"+modelData->column(i)->name,tmpRow.at(i));
        }
    }
    //qDebug()<</*query.executedQuery();*/qu;
    bool ok=query.exec();
    if (!ok) {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    } else {
        while (query.next()){
            for (int i=0; i<tmpRow.size(); i++) tmpRow[i]=query.value(i);
        }
        modelData->setRow(tmpRow,editor->currentPos());
        int r = rowCount()-1;
        emit dataChanged(this->index(r,0),this->index(r,editor->currentPos()));
        emit headerDataChanged(Qt::Vertical,r,r);
        emit sigUpd();
    }
    return ok;
}

bool DbTableModel::updateDb()
{
    QSqlQuery query;
    QVector<QVariant> newRow=editor->newRow();
    QVector<QVariant> oldRow=editor->oldRow();
    int r = editor->currentPos();
    if (newRow==oldRow) {
        emit headerDataChanged(Qt::Vertical,r,r);
        return true;
    }
    QString qu;
    QString sets;
    QString pkeys;

    for (int i=0; i<modelData->columnCount(); i++){
        if(newRow[i]!=oldRow[i]){
            if (!sets.isEmpty()){
                sets+=", ";
            }
            sets+=modelData->column(i)->name+" = :new"+modelData->column(i)->name;
        }
        if (pkList.contains(modelData->column(i)->name)) {
            if (!pkeys.isEmpty()){
                pkeys+=" AND ";
            }
            pkeys+=(modelData->column(i)->name +" = :pk"+modelData->column(i)->name);
        }
    }

    qu="UPDATE "+tableName+" SET "+sets+" WHERE "+pkeys;
    query.prepare(qu);
    for (int i=0; i<modelData->columnCount(); i++){
        if(newRow[i]!=oldRow[i]){
            query.bindValue(":new"+modelData->column(i)->name,newRow[i]);
        }
        if (pkList.contains(modelData->column(i)->name)) {
            query.bindValue(":pk"+modelData->column(i)->name,oldRow[i]);
        }
    }

    //qDebug()<<query.executedQuery()<<" "<<qu;
    bool ok=query.exec();
    if (!ok){
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    } else {
        emit dataChanged(this->index(r,0),this->index(r,editor->currentPos()));
        emit headerDataChanged(Qt::Vertical,r,r);
        emit sigUpd();
    }
    return ok;
}

bool DbTableModel::deleteDb(int row)
{
    QSqlQuery query;
    QString qu;
    QString pkeys;

    for (int i=0; i<modelData->columnCount(); i++){
        if (pkList.contains(modelData->column(i)->name)) {
            if (!pkeys.isEmpty()){
                pkeys+=" AND ";
            }
            pkeys+=(modelData->column(i)->name +" = :pk"+modelData->column(i)->name);
        }
    }

    qu="DELETE FROM "+tableName+" WHERE "+pkeys;
    query.prepare(qu);

    for (int i=0; i<modelData->columnCount(); i++){
        if (pkList.contains(modelData->column(i)->name)) {
            query.bindValue(":pk"+modelData->column(i)->name,modelData->value(row,i));
        }
    }
    //qDebug()<<query.executedQuery()<<" "<<qu;
    bool ok=query.exec();
    if (!ok){
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
    }
    return ok;
}

bool DbTableModel::select()
{
    QSqlQuery query;
    query.setForwardOnly(true);
    QString qu;
    QString cols;

    for (int i=0; i<modelData->columnCount(); i++){
        if (!cols.isEmpty()){
            cols+=", ";
        }
        cols+=tableName+"."+modelData->column(i)->name;
    }
    qu="SELECT "+cols+" FROM "+tableName;
    if (!suffix.isEmpty()) qu+=" "+suffix;
    if (!filter.isEmpty()) qu+=" WHERE "+filter;
    if (!sort.isEmpty()) qu+=" ORDER BY "+sort;

    query.prepare(qu);
    //qDebug()<<query.executedQuery()<<" "<<qu;
    beginResetModel();
    if (query.exec()){
        editor->esc();
        modelData->clear();
        while (query.next()){
            for (int i=0; i<modelData->columnCount(); i++){
                modelData->column(i)->data.push_back(query.value(i));
            }
        }
    } else {
        QMessageBox::critical(NULL,tr("Error"),query.lastError().text(),QMessageBox::Cancel);
        return false;
    }
    int r = modelData->rowCount();
    if (!r) editor->add(0,defaultTmpRow);
    endResetModel();
    emit sigRefresh();
    return true;
}

void DbTableModel::revert()
{
    block=true;
    int r=editor->currentPos();
    if (editor->isAdd() && rowCount()>1){
        beginRemoveRows(QModelIndex(),r,r);
        editor->esc();
        endRemoveRows();
    } else if (editor->isEdt()){
        editor->esc();
        emit dataChanged(this->index(r,0),this->index(r,columnCount()-1));
        emit headerDataChanged(Qt::Vertical,r,r);
    }
    block=false;
}

bool DbTableModel::submit()
{
    if (block) return false;
    if (editor->isEdt()){
        if (editor->isAdd()){
            if (insertDb()) {
                editor->submit();
                //qDebug()<<"SUBMIT_ADD";
            }
        } else if (!editor->isAdd()){
            if (updateDb()){
                editor->submit();
                //qDebug()<<"SUBMIT_EDT";
            }
        }
    } else if (editor->isAdd()){
        revert();
    }
    return !(editor->isAdd() || editor->isEdt());
}

MData::MData(QObject *parent):QObject(parent)
{
}

void MData::addColumn(col &column)
{
    data.push_back(column);
}

void MData::setRow(QVector<QVariant> &row, int pos)
{
    for (int i=0; i<data.size(); i++)
        data[i].data[pos]=row.at(i);
}

void MData::insertRow(int pos, QVector<QVariant> &row)
{
    for (int i=0; i<data.size(); i++)
        data[i].data.insert(pos,row.at(i));
}

void MData::delRow(int pos)
{
    for (int i=0; i<data.size(); i++)
        data[i].data.remove(pos,1);
}

int MData::rowCount()
{
    return data.size()? data[0].data.size() : 0;
}

int MData::columnCount()
{
    return data.size();
}

QVariant MData::value(int row, int column)
{
    return data[column].data[row];
}

col *MData::column(int c)
{
    return &data[c];
}

QVector<QVariant> MData::row(int r) const
{
    QVector<QVariant> tmp;
    for (int i=0; i<data.size(); i++)
        tmp.push_back(data[i].data[r]);
    return tmp;
}

void MData::setValue(const QVariant &val, int row, int column)
{
    data[column].data[row]=val;
}

void MData::clear()
{
    for (int i=0; i<data.size(); i++)
        data[i].data.clear();
}


DataEditor::DataEditor(MData *dat, QObject *parent)
    :QObject(parent)
{
    mData=dat;
    pos=-1;
    addFlag=false;
    edtFlag=false;
}

bool DataEditor::add(int p, QVector<QVariant> &row)
{
    bool ok=false;
    if (!addFlag && !edtFlag){
        mData->insertRow(p,row);
        pos=p;
        addFlag=true;
        ok=true;
    }
    return ok;
}

bool DataEditor::edt(int row, int col, QVariant val)
{
    if((edtFlag && row!=pos) || (addFlag && row!=pos)) return false;
    if (!edtFlag){
        pos=row;
        saveRow=mData->row(row);
    }
    mData->setValue(val,row,col);
    edtFlag=true;
    return true;
}

void DataEditor::submit()
{
    addFlag=false;
    edtFlag=false;
    pos=-1;
}

void DataEditor::esc()
{
    if ((edtFlag && !addFlag) || (addFlag && mData->rowCount()==1 && edtFlag)){
        mData->setRow(saveRow,pos);
        edtFlag=false;
    } else if (addFlag && mData->rowCount()>1){
        mData->delRow(pos);
        addFlag=false;
        edtFlag=false;
    } else {
        edtFlag=false;
        addFlag=false;
        pos=-1;
    }
}

bool DataEditor::isAdd()
{
    return addFlag;
}

bool DataEditor::isEdt()
{
    return edtFlag;
}

int DataEditor::currentPos()
{
    return pos;
}

QVector<QVariant> DataEditor::oldRow()
{
    return saveRow;
}

QVector<QVariant> DataEditor::newRow()
{
    QVector<QVariant> r;
    if (addFlag || edtFlag){
        for (int i=0; i<mData->columnCount(); i++){
            r.push_back(mData->column(i)->data.at(pos));
        }
    }
    return r;
}

DbRelation::DbRelation(QAbstractItemModel *queryModel, int key, int disp, QObject *parent) :
    relQueryModel(queryModel), keyCol(key), dispCol(disp), QObject(parent)
{
    reHash();
    filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(relQueryModel);
    filterModel->setFilterKeyColumn(disp);
    DbRelationalModel *sqlModel = qobject_cast< DbRelationalModel *>(relQueryModel);
    if (sqlModel){
        connect(queryModel,SIGNAL(sigRefresh()),this,SLOT(reHash()));
    } else {
        DbTableModel *dbModel = qobject_cast< DbTableModel *>(relQueryModel);
        if (dbModel){
            connect(dbModel,SIGNAL(sigUpd()),this,SLOT(reHash()));
            connect(dbModel,SIGNAL(sigRefresh()),this,SLOT(reHash()));
        }
    }
}

DbRelation::DbRelation(const QString &query, int key, int disp, QObject *parent) :
    keyCol(key), dispCol(disp), QObject(parent)
{
    relQueryModel= new DbRelationalModel(query,this);
    reHash();
    filterModel = new QSortFilterProxyModel(this);
    filterModel->setSourceModel(relQueryModel);
    filterModel->setFilterKeyColumn(disp);
    DbRelationalModel *sqlModel = qobject_cast< DbRelationalModel *>(relQueryModel);
    connect(sqlModel,SIGNAL(sigRefresh()),this,SLOT(reHash()));
}

QVariant DbRelation::data(QString key)
{
    return relQueryModel->data(dict.value(key,QModelIndex()),Qt::EditRole);
}

QModelIndex DbRelation::modelIndex(QString key)
{
    return dict.value(key,QModelIndex());
}

QAbstractItemModel *DbRelation::model() const
{
    return relQueryModel;
}

QSortFilterProxyModel *DbRelation::proxyModel() const
{
    return filterModel;
}

int DbRelation::columnKey()
{
    return keyCol;
}

int DbRelation::columnDisplay()
{
    return dispCol;
}

void DbRelation::reHash()
{
    dict.clear();
    for (int i=0; i<relQueryModel->rowCount(); i++){
        QString key=relQueryModel->data(relQueryModel->index(i,keyCol),Qt::EditRole).toString();
        QModelIndex val=relQueryModel->index(i,dispCol);
        dict[key]=val;
    }
}

void DbRelation::refreshModel()
{
    DbRelationalModel *sqlModel = qobject_cast< DbRelationalModel *>(relQueryModel);
    if (sqlModel){
        sqlModel->refresh();
    } else {
        DbTableModel *dbModel = qobject_cast< DbTableModel *>(relQueryModel);
        if (dbModel){
            dbModel->select();
        }
    }
}

DbRelationalModel::DbRelationalModel(QObject *parent) : QSqlQueryModel(parent)
{

}

DbRelationalModel::DbRelationalModel(QString query, QObject *parent) : QSqlQueryModel(parent)
{
    this->setQuery(query);
}

bool DbRelationalModel::setQuery(const QString &query, const QSqlDatabase &db)
{
    qu=query;
    QSqlQueryModel::setQuery(query,db);
    bool ok=!this->lastError().isValid();
    if (!ok){
        QMessageBox::critical(NULL,tr("Error"),this->lastError().text(),QMessageBox::Cancel);
    }
    emit sigRefresh();
    return ok;
}

void DbRelationalModel::refresh()
{
    this->setQuery(qu);
}
