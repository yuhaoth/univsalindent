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

	lexer = new QsciLexerCPP();

	parent->setLexer(lexer);
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::CommentLine );
	lexer->setFont( QFont("Courier", 10), QsciLexerCPP::Comment );
	//lexer->refreshProperties();
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
	parent->setLexer();
}


/*!
    Read the settings for the current lexer from the settings file.
 */
bool CppHighlighter::readCurrentSettings(QSettings &qs, const char *prefix)
{
    bool ok, flag, rc = true;
    int num;
    QString key;

    // Reset lists containing fonts and colors for each style
    fontForStyles.clear();
    colorForStyles.clear();

    // Read the styles.
    for (int i = 0; i < 128; ++i)
    {
        // Ignore invalid styles.
        if ( lexer->description(i).isEmpty() )
            continue;

        key.sprintf( "%s/%s/style%d/", prefix, lexer->language(), i );
		key.replace("+", "p");

        // Read the foreground colour.
        ok = qs.contains(key + "color");
        num = qs.value(key + "color", 0).toInt();

        if (ok)
            setColor( QColor((num >> 16) & 0xff, (num >> 8) & 0xff, num & 0xff), i );
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

            setFont(f, i);
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
		key.replace("+", "p");

		// Write style name
		qs.setValue( key + "", lexer->description(i) );

        // Write the foreground colour.
		if ( colorForStyles.contains(i) ) {
			c = colorForStyles[i];
		}
		else {
			c = lexer->color(i);
		}
        num = (c.red() << 16) | (c.green() << 8) | c.blue();

        qs.setValue(key + "color", num);

        // Write the end-of-line fill.
        qs.setValue( key + "eolfill", lexer->eolFill(i) );

        // Write the font
        QStringList fdesc;
        QString fmt("%1");
        QFont f;

		if ( fontForStyles.contains(i) ) {
			f = fontForStyles[i];
		}
		else {
			f = lexer->font(i);
		}

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


void CppHighlighter::setColor(const QColor &color, int style) {
    colorForStyles[style] = color;
    lexer->setColor( color, style );
}


void CppHighlighter::setFont(const QFont &font, int style) {
    fontForStyles[style] = font;
    lexer->setFont( font, style );
}


void CppHighlighter::setLexerForExtension( QString extension ) {
}