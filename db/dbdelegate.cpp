#include "dbdelegate.h"
#include <QLineEdit>

DbDelegate::DbDelegate(QObject *parent): QItemDelegate(parent)
{

}

QWidget *DbDelegate::createEditor (QWidget * parent, const QStyleOptionViewItem & option, const QModelIndex & index ) const
{
    const DbTableModel *sqlModel = qobject_cast<const DbTableModel *>(index.model());
    if (!sqlModel) return QItemDelegate::createEditor(parent, option, index);
    QWidget *editor=NULL;
    if (sqlModel->relation(index.column())){
        QAbstractItemModel *childModel=sqlModel->relation(index.column())->model();
        if (!childModel){
            editor=QItemDelegate::createEditor(parent, option, index);
        } else {
            QComboBox *combo = new QComboBox(parent);
            combo->setLineEdit(new ComboLineEdit(parent));
            editor=combo;
        }
    } else {
        switch (sqlModel->columnType(index.column())){
            case QMetaType::Bool:
            {
                editor=NULL;
                break;
            }
            case QMetaType::QString:
            {
                editor=new QLineEdit(parent);
                break;
            }
            case QMetaType::Int:
            {
                editor= sqlModel->data(index,Qt::CheckStateRole).isNull() ? new QLineEdit(parent) : NULL;
                break;
            }
            case QMetaType::Short:
            {
                editor= new QLineEdit(parent);
                break;
            }
            case QMetaType::Long:
            {
                editor= new QLineEdit(parent);
                break;
            }
            case QMetaType::LongLong:
            {
                editor= new QLineEdit(parent);
                break;
            }
            case QMetaType::Double:
            {
                editor=new QLineEdit(parent);
                break;
            }
            case QMetaType::Float:
            {
                editor= new QLineEdit(parent);
                break;
            }
            case QMetaType::QDate:
            {
                editor= new CustomDateEdit(parent);
                break;
            }
            default:
            {
                editor=QItemDelegate::createEditor(parent, option, index);
                break;
            }
        }
    }
    if (editor) {
        editor->installEventFilter(const_cast<DbDelegate *>(this));
        emit createEdt(index);
    }
    return editor;
}

void DbDelegate::setEditorData ( QWidget * editor, const QModelIndex & index ) const
{
    const DbTableModel *sqlModel = qobject_cast<const DbTableModel *>(index.model());
    if (sqlModel){
        if (sqlModel->relation(index.column())){
            QComboBox *combo = qobject_cast<QComboBox *>(editor);
            if (combo) {
                QAbstractItemModel *childModel=sqlModel->relation(index.column())->model();
                if (childModel){
                    combo->setModel(sqlModel->relation(index.column())->proxyModel());
                    combo->setModelColumn(sqlModel->relation(index.column())->columnDisplay());
                    combo->setEditable(true);
                    CustomCompletter *c = new CustomCompletter(combo);
                    c->setModel(sqlModel->relation(index.column())->proxyModel());
                    c->setCompletionColumn(sqlModel->relation(index.column())->columnDisplay());
                    combo->setCompleter(c);
                    int pos=combo->findText(sqlModel->data(index).toString());
                    if (pos!=-1){
                        combo->setCurrentIndex(pos);
                    } else {
                        combo->lineEdit()->setText(sqlModel->data(index).toString());
                    }
                    return;
                }
            } else {
                QLineEdit *le = qobject_cast<QLineEdit *>(editor);
                if (le){
                    le->setText(sqlModel->data(index,Qt::DisplayRole).toString());
                    return;
                }
            }
        }
        if (sqlModel->columnType(index.column()==QMetaType::QDate)){
            CustomDateEdit *dateEdit = qobject_cast<CustomDateEdit *>(editor);
            if (dateEdit){
                QVariant dat=sqlModel->data(index,Qt::EditRole);
                if (dat.isNull()){
                    dateEdit->setDate(dateEdit->minimumDate());
                } else {
                    dateEdit->setDate(dat.toDate());
                }
                return;
            }
        }
        if (sqlModel->validator(index.column())){
            QLineEdit *line = qobject_cast<QLineEdit *>(editor);
            if (line) line->setValidator(sqlModel->validator(index.column()));
        }
    }
    return QItemDelegate::setEditorData(editor, index);
}

void DbDelegate::setModelData ( QWidget * editor, QAbstractItemModel * model, const QModelIndex & index ) const
{
    if (!index.isValid())
        return;
    DbTableModel *sqlModel = qobject_cast<DbTableModel *>(model);
    if (sqlModel) {
        if (sqlModel->relation(index.column())){
            QComboBox *combo = qobject_cast<QComboBox *>(editor);
            if (combo) {
                QString text=combo->currentText();
                QVariant v;
                int pos=combo->findText(text);
                if (pos>=0){
                    combo->setCurrentIndex(pos);
                    v=combo->model()->data(combo->model()->index(combo->currentIndex(),sqlModel->relation(index.column())->columnKey()),Qt::EditRole);
                } else if (!text.isEmpty()) {
                    QSortFilterProxyModel *fmodel = qobject_cast<QSortFilterProxyModel *>(combo->model());
                    if (fmodel){
                        DbTableModel *m = qobject_cast<DbTableModel *>(fmodel->sourceModel());
                        if (m){
                            int n=QMessageBox::information(NULL,QString::fromUtf8("Предупреждение"),QString::fromUtf8("Не найдено значение ")+text+QString::fromUtf8(". Добавить его в таблицу?"),QMessageBox::Yes,QMessageBox::No);
                            if (n==QMessageBox::Yes){
                                m->insertRow(m->rowCount());
                                if (m->isAdd()){
                                    m->setData(m->index(m->rowCount()-1,sqlModel->relation(index.column())->columnDisplay()),text,Qt::EditRole);
                                    if (m->submit()){
                                        v=m->data(m->index(m->rowCount()-1,sqlModel->relation(index.column())->columnKey()),Qt::EditRole);
                                    }
                                }
                            }
                        }
                    }
                }
                QVariant val = v.isNull() ? sqlModel->nullVal(index.column()) : v;
                sqlModel->setData(index,val,Qt::EditRole);
                return;
            }

        } else {
            QLineEdit *le = qobject_cast<QLineEdit *>(editor);
            if (le){
                if (le->text().isEmpty()) {
                    sqlModel->setData(index,sqlModel->nullVal(index.column()),Qt::EditRole);
                    return;
                }
            }
            if (sqlModel->columnType(index.column())==QMetaType::QDate){
                CustomDateEdit *dateEdit = qobject_cast<CustomDateEdit *>(editor);
                if (dateEdit){
                    if (dateEdit->date()==dateEdit->minimumDate()){
                        sqlModel->setData(index,sqlModel->nullVal(index.column()),Qt::EditRole);
                    } else {
                        sqlModel->setData(index,dateEdit->date(),Qt::EditRole);
                    }
                    return;
                }
            }
            return QItemDelegate::setModelData(editor, model, index);
        }
    } else {
        return QItemDelegate::setModelData(editor, model, index);
    }
}

void DbDelegate::updateEditorGeometry(
            QWidget *editor,
            const QStyleOptionViewItem &option,
            const QModelIndex& /* index */) const
{
    editor->setGeometry(option.rect);
}

bool DbDelegate::eventFilter(QObject *object, QEvent *event)
{
    if (event->type()== QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(event);
        //qDebug()<<keyEvent;
        if (keyEvent->text()=="\r"){
            QWidget *editor = qobject_cast<QWidget*>(object);
            emit commitData(editor);
            emit closeEditor(editor);
            return true;
        }
        if(keyEvent->key()==Qt::Key_Tab || keyEvent->key()==Qt::Key_Down || keyEvent->key()==Qt::Key_Up){
            QWidget *editor = qobject_cast<QWidget*>(object);
            emit commitData(editor);
            emit closeEditor(editor);
            return false;
        }

        QLineEdit *line = qobject_cast<QLineEdit *>(object);
        if (line && line->validator()) {
            const QDoubleValidator *val = qobject_cast< const QDoubleValidator *>(line->validator());
            if (val){
                if (keyEvent->text()==",") {
                    line->insert(".");
                    return true;
                }
            }
        }
    }
    return QItemDelegate::eventFilter(object,event);
}

CustomCompletter::CustomCompletter(QObject *parent):QCompleter(parent)
{
    setCompletionMode(QCompleter::PopupCompletion);
    setCaseSensitivity(Qt::CaseInsensitive);
}

bool CustomCompletter::eventFilter(QObject *o, QEvent *e)
{
    if (e->type()==QEvent::KeyPress){
        QKeyEvent *keyEvent = static_cast<QKeyEvent *>(e);
        //qDebug()<<keyEvent;
        if (keyEvent->key()==Qt::Key_Tab) {
            this->popup()->close();
            return false;
        }
    }
    return QCompleter::eventFilter(o,e);
}

ComboLineEdit::ComboLineEdit(QWidget *parent) : QLineEdit(parent)
{

}

void ComboLineEdit::keyPressEvent(QKeyEvent *e)
{
    if (e->text()==",") {
        insert(".");
        return;
    } else {
        return QLineEdit::keyPressEvent(e);
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

CustomDateEdit::CustomDateEdit(QWidget *parent) : QDateEdit(parent)
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

void CustomDateEdit::txtChangeSlot(QString txt)
{
    if (txt.isEmpty()){
        this->setDate(this->minimumDate());
    }
}

void CustomDateEdit::shVid()
{
    if (QDateEdit::date()==this->minimumDate()){
        this->setDate(QDate::currentDate());
    }
}
