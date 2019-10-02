#include "tabwidget.h"

TabWidget::TabWidget(QWidget *parent) : QTabWidget(parent)
{
    this->setTabsClosable(true);
    this->setDocumentMode(true);
    this->setMovable(true);
    connect(this,SIGNAL(tabCloseRequested(int)),this,SLOT(closeTab(int)));
}

TabWidget::~TabWidget()
{
}

void TabWidget::addTabAction(QWidget *tab, QAction *action, bool en)
{
    tab->setEnabled(en);
    map.insert(action,tab);
    if (action->isChecked()){
        this->addTab(tab,action->text());
    }
    connect(action,SIGNAL(triggered(bool)),this,SLOT(newTab(bool)));
}

void TabWidget::loadSettings()
{
    QSettings settings("szsm", "el_sert");
    QAction *a;
    QMap<QAction*,QWidget*>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        if (settings.value("tab_"+i.key()->objectName(),false).toBool()){
            if (!i.key()->isChecked()) i.key()->trigger();
        }
        if (settings.value("tab_currentindex")==i.key()->objectName()){
            a=i.key();
        }
        ++i;
    }
    if (a){
        this->setCurrentWidget(map.value(a));
    }
}

void TabWidget::saveSettings()
{
    QSettings settings("szsm", "el_sert");
    QMap<QAction*,QWidget*>::const_iterator i = map.constBegin();
    while (i != map.constEnd()) {
        settings.setValue("tab_"+i.key()->objectName(),i.key()->isChecked());
        ++i;
    }
    if (this->currentWidget()){
        settings.setValue("tab_currentindex",map.key(this->currentWidget())->objectName());
    }
}

void TabWidget::closeTab(int index)
{
    QAction *a=map.key(this->widget(index));
    if (a){
        a->setChecked(false);
    }
    this->removeTab(index);
}

void TabWidget::newTab(bool b)
{
    QAction *a=qobject_cast<QAction *>(this->sender());
    if (a){
        QWidget *w=map.value(a);
        if (w && b){
            if (this->indexOf(w)!=-1){
                this->setCurrentWidget(w);
            } else {
                this->addTab(w,a->text());
                this->setCurrentWidget(w);
            }
        } else if (w){
            this->removeTab(this->indexOf(w));
        }
    }
}
