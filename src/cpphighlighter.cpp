/***************************************************************************
 *   Copyright (C) 2006-2007 by Thomas Schweitzer                          *
 *   thomas-schweitzer(at)arcor.de                                         *
 *                                                                         *
 *   This program is free software; you can redistribute it and/or modify  *
 *   it under the terms of the GNU General Public License version 2.0 as   *
 *   published by the Free Software Foundation.                            *
 *                                                                         *
 *   This program is distributed in the hope that it will be useful,       *
 *   but WITHOUT ANY WARRANTY; without even the implied warranty of        *
 *   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the         *
 *   GNU General Public License for more details.                          *
 *                                                                         *
 *   You should have received a copy of the GNU General Public License     *
 *   along with this program in the file LICENSE.GPL; if not, write to the *
 *   Free Software Foundation, Inc.,                                       *
 *   59 Temple Place - Suite 330, Boston, MA  02111-1307, USA.             *
 ***************************************************************************/

#include <QtGui>

#include "cpphighlighter.h"

/*!
    \class CppHighlighter
    \brief Highlighter used by QTextEdit fields to syntax highlight cpp source code
*/

/*!
    The constructor initializes some regular expressions and keywords to identify cpp tokens
 */
CppHighlighter::CppHighlighter(QsciScintilla *parent)
: QObject(parent)
{
    this->parent = parent;
    highlightningIsOn = true;
/*
    keywordFormat.setForeground(Qt::darkBlue);
    keywordFormat.setFontWeight(QFont::Bold);
    QStringList keywordPatterns;
    keywordPatterns << "\\bchar\\b" << "\\bclass\\b" << "\\bconst\\b"
        << "\\bdouble\\b" << "\\benum\\b" << "\\bexplicit\\b"
        << "\\bfriend\\b" << "\\binline\\b" << "\\bint\\b"
        << "\\blong\\b" << "\\bnamespace\\b" << "\\boperator\\b"
        << "\\bprivate\\b" << "\\bprotected\\b" << "\\bpublic\\b"
        << "\\bshort\\b" << "\\bsignals\\b" << "\\bsigned\\b"
        << "\\bslots\\b" << "\\bstatic\\b" << "\\bstruct\\b"
        << "\\btemplate\\b" << "\\btypedef\\b" << "\\btypename\\b"
        << "\\bunion\\b" << "\\bunsigned\\b" << "\\bvirtual\\b"
        << "\\bvoid\\b" << "\\bvolatile\\b" << "\\belse\\b"
        << "\\bif\\b" << "\\bwhile\\b";
    foreach (QString pattern, keywordPatterns) {
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

    quotationFormat.setForeground(Qt::darkRed);
    rule.pattern = QRegExp("\".*\"");
    rule.format = quotationFormat;
    highlightingRules.append(rule);

    //functionFormat.setFontItalic(true);
    functionFormat.setForeground(Qt::blue);
    rule.pattern = QRegExp("\\b[A-Za-z0-9_]+(?=\\()");
    rule.format = functionFormat;
    highlightingRules.append(rule);
*/
    //commentStartExpression = QRegExp("/\\*");
    //commentEndExpression = QRegExp("\\*/");

	lexer = new QsciLexerCPP();

	lexer->setDefaultFont( QFont("Courier", 10) );
	parent->setLexer(lexer);
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::CommentLine );
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::Comment );
	lexer->refreshProperties();

	
	lexer2 = new QsciLexerHTML();
	lexer2->setDefaultFont( QFont("Courier", 10) );
}

/*!
    Turns the syntax parser on.
*/
void CppHighlighter::turnHighlightOn() {
    highlightningIsOn = true;
	parent->setLexer(lexer);
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::CommentLine );
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::Comment );
}

/*!
    Turns the syntax parser off.
*/
void CppHighlighter::turnHighlightOff() {
    highlightningIsOn = false;
	parent->setLexer(lexer2);
}


/*!
    Read the settings for the current lexer from the settings file.
 */
bool CppHighlighter::readCurrentSettings(QSettings &qs, const char *prefix)
{
    bool ok, flag, rc = true;
    int num;
    QString key;

    // Read the styles.
    for (int i = 0; i < 128; ++i)
    {
        // Ignore invalid styles.
        if ( lexer->description(i).isEmpty() )
            continue;

        key.sprintf( "%s/%s/style%d/", prefix, lexer->language(), i );

        // Read the foreground colour.
        ok = qs.contains(key + "color");
        num = qs.value(key + "color", 0).toInt();

        if (ok)
            lexer->setColor( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;

        // Read the end-of-line fill.
        ok = qs.contains(key + "eolfill");
        flag = qs.value(key + "eolfill", false).toBool();

        if (ok)
            lexer->setEolFill( flag, i );
        else
            rc = false;

        // Read the font
        QStringList fdesc;

        ok = qs.contains(key + "font");
        fdesc = qs.value(key + "font").toStringList();

        if (ok && fdesc.count() == 5)
        {
            QFont f;

            f.setFamily(fdesc[0]);
            f.setPointSize(fdesc[1].toInt());
            f.setBold(fdesc[2].toInt());
            f.setItalic(fdesc[3].toInt());
            f.setUnderline(fdesc[4].toInt());

            lexer->setFont(f, i);
        }
        else
            rc = false;

        // Read the background colour.
        ok = qs.contains(key + "paper");
        num = qs.value(key + "paper", 0).toInt();

        if (ok)
            lexer->setPaper( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
        else
            rc = false;
    }

    // Read the properties.
    key.sprintf( "%s/%s/properties/", prefix, lexer->language() );

    lexer->refreshProperties();

    return rc;
}


/*!
    Write the settings for the current lexer to the settings file.
 */
void CppHighlighter::writeCurrentSettings(QSettings &qs, const char *prefix) {
    QString key;

    // Write the styles.
    for (int i = 0; i < 128; ++i) {
        // Ignore invalid styles.
        if ( lexer->description(i).isEmpty() )
            continue;

        int num;
        QColor c;

        key.sprintf( "%s/%s/style%d/", prefix, lexer->language(), i );

        // Write the foreground colour.
        c = lexer->color(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        qs.setValue(key + "color", num);

        // Write the end-of-line fill.
        qs.setValue( key + "eolfill", lexer->eolFill(i) );

        // Write the font
        QStringList fdesc;
        QString fmt("%1");
        QFont f;

        f = lexer->font(i);

        fdesc += f.family();
        fdesc += fmt.arg( f.pointSize() );

        // The casts are for Borland.
        fdesc += fmt.arg( (int)f.bold() );
        fdesc += fmt.arg( (int)f.italic() );
        fdesc += fmt.arg( (int)f.underline() );

        qs.setValue(key + "font", fdesc);

        // Write the background colour.
        c = lexer->paper(i);
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        qs.setValue(key + "paper", num);
    }
}
