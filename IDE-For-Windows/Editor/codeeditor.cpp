#include <QtWidgets>
#include <QDebug>
#include "codeeditor.h"



CodeEditor::CodeEditor(QWidget *parent) : QPlainTextEdit(parent){
    // set for the line action of editor
    lineNumberArea = new LineNumberArea(this);

    connect(this, SIGNAL(updateRequest(QRect, int)), this, SLOT(updateLineNumberArea(QRect, int)));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(highlightCurrentLine()));
    connect(this, SIGNAL(cursorPositionChanged()), this, SLOT(showCompleteWidget()));

    updateWidthOfLineNumberArea(0);
    highlightCurrentLine();

    //set color value
    lineColor.setRgb(255, 255, 255);
    editorColor.setRgb(255, 255, 255);

    // initializes the complete list
    setCompleteList();
    completeWidget = new CompleteListWidget(this);
    completeWidget->hide();
    completeWidget->setMaximumHeight(fontMetrics().height()*5);
    completeState = CompleteState::Hide;
}


/*
 * set the width of line number area
 * ----------------------------
 * Count the number of characters
 */
int CodeEditor::widthOfLineNumberArea(){

    int digits = 1; //the number of digits in the number of rows
    int max = qMax(1, blockCount()); // at least one block
    while (max >= 10) {
        max /= 10;
        ++digits;
    }
    if(digits < 3) digits = 3;

    int space = 3 + fontMetrics().width(QLatin1Char('9')) * digits;

    return space;
}


/*
 * set the width of white space on the left
 * ----------------------------
 * Set all parameters to 0 as default value
 */
void CodeEditor::updateWidthOfLineNumberArea(int){
    setViewportMargins(widthOfLineNumberArea(), 0, 0, 0);
}


/*
 * set line with scrolling
 * ----------------------------
 * the number of lines to scroll while the text box is scrolling
 */
void CodeEditor::updateLineNumberArea(const QRect &rect, int yDirection){
    if (yDirection)
        lineNumberArea->scroll(0, yDirection);
    else
        lineNumberArea->update(0, rect.y(), lineNumberArea->width(), rect.height());
    // the rectangular of viewport widget
    if (rect.contains(viewport()->rect()))
        updateWidthOfLineNumberArea(0);

}


/*
 * reset the size of text plain
 * ----------------------------
 * according to the inside area
 */
void CodeEditor::resizeEvent(QResizeEvent *event){
    QPlainTextEdit::resizeEvent(event);
    // cr refers to the inside rectangular
    QRect cr = contentsRect();
    lineNumberArea->setGeometry(QRect(cr.left(), cr.top(), widthOfLineNumberArea(), cr.height()));
}


/*
 * highlight the current line
 * ----------------------------
 * Determine the currently active line based on the position of the cursor
 */
void CodeEditor::highlightCurrentLine(){
    QList<QTextEdit::ExtraSelection> extraSelections;
    if (!isReadOnly()) {
        // specify the format of the selected line using ExtraSelection
        QTextEdit::ExtraSelection selection;
        QColor lineColor = QColor(Qt::blue).lighter(190);// CMYK

        selection.format.setBackground(lineColor);
        selection.format.setProperty(QTextFormat::FullWidthSelection, true);
        selection.cursor = textCursor();
        selection.cursor.clearSelection();
        extraSelections.append(selection);
    }

    setExtraSelections(extraSelections);
}


/*
 * Paint the number of active line
 * ----------------------------
 * In the left bar, show the line number that has been activated
 */
void CodeEditor::paintLineNumberArea(QPaintEvent *event){
    QPainter painter(lineNumberArea);
    painter.fillRect(event->rect(), lineColor);

    QTextBlock block = firstVisibleBlock();
    int blockNumber = block.blockNumber();
    int top = (int) blockBoundingGeometry(block).translated(contentOffset()).top();
    int bottom = top + (int) blockBoundingRect(block).height();

    // draw new line number
    while (block.isValid() && top <= event->rect().bottom()) {
        if (block.isVisible() && bottom >= event->rect().top()) {
            QString number = QString::number(blockNumber + 1);
            painter.setPen(Qt::lightGray);// the color of the stripe on the left, CMYK
            painter.drawText(0, top, lineNumberArea->width(), fontMetrics().height(),
                             Qt::AlignCenter, number);
        }

        // shift block
        block = block.next();
        top = bottom;
        bottom = top + (int) blockBoundingRect(block).height();
        ++blockNumber;
    }
}


/*
 * Keyboard operations
 * ----------------------------
 * Automatic completion of some symbols like "()", "{}" and "\\",
 * usages like while(), if(), for()
 * keyboard operations like up, down and enter.
 */
void CodeEditor::keyPressEvent(QKeyEvent *event){

  if(event->modifiers() == Qt::ShiftModifier && event->key() == 40){
      this->insertPlainText(tr("()"));
      this->moveCursor(QTextCursor::PreviousCharacter);

    }else if(event->modifiers() == Qt::ShiftModifier && event->key() == 34){
      this->insertPlainText(tr("\"\""));
      this->moveCursor(QTextCursor::PreviousCharacter);

    }else if(event->key() == 16777235 && completeState == CompleteState::Showing){
      if(completeWidget->currentRow() > 0)
        completeWidget->setCurrentRow(completeWidget->currentRow() - 1);

    }else if(event->key() == 16777237 && (completeState == CompleteState::Showing)){
      if(completeWidget->currentRow() < completeWidget->count() - 1)
        completeWidget->setCurrentRow(completeWidget->currentRow() + 1);

      // press enter for completelist
    }else if(event->key() == Qt::Key_Return && (completeState == CompleteState::Showing)){
      QString insertText = completeWidget->currentItem()->text();
      QString word = this->getWordOfCursor();

      completeState = CompleteState::Ignore;
      for(int i = 0; i < word.count(); ++i)
        this->textCursor().deletePreviousChar();
      this->insertPlainText(insertText);
      if(insertText.contains(tr("#include")))
        this->moveCursor(QTextCursor::PreviousCharacter);
      completeState = CompleteState::Hide;
      completeWidget->hide();

    }else if(event->key() == Qt::Key_Return){
      //get the text of the line
      QString temp = this->document()->findBlockByLineNumber(this->textCursor().blockNumber()).text();
      QPlainTextEdit::keyPressEvent(event);
      if(temp.count() <= 0)return;
      //output the leading space of the carriage return line

      foreach(const QChar &c, temp){
          if(c.isSpace())this->insertPlainText(c);
          else break;
        }

      //for() while() if()
      if(temp.at(temp.count() - 1)==')' && (temp.contains(tr("for("))||temp.contains(tr("while("))||temp.contains(tr("if("))))
          this->insertPlainText(tr("\t"));

      //If {, indent and complement}
      if(temp.at(temp.count() - 1)=='{'){
          this->insertPlainText(tr("\t"));
          QTextCursor cursor = this->textCursor();
          int pos = this->textCursor().position();
          this->insertPlainText(tr("\n"));
          foreach(const QChar &c, temp){
              if(c.isSpace())this->insertPlainText(c);
              else break;
            }
          this->insertPlainText(tr("}"));
          cursor.setPosition(pos);
          this->setTextCursor(cursor);//returns the middle row
        }

      // backspace
    }else if(event->key() == Qt::Key_Backspace){

      switch(this->document()->characterAt(this->textCursor().position() - 1).toLatin1()){
          // delete both '(' and ')' if it is '('+cursor+')'
        case '(':
          QPlainTextEdit::keyPressEvent(event);
          if(this->document()->characterAt(this->textCursor().position()) ==')'){
              this->textCursor().deleteChar();
            }break;

        default:
          QPlainTextEdit::keyPressEvent(event);
        }
    }else{
    QPlainTextEdit::keyPressEvent(event);
    }
}


/*
 * Set the list of complete key words
 * ----------------------------
 * Completion of words.
 */
void CodeEditor::setCompleteList(){
    completeList<< "char" << "int"
                <<"if"<<"for"<<"while"<<"else"<<"main";
                  //for other keywords, just add them to the list, "using","namespace"

}

/*
 * Gets the string of the current cursor position
 * ----------------------------
 * use a vector to collect its characters.
 */
QString CodeEditor::getWordOfCursor(){
    int pos = this->textCursor().position() -1 ;
    QVector<QChar> words;
    QString result;
    QChar ch = this->document()->characterAt(pos + 1);
    if(ch.isDigit()||ch.isLetter()||ch ==' ')return result;
    ch = this->document()->characterAt(pos);

    // add characters to get the current word
    while(ch.isDigit()||ch.isLetter()||ch =='_'||ch =='#'){
        words.append(ch);
        pos--;
        ch = this->document()->characterAt(pos);
    }
    for(int i= words.size() - 1; i >= 0; i--) result += words[i];
    return result;

}

/*
 * Give the complete widget of word options
 * ----------------------------
 * according to the current entering.
 */
void CodeEditor::showCompleteWidget(){
    //ignore the response of the cursor and text changes
    if(completeState == CompleteState::Ignore)return;
    completeWidget->hide();
    completeState = CompleteState::Hide;
    QString word = this->getWordOfCursor();
    completeWidget->clear();

    //Determine whether the word where the cursor is associative
    if(!word.isEmpty()){
        int maxSize = 0;
        QMap<QString,int> distance;
        vector<QString> itemList;
        foreach(const QString &temp, completeList){
            if(temp.contains(word)){
                itemList.push_back(temp);
                distance[temp] = CompleteListWidget::ldistance(temp.toStdString(), word.toStdString());
                if(temp.length() > maxSize)maxSize = temp.length();

            }
        }

      //If there is a matching character
      if(itemList.size() > 0){
      sort(itemList.begin(), itemList.end(),[&](const QString &s1, const QString &s2)->bool{return distance[s1]<distance[s2]; });
      foreach(const QString& item, itemList){
          completeWidget->addItem(new QListWidgetItem(item));
        }

      // set the widget
      int x = this->getCompleteWidgetX();
      int y = this->cursorRect().y()+fontMetrics().height();

      completeWidget->move(x, y);
      if(completeWidget->count() > 5)completeWidget->setFixedHeight(fontMetrics().height()*6);
      else completeWidget->setFixedHeight(fontMetrics().height()*(completeWidget->count()+1));
      completeWidget->setFixedWidth((fontMetrics().width(QLatin1Char('9')) + 6)*maxSize);
      completeWidget->show();
      completeState = CompleteState::Showing;
      completeWidget->setCurrentRow(0, QItemSelectionModel::Select);
        }
    }

}


/*
 * Get the width x of the completion widget
 * ----------------------------
 * according to the word length.
 */
int CodeEditor::getCompleteWidgetX(){
    QTextCursor cursor = this->textCursor();
    int pos = cursor.position() - 1;
    int origianlPos = pos + 1;
    QChar ch;
    ch = this->document()->characterAt(pos);

    // go to the word beginning
    while((ch.isDigit()||ch.isLetter()||ch=='_'||ch=='#') && pos > 0){
        pos--;
        ch = this->document()->characterAt(pos);
    }
    pos++;

    completeState = CompleteState::Ignore;
    cursor.setPosition(pos);
    this->setTextCursor(cursor);

    // use cursor position to get x
    int x = this->cursorRect().x()+2*fontMetrics().width(QLatin1Char('9'));
    // set the cursor back
    cursor.setPosition(origianlPos);
    this->setTextCursor(cursor);
    completeState = CompleteState::Hide;
    return x;
}
