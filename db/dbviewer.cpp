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
    saveAct = new QAction(tr("Сохранить"),this);
    this->setAutoScroll(true);
    this->setItemDelegate(new DbDelegate(this));
    writeOk=true;
    connect(updAct,SIGNAL(triggered()),this,SLOT(upd()));
    connect(removeAct,SIGNAL(triggered()),this,SLOT(remove()));
    connect(saveAct,SIGNAL(triggered()),this,SLOT(save()));
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
                sqlModel->escAdd();
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
                } else
                    //this->setCurrentIndex(this->moveCursor(this->MoveNext,Qt::NoModifier));
                    QTableView::keyPressEvent(e);
                break;
            }
            default:
            {
                QTableView::keyPressEvent(e);
                break;
            }
        }

    } else
        QTableView::keyPressEvent(e);
}


void DbViewer::upd()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    if (sqlModel){
        sqlModel->select();
    }
}

void DbViewer::remove()
{
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
    QModelIndex ind=this->currentIndex();
    if (sqlModel && sqlModel->rowCount() && sqlModel->removeRow(ind.row()))
        setCurrentIndex(model()->index(ind.row()-1,ind.column()));
}

void DbViewer::save()
{
    //DbXlsx x(this,QString("Сохранить в файл"));
    //x.saveToFile();
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
        if (sqlModel->isAdd() && !sqlModel->isEdt() && oldInd.row()!=sqlModel->rowCount()-2)
            sqlModel->escAdd();
        else if ((sqlModel->isEdt() && !sqlModel->isAdd()) || (sqlModel->isAdd() && ind.row()!=sqlModel->rowCount()-1)){
            writeOk=sqlModel->submitRow();
        }
    }
}

void DbViewer::focusOutEvent(QFocusEvent *event)
{
    if (this->editTriggers()!=QAbstractItemView::NoEditTriggers && event->reason()==Qt::MouseFocusReason){
        DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
        if (sqlModel && sqlModel->isAdd() && !sqlModel->isEdt())
            sqlModel->escAdd();
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
        if (this->editTriggers()!=QAbstractItemView::NoEditTriggers){
            DbTableModel *sqlModel = qobject_cast<DbTableModel *>(this->model());
            if (sqlModel){
                if ((sqlModel->rowCount()>0 && !sqlModel->isAdd()) || sqlModel->rowCount()>1){
                    menu.addAction(removeAct);
                    menu.addSeparator();
                }
            }
        }
        //menu.addAction(saveAct);
        //menu.addSeparator();
    }
    menu.exec(event->globalPos());
}

DateEdit::DateEdit(QWidget *parent): QDateEdit(parent)
{
    this->setCalendarPopup(true);
    CustomCalendarWidget * pCW = new CustomCalendarWidget(this);
    pCW->setFirstDayOfWeek( Qt::Monday );
    this->setCalendarWidget( pCW );
    this->setDisplayFormat("dd.MM.yy");
    this->setSpecialValueText("NULL");
    connect(this->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(txtChangeSlot(QString)));
    connect(pCW,SIGNAL(shown()),this,SLOT(shVid()));
}

void DateEdit::txtChangeSlot(QString txt)
{
    if (txt.isEmpty()){
        this->setDate(this->minimumDate());
    }
}

void DateEdit::shVid()
{
    if (QDateEdit::date()==this->minimumDate()){
        this->setDate(QDate::currentDate());
    }
}

CustomCalendarWidget::CustomCalendarWidget(QWidget *parent) : QCalendarWidget(parent)
{
    this->setFirstDayOfWeek( Qt::Monday );
}

void CustomCalendarWidget::showEvent(QShowEvent *event)
{
    if (event->type() == QEvent::Show){
        emit shown();
    }
    QCalendarWidget::showEvent(event);
}
