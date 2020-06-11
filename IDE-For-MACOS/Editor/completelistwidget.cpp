#include "completelistwidget.h"


// set color
CompleteListWidget::CompleteListWidget(QWidget *parent):QListWidget(parent){
    p = (QPlainTextEdit*)parent;
    backgroundColor = Qt::white;
    // color of the complete list, RGB
    highlightColor.setRgb(240,255,240); //Honeydew
    QPalette palette =  this->palette();

    palette.setColor(QPalette::Highlight,highlightColor);                       //set the color of highlight when selected
    palette.setColor(QPalette::Active, QPalette::Base, backgroundColor);
    palette.setColor(QPalette::Inactive, QPalette::Base, backgroundColor);
    palette.setColor(QPalette::Text, Qt::black);
    this->setPalette(palette);
}

// action settings for the keyboard
void CompleteListWidget::keyPressEvent(QKeyEvent *event){
    if(event->key() == 16777235||event->key() == 16777237){ // case up and down
        QListWidget::keyPressEvent(event);
    }else{                                                  // case left and right, and so on
        QApplication::sendEvent(p,event);
        p->setFocus();
    }
}

//compute the distance between source and target by matrix
int CompleteListWidget::ldistance(const string source, const string target){

    int n = source.length();
    int m = target.length();
    if (m == 0) return n;
    if (n == 0) return m;
    // Construct a (n)*(m) matrix with row and column value
    typedef vector< vector<int> >  Tmatrix;
    Tmatrix matrix(n + 1);
    for (int i = 0; i <= n; i++)  matrix[i].resize(m + 1);
    for (int i = 1; i <= n; i++) matrix[i][0] = i; //row number
    for (int j = 1; j <= m; j++) matrix[0][j] = j; //column number

    // distance accumulation (count distance by parts)
    for (int i = 1; i <= n; i++){
        const char si = source[i - 1]; // get the row-corresponding source value
        for (int j = 1; j <= m; j++){
            const char dj = target[j - 1]; // get the column-corresponding source value
            int cost;
            if (si == dj){
            cost = 0;
            }else{
                cost = 1;
            }
            // choose the shortest part distance
            const int above = matrix[i - 1][j] + 1;
            const int left = matrix[i][j - 1] + 1;
            const int diag = matrix[i - 1][j - 1] + cost;
            matrix[i][j] = min(above, min(left, diag));
        }
    }
    return matrix[n][m];

}
