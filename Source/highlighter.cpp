#include "highlighter.h"

Highlighter::Highlighter(QTextDocument *parent)
    : QSyntaxHighlighter(parent)
{
    HighlightingRule rule;

    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bconst\\b" << "\\benum\\b" << "\\bforward\\b"
                    << "\\bnative\\b" << "\\bnew\\b" << "\\boperator\\b"
                    << "\\bpublic\\b" << "\\bstatic\\b" << "\\bstock\\b"
                    << "\\bassert\\b" << "\\bbreak\\b" << "\\bcase\\b"
                    << "\\bcontinue\\b" << "\\bdefault\\b" << "\\belse\\b"
                    << "\\bexit\\b" << "\\bfor\\b" << "\\bgoto\\b"
                    << "\\bif\\b" << "\\breturn\\b" << "\\bsleep\\b"
                    << "\\bstate\\b" << "\\bswitch\\b" << "\\bwhile\\b"
                    << "\\btrue\\b" << "\\bfalse\\b" << "\\bnull\\b"
                    << "\\bbool\\b" << "\\bFloat\\b" << "\\bdefine\\b"
                    << "\\belseif\\b" << "\\bemit\\b" << "\\bendif\\b"
                    << "\\bendinput\\b" << "\\bendscript\\b" << "\\berror\\b"
                    << "\\binclude\\b" << "\\bpragma\\b" << "\\btryinclude\\b"
                    << "\\bundef\\b" << "\\bifdef\\b" << "\\bifndef\\b"
                    << "\\bdefined\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);

    rule.pattern = QRegExp("\\b[0-9]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\".*\"");
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    singleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\'.*\'");
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("<.*>");
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);



    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::darkCyan);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);

    commentStartExpression = QRegExp("/\\*");
    commentEndExpression = QRegExp("\\*/");
}

void Highlighter::highlightBlock(const QString &text)
{
    foreach (const HighlightingRule &rule, highlightingRules)
    {
        QRegExp expression(rule.pattern);
        int index = expression.indexIn(text);
        while (index >= 0)
        {
            int length = expression.matchedLength();
            setFormat(index, length, rule.format);
            index = expression.indexIn(text, index + length);
        }
    }
    setCurrentBlockState(0);

    int startIndex = 0;
    if (previousBlockState() != 1)
        startIndex = commentStartExpression.indexIn(text);

    while (startIndex >= 0)
    {
        int endIndex = commentEndExpression.indexIn(text, startIndex);
        int commentLength;
        if (endIndex == -1)
        {
            setCurrentBlockState(1);
            commentLength = text.length() - startIndex;
        }
        else
        {
            commentLength = endIndex - startIndex
                            + commentEndExpression.matchedLength();
        }
        setFormat(startIndex, commentLength, multiLineCommentFormat);
        startIndex = commentStartExpression.indexIn(text, startIndex + commentLength);
    }
}
