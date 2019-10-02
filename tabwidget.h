#ifndef TABWIDGET_H
#define TABWIDGET_H
#include <QTabWidget>
#include <QAction>
#include <QMap>
#include <QDebug>
#include <QSettings>

class TabWidget : public QTabWidget
{
    Q_OBJECT
public:
    TabWidget(QWidget *parent=0);
    ~TabWidget();
    void addTabAction(QWidget *tab, QAction *action, bool en = true);
    void loadSettings();
    void saveSettings();

private:
    QMap <QAction*,QWidget*> map;
private slots:
    void closeTab(int index);
    void newTab(bool b);
};

#endif // TABWIDGET_H
