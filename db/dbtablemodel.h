#ifndef DBTABLEMODEL_H
#define DBTABLEMODEL_H

#include <QAbstractTableModel>
#include <QVector>
#include <QAbstractItemModel>
#include <QSqlQuery>
#include <QSqlQueryModel>
#include <QMessageBox>
#include <QSqlError>
#include <QDebug>
#include <QDate>
#include <QValidator>
#include <QSortFilterProxyModel>
#include <QLocale>

enum {
    TYPE_VARIANT = 0,
    TYPE_STRING,
    TYPE_INT,
    TYPE_DOUBLE,
    TYPE_DATE,
    TYPE_BOOL,
    TYPE_INTBOOL
};

class DbRelationalModel : public QSqlQueryModel{
    Q_OBJECT
public:
    DbRelationalModel(QObject *parent=0);
    DbRelationalModel(QString query, QObject *parent=0);
    bool setQuery(const QString &query, const QSqlDatabase &db=QSqlDatabase());
signals:
    void sigRefresh();
public slots:
    void refresh();
private:
    QString qu;
};

class DbRelation : public QObject
{
    Q_OBJECT
public:
    DbRelation(QAbstractItemModel *queryModel, int key, int disp, QObject *parent=0);
    DbRelation(const QString &query, int key, int disp, QObject *parent=0);
    QVariant data(QString key);
    QAbstractItemModel *model() const;
    QSortFilterProxyModel *proxyModel() const;
    int columnKey();
    int columnDisplay();
private:
    QAbstractItemModel *relQueryModel;
    QSortFilterProxyModel *filterModel;
    QHash <QString, QModelIndex> dict;
    int keyCol;
    int dispCol;
private slots:
    void reHash();
public slots:
    void refreshModel();
};

typedef struct
{
    QString name;
    QString display;
    bool isPk;
    int type;
    DbRelation *relation;
    QVector<QVariant> data;
    QValidator *validator;
} col;


class MData : public QObject
{
    Q_OBJECT
public:
    MData(QObject *parent=0);
    int rowCount();
    int columnCount();
    QVariant value(int row, int column);
    col *column(int c);
    void addColumn(col &column);
    void clear();
    void delRow(int pos);
    void setRow(QVector<QVariant> &row, int pos);
protected:
    void insertRow(int pos, QVector<QVariant> &row);
    QVector<QVariant> row(int r) const;
    void setValue(const QVariant &val, int row, int column);
    friend class DataEditor;
private:
    QVector<col> data;
};

class DataEditor : public QObject
{
    Q_OBJECT
public:
    DataEditor(MData *dat, QObject *parent=0);
    bool add(int p, QVector<QVariant> &row);
    bool edt(int row, int col, QVariant val);
    void submit();
    void esc();
    bool isAdd();
    bool isEdt();
    int currentPos();
    QVector<QVariant> oldRow();
    QVector<QVariant> newRow();
private:
    MData *mData;
    QVector<QVariant> saveRow;
    bool addFlag;
    bool edtFlag;
    int pos;
};

class DbTableModel : public QAbstractTableModel
{
    Q_OBJECT
public:
    explicit DbTableModel(QString table, QObject *parent = 0);
    Qt::ItemFlags flags(const QModelIndex &index) const;
    QVariant data(const QModelIndex &index, int role=Qt::DisplayRole) const;
    int rowCount(const QModelIndex &parent=QModelIndex()) const;
    int columnCount(const QModelIndex &parent=QModelIndex()) const;
    bool setData(const QModelIndex &index, const QVariant &value, int role);
    QVariant headerData(int section, Qt::Orientation orientation, int role = Qt::DisplayRole) const;
    bool addColumn(QString name, QString display, bool isPk, int type, QValidator *validator=NULL, DbRelation *relation=NULL);
    bool removeRow(int row, const QModelIndex &parent = QModelIndex());
    void setFilter(QString s);
    void setSort(QString s);
    void setSuffix(QString s);
    bool isAdd();
    bool isEdt();
    void escAdd();
    virtual bool submitRow();
    virtual bool insertRow(int row, const QModelIndex &parent=QModelIndex());
    DbRelation *relation(int column) const;
    int columnType(int column) const;
    int currentEdtRow();
    QValidator* validator(int column) const;
    void setDefaultValue(int column, QVariant value);

protected:
    QString tableName;
    QString filter;
    QString sort;
    QString suffix;
    QVector<QVariant> defaultTmpRow;
    bool insertDb();
    bool updateDb();
    bool deleteDb(int row);
    
private:
    MData *modelData;
    DataEditor *editor;
    bool block;

signals:
    void sigUpd();
    void sigRefresh();
    
public slots:
    virtual bool select();
};


#endif // DBTABLEMODEL_H
