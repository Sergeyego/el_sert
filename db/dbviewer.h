#ifndef DBVIEWER_H
#define DBVIEWER_H

#include <QtGui>
#include "dbtablemodel.h"
#include "dbdelegate.h"
#include <QTableView>
#include <QHeaderView>
#include <QAction>
#include <QMenu>
#include <QDateEdit>
#include <QCalendarWidget>

class QMenu;
class QAction;

class DbViewer : public QTableView
{
    Q_OBJECT
public:
    DbViewer(QWidget *parent = 0);
    void setModel(QAbstractItemModel *model);
    void setColumnsWidth(QVector<int> width);

protected:
    virtual void keyPressEvent (QKeyEvent * e );
    virtual void contextMenuEvent(QContextMenuEvent *event);

private:
    QAction *updAct;
    QAction *removeAct;
    QAction *saveAct;
    bool menuEnabled;
    bool writeOk;

private slots:
     void upd();
     void remove();
     void save();
     void submit(QModelIndex ind, QModelIndex oldInd);
     void focusOutEvent(QFocusEvent *event);

public slots:
     void setMenuEnabled(bool value);

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

class DateEdit : public QDateEdit{
    Q_OBJECT
public:
    DateEdit(QWidget *parent=0);
private slots:
    void txtChangeSlot(QString txt);
    void shVid();
};

#endif // DBVIEWER_H
