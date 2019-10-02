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
#include "sertbuild.h"

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

    void setupTextActions();
    void mergeFormatOnWordOrSelection(const QTextCharFormat &format);

public slots:
    void filePrint();
    void filePrintPreview();
    void exportPdf();
    void exportPdf(QString filename);
    void print(QPrinter *p);
    void drawDoc(QPainter *painter);

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
    void setObr(bool b);

};

#endif // EDITOR_H
