#include "console.h"

//This part is for input box

Console::Console(QWidget* parent):QPlainTextEdit(parent){
    connect(this,SIGNAL(cursorPositionChanged()), this, SLOT(resetCursorPosition()));
    QColor editorColor;
    editorColor.setRgb(255,255,255); //editor background: White, RGB
    //set background color
    QPalette palette = this->palette();
    palette.setColor(QPalette::Active, QPalette::Base, editorColor);
    palette.setColor(QPalette::Inactive, QPalette::Base, editorColor);
    palette.setColor(QPalette::Text, Qt::black); //text color: Black, CMYK
    this->setPalette(palette);
}


/* press event */
void Console::keyPressEvent(QKeyEvent *event){
    if(event->key() == Qt::Key_Backspace && this->textCursor().atBlockStart()) return;
    if(event->key() == Qt::Key_Return){
        QString data = (this->textCursor()).block().text() + tr("\n");
        qDebug() << "sending data:  " << data;
        parentWindow->inputData(data);
    }
    QPlainTextEdit::keyPressEvent(event);
}


/* Reset the cursor's position */
void Console::resetCursorPosition(){
    QTextCursor cursor = this->textCursor();
    cursor.movePosition(QTextCursor::End, QTextCursor::MoveAnchor);
    this->setTextCursor(cursor);
}
