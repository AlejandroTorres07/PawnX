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
                    << "\\bexit\\b" << "\\for\\b" << "\\bgoto\\b"
                    << "\\bif\\b" << "\\breturn\\b" << "\\bsleep\\b"
                    << "\\bstate\\b" << "\\bswitch\\b" << "\\bwhile\\b";

    foreach (const QString &pattern, keywordPatterns)
    {
        rule.pattern = QRegExp(pattern);
        rule.format = keywordFormat;
        highlightingRules.append(rule);
    }

    classFormat.setFontWeight(QFont::Bold);
    classFormat.setForeground(Qt::darkMagenta);
    rule.pattern = QRegExp("\\bQ[A-Za-z]+\\b");
    rule.format = classFormat;
    highlightingRules.append(rule);

    singleLineCommentFormat.setForeground(Qt::darkGreen);
    rule.pattern = QRegExp("//[^\n]*");
    rule.format = singleLineCommentFormat;
    highlightingRules.append(rule);

    multiLineCommentFormat.setForeground(Qt::darkGreen);

    doubleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\".*\"");
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    doubleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("<.*>");
    rule.format = doubleQuotationFormat;
    highlightingRules.append(rule);

    singleQuotationFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\'.*\'");
    rule.format = singleQuotationFormat;
    highlightingRules.append(rule);

    functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
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
