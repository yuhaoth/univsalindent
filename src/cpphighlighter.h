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

#ifndef CPPHIGHLIGHTER_H
#define CPPHIGHLIGHTER_H

#include <QObject>
#include <QMap>
#include <Qsci/qscilexer.h>
#include <Qsci/qscilexercpp.h>
#include <Qsci/qscilexerhtml.h>
#include <Qsci/qsciscintilla.h>

class CppHighlighter : public QObject
{
    Q_OBJECT

public:
    CppHighlighter(QsciScintilla *parent);
    void turnHighlightOff();
    void turnHighlightOn();

private:
    bool readCurrentSettings(QSettings &qs,const char *prefix);
    void writeCurrentSettings(QSettings &qs,const char *prefix);

    bool highlightningIsOn;
    QsciScintilla *parent;
    QMap<int, QFont> fontForStyles;
    QMap<int, QColor> colorForStyles;
	QsciLexer* lexer;
	QsciLexer* lexer2;
public slots:
    //! The foreground colour for style number \a style is set to \a color.  If
    //! \a style is -1 then the colour is set for all styles.
    void setColor(const QColor &color, int style = -1);

    //! The font for style number \a style is set to \a font.  If \a style is
    //! -1 then the font is set for all styles.
    void setFont(const QFont &font, int style = -1);
};

#endif
