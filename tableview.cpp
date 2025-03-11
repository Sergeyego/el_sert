#include "tableview.h"

TableView::TableView(QWidget *parent) : QTableView(parent)
{
    verticalHeader()->setDefaultSectionSize(verticalHeader()->fontMetrics().height()*1.5);
    verticalHeader()->setSectionResizeMode(QHeaderView::Fixed);
}


void TableView::resizeToContents()
{
    if (!model()) return;
    int n=model()->columnCount();
    int m=model()->rowCount();
    int max=0;
    QStringList l;
    QString s;
    for (int i=0; i<n; i++){
        s=model()->headerData(i,Qt::Horizontal).toString();
        l=s.split("\n");
        max=0;
        for (int k=0; k<l.size(); k++){
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
            if (max<fontMetrics().width(l.at(k)))
                max=fontMetrics().width(l.at(k));
#else
            if (max<fontMetrics().horizontalAdvance(l.at(k)))
                max=fontMetrics().horizontalAdvance(l.at(k));
#endif
        }
        for (int j=0; j<m; j++){
            s=model()->data(model()->index(j,i)).toString();
            l=s.split("\n");
            for (int k=0; k<l.size(); k++){
#if (QT_VERSION < QT_VERSION_CHECK(5, 11, 0))
            if (max<fontMetrics().width(l.at(k)))
                max=fontMetrics().width(l.at(k));
#else
            if (max<fontMetrics().horizontalAdvance(l.at(k)))
                max=fontMetrics().horizontalAdvance(l.at(k));
#endif
            }
        }
        if (max>300) {
            max=300;
        }
        setColumnWidth(i,max+12);
    }
}
