#ifndef TABLEVIEW_H
#define TABLEVIEW_H

#include <QWidget>
#include <QTableView>
#include <QHeaderView>
#include <QDir>
#include <QFileDialog>
#include <QSettings>
#include <QApplication>
#include <QMenu>
#include <QMessageBox>
#include <QContextMenuEvent>

class TableView : public QTableView
{
        Q_OBJECT
public:
    TableView(QWidget *parent=nullptr);

public slots:
    void resizeToContents();
};

#endif // TABLEVIEW_H
