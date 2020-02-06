#ifndef DBDELEGATE_H
#define DBDELEGATE_H
#include <QtGui>
#include <QtSql>
#include "dbtablemodel.h"
#include <QDateEdit>
#include <QCompleter>
#include <QCalendarWidget>
#include <QLineEdit>

class DbDelegate : public QItemDelegate
{
    Q_OBJECT
public:
     DbDelegate(QObject *parent = 0);
     QWidget *createEditor(
                 QWidget *parent,
                 const QStyleOptionViewItem &option,
                 const QModelIndex &index) const;
     void setEditorData(QWidget *editor,
                        const QModelIndex &index) const;
     void setModelData(QWidget *editor,
                       QAbstractItemModel *model,
                       const QModelIndex &index) const;
     void updateEditorGeometry(
             QWidget *editor,
             const QStyleOptionViewItem &option,
             const QModelIndex &index) const;
     bool eventFilter(QObject *object, QEvent *event);

signals:
     void createEdt(const QModelIndex index) const;
};

class CustomCompletter : public QCompleter
{
    Q_OBJECT
public:
    CustomCompletter(QObject *parent=0);
    bool eventFilter(QObject *o, QEvent *e);

};

class ComboLineEdit : public QLineEdit
{
    Q_OBJECT
public:
    explicit ComboLineEdit(QWidget *parent = 0);
    void keyPressEvent(QKeyEvent *e);
};

class CustomCalendarWidget : public QCalendarWidget
{
    Q_OBJECT
public:
    CustomCalendarWidget(QWidget *parent = 0);
    virtual void showEvent(QShowEvent *event);
signals:
    void shown();
};

class CustomDateEdit : public QDateEdit{
    Q_OBJECT
public:
    CustomDateEdit(QWidget *parent=0);
private slots:
    void txtChangeSlot(QString txt);
    void shVid();
};
#endif // DBDELEGATE_H
