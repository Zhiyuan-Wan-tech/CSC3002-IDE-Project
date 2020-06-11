/*
 * File: codeeditor.h
 * --------------
 * This interface exports the design of text area.
 */

#ifndef CODEEDITOR_H
#define CODEEDITOR_H

#include <QPlainTextEdit>
#include <QObject>
#include <QPainter>
#include <QListWidget>
#include <QListWidgetItem>
#include "completelistwidget.h"
#include <algorithm>

QT_BEGIN_NAMESPACE
class QPaintEvent;
class QResizeEvent;
class QSize;
class QWidget;
QT_END_NAMESPACE

class LineNumberArea;

//code editor definition

class CodeEditor : public QPlainTextEdit {
    Q_OBJECT

public:
    CodeEditor(QWidget *parent = 0);

    void paintLineNumberArea(QPaintEvent *event);
    int widthOfLineNumberArea();
    void setCompleteList();

protected:
    void resizeEvent(QResizeEvent *event) override;
    void keyPressEvent(QKeyEvent *event) override;

private slots:
    void updateWidthOfLineNumberArea(int newBlockCount);
    void highlightCurrentLine();
    void updateLineNumberArea(const QRect &, int);
    void showCompleteWidget();

private:
    QWidget *lineNumberArea;
    QColor lineColor;
    QColor editorColor;
    QStringList completeList; //stores automatically filled keywords

    CompleteListWidget *completeWidget;
    QString getWordOfCursor();
    int completeState;
    int getCompleteWidgetX();
};


class LineNumberArea : public QWidget{
public:
    LineNumberArea(CodeEditor *editor) : QWidget(editor) {
        codeEditor = editor;
    }

    QSize sizeHint() const override {
        return QSize(codeEditor->widthOfLineNumberArea(), 0);
    }

protected:
    void paintEvent(QPaintEvent *event) override {
        codeEditor->paintLineNumberArea(event);

    }

private:
    CodeEditor *codeEditor;
};


enum CompleteState{
    Ignore = 0,
    Showing = 1 ,
    Hide = 2
};



#endif
