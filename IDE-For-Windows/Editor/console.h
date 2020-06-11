/*
 * File: console.h
 * --------------
 * This interface is related to the graphical console window,
 * like editor background color, cursor.
 */

#ifndef CONSOLE_H
#define CONSOLE_H

#include <QObject>
#include <QWidget>
#include <QPlainTextEdit>
#include <QDebug>
#include <QTextBlock>
#include "mainwindow.h"



class Console : public QPlainTextEdit {
    Q_OBJECT

public:
    Console(QWidget* parent = 0);
    MainWindow *parentWindow;

protected:
    void keyPressEvent(QKeyEvent *event)override;

protected slots:
    void resetCursorPosition();
};

#endif // CONSOLE_H
