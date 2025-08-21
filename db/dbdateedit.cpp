#include "dbdateedit.h"


DbDateEdit::DbDateEdit(QWidget *parent) : QDateEdit(parent)
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

void DbDateEdit::txtChangeSlot(QString txt)
{
    if (txt.isEmpty()){
        this->setDate(this->minimumDate());
    }
}

void DbDateEdit::shVid()
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

DbDateTimeEdit::DbDateTimeEdit(QWidget *parent) : QDateTimeEdit(parent)
{
    this->setCalendarPopup(true);
    CustomCalendarWidget * pCW = new CustomCalendarWidget(this);
    pCW->setFirstDayOfWeek( Qt::Monday );
    this->setCalendarWidget( pCW );
    this->setDisplayFormat("dd.MM.yyyy hh:mm");
    this->setSpecialValueText("NULL");

    connect(this->lineEdit(),SIGNAL(textChanged(QString)),this,SLOT(txtChangeSlot(QString)));
    connect(pCW,SIGNAL(shown()),this,SLOT(shVid()));
}

void DbDateTimeEdit::setDateTime(const QDateTime &dateTime)
{
    if (dateTime.isNull()){
        QDateTimeEdit::setDateTime(this->minimumDateTime());
    }
    QDateTimeEdit::setDateTime(dateTime);
}

void DbDateTimeEdit::clear()
{
    QDateTimeEdit::setDateTime(this->minimumDateTime());
}

void DbDateTimeEdit::txtChangeSlot(QString txt)
{
    if (txt.isEmpty()){
        this->setDateTime(this->minimumDateTime());
    }
}

void DbDateTimeEdit::shVid()
{
    if (QDateTimeEdit::dateTime()==this->minimumDateTime()){
        this->setDateTime(QDateTime::currentDateTime());
    }
}
