#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent): QSyntaxHighlighter(parent){
    HighlightingRule rule; // pattern + format

    /* Set the key word format */
    keywordFormat.setForeground(QColor(233, 150, 122)); //DarkSalmon
    keywordFormat.setFontWeight(QFont::Bold); //Make the font bold

    QStringList keywordPatterns;
    keywordPatterns << "\\bwhile\\b" << "\\belse\\b"
                    <<"\\bif\\b"<<"\\bfor\\b"<<"\\bor\\b"<<"\\bnot\\b";

    //For each keyword group, add it to the list of changing colors
    foreach (const QString &pattern, keywordPatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    /* Set the variable format */
    variableFormat.setForeground(QColor(0,0,205)); //MediumBlue
    variableFormat.setFontWeight(QFont::Bold); //Make the font bold

    QStringList variablePatterns;
    variablePatterns << "\\bchar\\b" << "\\bint\\b" ;

    //For each variable group, add it to the list of changing colors
    foreach (const QString &pattern, variablePatterns) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = variableFormat;
        highlightingRules.append(rule);
    }


    /* Set "main" format */
    formatEspecillyForMain.setForeground(QColor(255,0,0));
    formatEspecillyForMain.setFontWeight(QFont::Bold);

    QStringList forMain;
    forMain << "\\bmain\\b";
    foreach (const QString &pattern, forMain) {
        rule.pattern = QRegularExpression(pattern);
        rule.format = formatEspecillyForMain;
        highlightingRules.append(rule);
    }


    /* Set the comment format */
    singleLineCommentFormat.setForeground(Qt::darkGreen); //CMYK
    rule.pattern = QRegularExpression("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);
    commentStartExpression = QRegularExpression("/\\*");
    commentEndExpression = QRegularExpression("\\*/");


    /* Set the function format */
    functionFormat.setFontItalic(true);
    functionFormat.setForeground(QColor(204,51,153));//RGB
    rule.pattern = QRegularExpression("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    /* Set the number format */
    numberFormat.setForeground(QColor(30,144,255)); //RGB, DodgerBlue
    rule.pattern = QRegularExpression("\\d");
    rule.format = numberFormat;
    highlightingRules.append(rule);

}



void Highlighter::highlightBlock(const QString &text){
    foreach (const HighlightingRule &rule, highlightingRules) {
        QRegularExpressionMatchIterator matchIterator = rule.pattern.globalMatch(text);
        while (matchIterator.hasNext()) {
            QRegularExpressionMatch match = matchIterator.next();
            setFormat(match.capturedStart(), match.capturedLength(), rule.format);
        }
    }
    setCurrentBlockState(0);


    //For format for comments
    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = text.indexOf(commentStartExpression);

    while (startIndex >= 0) {
        QRegularExpressionMatch match = commentEndExpression.match(text, startIndex);
        int endIndex = match.capturedStart();
        int commentLength = 0;
        if (endIndex == - 1) {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        } else {
            commentLength = endIndex - startIndex
                            + match.capturedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = text.indexOf(commentStartExpression, startIndex + commentLength);
    }
}

