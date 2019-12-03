#include "dbviewer.h"

DbViewer::DbViewer(QWidget *parent) :
    QTableView(parent)
{
    setSelectionMode(QAbstractItemView::SingleSelection);
    menuEnabled=true;
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height()*1.5);
    verticalHeader()->setFixedWidth(verticalHeader()->fontMetrics().height()*1.2);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);

    updAct = new QAction(tr("Обновить"),this);
    removeAct = new QAction(tr("Удалить"),this);
    //saveAct = new QAction(tr("Сохранить"),this);
    this->setAutoScroll(true);
    this->setItemDelegate(new DbDelegate(this));
    writeOk=true;
    connect(updAct,SIGNAL(triggered()),this,SLOT(upd()));
    connect(removeAct,SIGNAL(triggered()),this,SLOT(remove()));
}

void DbViewer::setModel(QAbstractItemModel *model)
{
    QTableView::setModel(model);
    connect(this->selectionModel(),SIGNAL(currentRowChanged(QModelIndex,QModelIndex)),this,SLOT(submit(QModelIndex,QModelIndex)));
}

void DbViewer::setColumnsWidth(QVector<int> width)
{
    for (int i=0; i<width.size(); i++){
        setColumnWidth(i,width[i]);
    }
}

void DbViewer::keyPressEvent(QKeyEvent *e)
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    if (sqlModel && this->editTriggers()!=QAbstractItemView::NoEditTriggers){
        int c=sqlModel->rowCount();
        int row=currentIndex().row();

        switch (e->key()){
            case Qt::Key_Delete:
            {
                if (e->modifiers()==Qt::ControlModifier) remove();
                break;
            }
            case Qt::Key_Escape:
            {
                sqlModel->revert();
                break;
            }
            case Qt::Key_Down:
            {
                if ((row==c-1) || !c){
                    sqlModel->insertRow(sqlModel->rowCount());
                    scrollToBottom();
                }
                QTableView::keyPressEvent(e);
                break;
            }
            case Qt::Key_Tab:
            {
                int j=sqlModel->columnCount()-1;
                bool hidden=isColumnHidden(j);
                while (hidden && j>0){
                    j--;
                    hidden=this->isColumnHidden(j);
                }

                int i=0;
                hidden=isColumnHidden(i);
                while (hidden && i<sqlModel->columnCount()){
                    i++;
                    hidden=this->isColumnHidden(i);
                }

                if ((currentIndex().column()==j) && (row==sqlModel->rowCount()-1)) {
                    sqlModel->insertRow(sqlModel->rowCount());
                    setCurrentIndex(this->model()->index(sqlModel->rowCount()-1,i));
                } else {
                    QTableView::keyPressEvent(e);
                }
                break;
            }
            default:
            {
                QTableView::keyPressEvent(e);
                break;
            }
        }

    } else {
        QTableView::keyPressEvent(e);
    }
}


void DbViewer::upd()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    if (sqlModel) {
        sqlModel->select();
    }
}

void DbViewer::remove()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    QModelIndex ind=this->currentIndex();
    if (sqlModel && sqlModel->rowCount() && sqlModel->removeRow(ind.row())){
        setCurrentIndex(model()->index(ind.row()-1,ind.column()));
    }
}

void DbViewer::submit(QModelIndex ind, QModelIndex oldInd)
{
    if (this->editTriggers()==QAbstractItemView::NoEditTriggers) return;
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    if (sqlModel){
        if (!writeOk){
            writeOk=true;
            return;
        }
        if (sqlModel->isAdd() && !sqlModel->isEdt() && oldInd.row()!=sqlModel->rowCount()-2){
            sqlModel->revert();
        } else if ((sqlModel->isEdt() && !sqlModel->isAdd()) || (sqlModel->isAdd() && ind.row()!=sqlModel->rowCount()-1)){
            writeOk=sqlModel->submit();
        }
    }
}

void DbViewer::focusOutEvent(QFocusEvent *event)
{
    if (this->editTriggers()!=QAbstractItemView::NoEditTriggers && event->reason()==Qt::MouseFocusReason){
        DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
        if (sqlModel && sqlModel->isAdd() && !sqlModel->isEdt()){
            sqlModel->revert();
        }
    }
    return QTableView::focusOutEvent(event);
}

void DbViewer::setMenuEnabled(bool value)
{
    menuEnabled=value;
}

void DbViewer::contextMenuEvent(QContextMenuEvent *event)
{
    QMenu menu(this);
    if (menuEnabled){
        menu.addAction(updAct);
        menu.addSeparator();
        if (this->selectionModel()){
            if (this->selectionModel()->currentIndex().isValid()){
                menu.addAction(removeAct);
                menu.addSeparator();
            }
        }
    }
    menu.exec(event->globalPos());
}

DateEdit::DateEdit(QWidget *parent): QDateEdit(parent)
{
    this->setCalendarPopup(true);
    QCalendarWidget * pCW = new QCalendarWidget(this);
    pCW->setFirstDayOfWeek( Qt::Monday );
    this->setCalendarWidget( pCW );
    this->setDisplayFormat("dd.MM.yy");
}
