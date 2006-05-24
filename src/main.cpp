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

#include "mainwindow.h"
#include <QApplication>

//#include <iostream>

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    MainWindow ui;
/*
    // set another style for the window and its widgets
    QStyle *arthurStyle = new ArthurStyle();
    //ui.setStyle(arthurStyle);
    QList<QWidget *> widgets = qFindChildren<QWidget *>(&ui);
    foreach (QWidget *w, widgets) {
        if ( qobject_cast<QPushButton *>(w) ) {
            w->setStyle(arthurStyle);
        }
    }
*/
    ui.show();
	
    return app.exec();
}
