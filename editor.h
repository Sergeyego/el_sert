#ifndef EDITOR_H
#define EDITOR_H

#include <QWidget>
#include <QTextDocument>
#include "sertbuild.h"
#include <QPrinter>
#include <QToolBar>
#include <QComboBox>
#include <QFontComboBox>
#include <QColorDialog>
#include <QMenu>
#include <QTextEdit>
#include <QInputDialog>
#include <QCheckBox>
#include <QPdfWriter>
#include "sertbuild.h"
#include "httpsyncmanager.h"
#include "dialogsignature.h"
#include <QProgressDialog>

namespace Ui {
class Editor;
}

class TextEdit : public QTextEdit
{
    Q_OBJECT
public:
    TextEdit(QWidget *parent = 0);
    void contextMenuEvent(QContextMenuEvent *e);

private:
    QAction *actionAddRow;
    QAction *actionAddCol;
    QAction *actionDelRow;
    QAction *actionDelCol;
    QAction *actionMerge;
    QAction *actionUnMerge;
    QAction *actionAddTable;
    void paintEvent(QPaintEvent *e);

private slots:
    void addRow();
    void addCol();
    void delRow();
    void delCol();
    void mergeCells();
    void unmergeCells();
    void addTable();
};

class Editor : public QWidget
{
    Q_OBJECT
    
public:
    explicit Editor(QTextDocument *doc, QWidget *parent = 0);
    QTextDocument* document();
    ~Editor();
    QPrinter *printer;
    bool signDS(QString sn, QString fname = QString());
    void sign(QString fname);
    
private:
    Ui::Editor *ui;
    QToolBar *toolBar;
    QToolBar *toolBarFont;
    QAction *actionTextBold;
    QAction *actionTextUnderline;
    QAction *actionTextItalic;
    QAction *actionTextColor;
    QAction *actionAlignLeft;
    QAction *actionAlignCenter;
    QAction *actionAlignRight;
    QAction *actionAlignJustify;
    QAction *actionUndo;
    QAction *actionRedo;
    QComboBox *comboStyle;
    QFontComboBox *comboFont;
    QComboBox *comboSize;
    QVector <QCheckBox*> boxes;

    void setupTextActions();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);
    QString imBackPath();

public slots:
    void filePrint();
    void filePrintPreview();
    void exportPdf();
    void exportPdf(QString filename);
    void print(QPrinter *p);
    void drawDoc(QPainter *painter);
    void exportHtml();
    void signDS();
    void loadDS();

private slots:
    void textBold();
    void textItalic();
    void textUnderline();
    void textAlign(QAction *a);
    void textColor();
    void colorChanged(const QColor &c);
    void fontChanged(const QFont &f);
    void alignmentChanged(Qt::Alignment a);
    void currentCharFormatChanged(const QTextCharFormat &format);
    void cursorPositionChanged();
    void textStyle(int styleIndex);
    void textFamily(const QString &f);
    void textSize(const QString &p);
    void setObr();
    void chDoc();
    void setEnDoc(bool en);
    void setDefaultDoc();
    void loadHtml();
    void setLang();
    void setType();

signals:
    void signFinished();
    void signEnChanged(bool b);
};

#endif // EDITOR_H
