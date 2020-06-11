/*
 * File: mainwindow.h
 * --------------
 * This interface is the combination of UI,
 * highlighter settings designed in highlighter.h
 * file operations and editor basic fuctions
 * and outside processes
 */

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QRegularExpression>
#include <QMessageBox>
#include <QProcess>
#include <QDebug>
#include "highlighter.h"


namespace Ui{
    class MainWindow;
}

class MainWindow : public QMainWindow{
    Q_OBJECT

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    void inputData(QString data);

private:
    QIcon runIcon;
    QIcon stopIcon;
    Ui::MainWindow *ui;
    Highlighter *highlighter;
    QProcess process;
    void setUpHighlighter();

    /*
     * File information
     * ----------------------------
     * This part is used to record all information about a file
     */
    QString fileName;
    QString filePath;
    QString fileName2;
    bool fileSaved;
    bool isRunning;
    bool firstLoad;
    bool isediting;
    void initFileData();


    /*
     * Running data
     * ----------------------------
     * This part is used to record the running data and error
     */
    QString output;
    QString error;

// open for receiving signals （connect signals with SLOT)
public slots:

    /*
     * Title appearance
     * ----------------------------
     * This part is used to show the file state
     * If the file has been saved or we just open it, the title of the window will be "Editor - xxx"
     * If not, the title of the window will be "Editor - xxx*"
     */
    void changeSaveState(){
        if(firstLoad && fileSaved){
        this->setWindowTitle(fileName + tr(" - Editor"));
        firstLoad = false;
        return;
        }

    fileSaved = false;
    isediting = true;
    this->setWindowTitle(fileName + tr("*") + tr(" - Editor"));
    }


    /*
     * Functions of the function(tool) bar
     * ----------------------------
     * This part includes functions for
     * create a new file
     * save a file
     * open a file
     * undo/redo while editing
     * run in other process
     */
    void newFile();
    void saveFile();
    void openFile();
    void undo();
    void redo();
    void run();
    void runFinished(int code);
    void updateMessage();
    void updateOutput();
    void updateError();
    void about();

protected:
    void resizeEvent(QResizeEvent* event)override;
    void closeEvent(QCloseEvent* event)override;
};

#endif
