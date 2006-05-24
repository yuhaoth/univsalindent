/****************************************************************************
**
** This file may be used under the terms of the GNU General Public
** License version 2.0 as published by the Free Software Foundation
** and appearing in the file LICENSE.GPL included in the packaging of
** this file.  
**
** This file is provided AS IS with NO WARRANTY OF ANY KIND, INCLUDING THE
** WARRANTY OF DESIGN, MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE.
**
****************************************************************************/

#ifndef MAINWINDOW_H
#define MAINWINDOW_H

//#include "arthurwidgets.h"

#include "ui_indentgui.h"
#include "cpphighlighter.h"
#include "gchandler.h"
#include <QWidget>
#include <QString>
#include <QScrollBar>

class MainWindow : public QMainWindow, private Ui::IndentGui
{
    Q_OBJECT

public:
    //! Constructor
    MainWindow(QWidget *parent = 0);

private:
    QString sourceFileContent;
    QString sourceFormattedContent;
    QString sourceViewContent;
	CppHighlighter *highlighter;
	QScrollBar *textEditVScrollBar;
	QScrollBar *textEdit2VScrollBar;
	int textEditLastScrollPos;

    GcHandler *gcHandler;

    //! Tries to load a file and returns its content as QString
    QString loadFile(QString filePath);
    
    QString openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr);
    
public slots:
    void openConfigFileDialog();
    void openSourceFileDialog();
    void callIndenter();
    void updateSourceView();
    void turnHighlightOnOff(bool turnOn);
};

#endif
