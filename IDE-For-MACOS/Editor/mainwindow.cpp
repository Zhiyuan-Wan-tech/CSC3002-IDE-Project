#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFileDialog>
#include <QFile>
#include <QTextStream>
#include <unistd.h>
#include <iostream>


MainWindow::MainWindow(QWidget *parent) :

    QMainWindow(parent),
    ui(new Ui::MainWindow){
    firstLoad = true;  // initialize
    isediting = false;
    ui->setupUi(this);
    setUpHighlighter();


    /*
     * status bar setting
     * ----------------------------
     * At the beginng, show the initial information
     */
    ui->outputText->parentWindow = this;
    ui->statusBar->showMessage(tr("You can begin now."));

    /*
     * tool bar setting
     * ----------------------------
     * Set the color for the tool bar
     */
    ui->mainToolBar->setMovable(false);
    ui->mainToolBar->setStyleSheet("QToolButton:hover {background-color:lightgray} QToolBar {background: rgb(255,255,255);border: none;}");

    runIcon.addPixmap(QPixmap(":/image/Run.png"));
    stopIcon.addPixmap(QPixmap(":/image/stop.png"));

    /*
     * background color setting
     * ----------------------------
     * Set the color for the background
     */
    QPalette windowPalette=this->palette();
    windowPalette.setColor(QPalette::Active, QPalette::Window, QColor(230,230,250)); //When active, color name is lavender

    this->setPalette(windowPalette);

    /*
     * connect all the buttons with corresponding functions
     * ----------------------------
     * For all the functions inside the editor, connect them to ui bottons
     * For functions outside the editor, connect them to Qprocess
     */
    initFileData();

    //All the buttons in tool bar
    connect(ui->actionNewFile, SIGNAL(triggered(bool)), this, SLOT(newFile()));
    connect(ui->actionOpen, SIGNAL(triggered(bool)), this, SLOT(openFile()));
    connect(ui->actionSave_File, SIGNAL(triggered(bool)), this, SLOT(saveFile()));
    connect(ui->actionUndo, SIGNAL(triggered(bool)), this, SLOT(undo()));
    connect(ui->actionRedo, SIGNAL(triggered(bool)), this, SLOT(redo()));
    connect(ui->actionRun, SIGNAL(triggered(bool)), this, SLOT(run()));
    connect(ui->actionAbout, SIGNAL(triggered(bool)), this, SLOT(about()));
    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(changeSaveState()));
    connect(ui->editor, SIGNAL(textChanged()), this, SLOT(updateMessage()));

    connect(&process, SIGNAL(finished(int)), this, SLOT(runFinished(int)));
    connect(&process, SIGNAL(readyReadStandardOutput()), this, SLOT(updateOutput()));
    connect(&process, SIGNAL(readyReadStandardError()), this, SLOT(updateError()));


    fileSaved = true;
}


MainWindow::~MainWindow(){
    delete ui;
}


/*
 * set highlight
 * ----------------------------
 * Set font and font size
 * The font is "Courier new", size is 18
 * Courier new is the most commonly used font in programming
 * Set highlight for text in the editor
 */
void MainWindow::setUpHighlighter(){

    QFont font;

    font.setFamily("Courier new");
    font.setFixedPitch(true);
    font.setPointSize(18);
    ui->editor->setFont(font);
    ui->editor->setTabStopWidth(fontMetrics().width(QLatin1Char('9')) * 4);
    highlighter = new Highlighter(ui->editor->document());
}


/*
 * Set the size of the input and output boxes
 * ----------------------------
 * Rely on the relative distance and size of the toolbar and status bar
 * to set the size of the input and output boxes
 */
void MainWindow::resizeEvent(QResizeEvent *event){
    QMainWindow::resizeEvent(event);
    ui->editor->setGeometry(0, 0, width(), height()-ui->statusBar->height()-ui->mainToolBar->height()-100);
    ui->outputText->setGeometry(0, ui->editor->height(), this->width(), 100);
}


/*
 * Set initial status of a file
 * ----------------------------
 * When create a new file or open the editor,
 * the status is initially set to a file named "Untitled.txt"
 */
void MainWindow::initFileData(){
    fileName = tr("Untitled.txt");
    filePath = tr("~/Desktop/Untitled.txt");
    fileSaved = true;
    isRunning = false;
}


/*
 * Set undo and redo functions
 * ----------------------------
 * When click the "undo" and "redo" buttons, it will do immediately
 */
void MainWindow::undo(){
    ui->editor->undo();
}

void MainWindow::redo(){
    ui->editor->redo();
}


/*
 * Save the file
 * ----------------------------
 * Choose the path and file name to save the file
 */
void MainWindow::saveFile(){
    QString savePath=QFileDialog::getSaveFileName(this, tr("Choose the path and file name"), fileName, tr("*.txt"));
    if(!savePath.isEmpty()){
        QFile out(savePath);
        out.open(QIODevice::WriteOnly| QIODevice::Text);
        QTextStream str(&out);
        str << ui->editor->toPlainText();
        out.close();
        fileSaved = true;
        QRegularExpression re(tr("(?<=\\/)\\w+\\.txt"));
        fileName = re.match(savePath).captured();
        filePath = savePath;
        this->setWindowTitle(fileName + tr(" - Editor"));
    }
}


/*
 * create a new file
 * ----------------------------
 * When click the "New file" button, a window will be shown
 */
void MainWindow::newFile(){

    MainWindow *newWindow = new MainWindow();
    //Set the relative position of the new window and the original window
    QRect newPos = this->geometry();
    newWindow->setGeometry(newPos.x()+15, newPos.y()+15, newPos.width(), newPos.height());
    newWindow->show();
}


/*
 * open a file
 * ----------------------------
 * When click the "Open..." button, a window will be shown for users to choose the file path and name
 */
void MainWindow::openFile(){

    //If the current file has not been saved, save it first
    if(!fileSaved){
        if(QMessageBox::Save == QMessageBox::question(this, tr("The file has not been saved."), tr("Do you want to save this file?"), QMessageBox::Save, QMessageBox::Cancel))
          saveFile();

    }

    //Search for the correct suffix
    QString openPath = QFileDialog::getOpenFileName(this,tr("Open a file"),filePath,tr("*.txt"));

    if(!openPath.isEmpty()){
        QFile in(openPath);
        in.open(QIODevice::ReadOnly|QIODevice::Text);
        QTextStream str(&in);
        ui->editor->setPlainText(str.readAll());
        QRegularExpression re(tr("(?<=\\/)\\w+\\.txt"));
        fileName = re.match(openPath).captured();
        this->setWindowTitle(fileName + tr(" - Editor"));
        filePath = openPath;
        fileSaved = true;
    }

}



void MainWindow::run(){
  if(isRunning){
      process.terminate();
      ui->actionRun->setIcon(runIcon);
      return;
    }
  if(!fileSaved){
      char* path;
      path = getcwd(NULL, 0);
      string newpath = path;
      newpath = newpath.substr(0,newpath.find("Project")+7);
      free(path);

      if(QMessageBox::Save==QMessageBox::question(this,tr("Unsaved"),tr("Can only run after saving."),QMessageBox::Save,QMessageBox::Cancel))
        //在这里加上copy到指定位置,其他部分所在的文件夹
        fileName2 = QString::fromStdString(newpath+"/Files/arithemetic.txt");
        QFile otherPath(fileName2);
        otherPath.open(QIODevice::WriteOnly| QIODevice::Text);
        QTextStream str(&otherPath);
        str << ui->editor->toPlainText();
        otherPath.close();

        saveFile();


    }
  if(fileSaved){
    //if(process!=nullptr)delete process;
    isRunning=true;
    ui->statusBar->showMessage(tr("Processing..."));
    ui->outputText->clear();
    output.clear();
    error.clear();

    char* path;
    path = getcwd(NULL, 0);
    string newpath = path;
    newpath = newpath.substr(0,newpath.find("Project")+7);
    free(path);

    QString systempath = QString::fromStdString(newpath+"/Original.app");

    process.start(systempath);
    process.waitForStarted();
//    process.waitForFinished(-1);


    ui->outputText->setFocus();
    ui->actionRun->setIcon(stopIcon);
    }
}


/*
 * Show the information/running tips of the whole project
 * ----------------------------
 * When click the "About" button, a window carrying some information will be shown
 */
void MainWindow::about(){
    QMessageBox::information(this,tr("About"),tr("Support:\n"
                                                 "Structures: if, for, while\n"
                                                 "Arithmetic: + —  ÷ \n"
                                                 "Data types: Int, Char, String\n"
                                                 "Operations: Input Output"), QMessageBox::Yes);
}



/*
 * After finishng running a file
 * ----------------------------
 * When the run process finished, show some information
 */
void MainWindow::runFinished(int code){
    ui->actionRun->setIcon(runIcon);
    isRunning = false;
    qDebug() << tr("exit code=") << code;
    ui->statusBar->showMessage(tr("Ready"));
}

void MainWindow::updateMessage(){
    if(isediting == true){
        ui->statusBar->showMessage(tr("Texting..."));
    }
}

/*
 * Show the output
 * ----------------------------
 * When the run process finished, show the output
 */
void MainWindow::updateOutput(){
    output = QString::fromLocal8Bit(process.readAllStandardOutput());
    //如果需要换行符，就写成
    //ui->outputText->setPlainText(ui->outputText->toPlainText() + output + tr("\n"));
    ui->outputText->setPlainText(ui->outputText->toPlainText() + output);
}


/*
 * Show the error
 * ----------------------------
 * When the run process finished, show the error
 */
void MainWindow::updateError(){
    error = QString::fromLocal8Bit(process.readAllStandardError());
    //如果需要换行符，就写成
    //ui->outputText->setPlainText(ui->outputText->toPlainText() + error + tr("\n"));
    ui->outputText->setPlainText(ui->outputText->toPlainText() + error);
    process.terminate();
    isRunning = false;
}


/*
 * input the data
 * ----------------------------
 * when it is running, input the data to process
 */
void MainWindow::inputData(QString data){
    if(isRunning)process.write(data.toLocal8Bit());
}



/*
 * Choose whether to close the window
 * ----------------------------
 * pop up a window to choose
 */
void MainWindow::closeEvent(QCloseEvent *event){

    //If the file has not been saved, ask and choose the status
    if(!fileSaved){
        if(QMessageBox::Save == QMessageBox::question(this, tr("The file has not been saved, whether to exit?"), tr("Warning! Do you want to save this file?"), QMessageBox::Save, QMessageBox::Cancel))
          saveFile();
        fileSaved = true;
    }else{
        event->accept(); //If the file has been saved, just close the event
    }
}


