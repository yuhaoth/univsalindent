/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
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

#include "mainwindow.h"
#include <QApplication>

#include "UiguiIndentServer.h"

#include "uiguiIniFileParser.h"
#include "uiguisettings.h"

/*!
    /brief Entry point to UniversalIndentGUI application.

    Evaluates the following parameters:
    No parameters starts without server and full gui.
    -f filename --file filename   : Opens the by filename defined file on start.
    -p --plugin  : Run as plugin. Server will be startet with a simplified gui.
    -s --server  : Run as server only without gui.
    If -p and -s are set, -p will be used.
 */
int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QString file2OpenOnStart = "";
    bool startAsPlugin = false;
    bool startAsServer = false;

    UiguiIndentServer server;
    MainWindow *mainWindow = NULL;
    IndentHandler *indentHandler = NULL;

    // Parse command line arguments. First parameter is the executable itself.
    for ( int i = 1; i < argc; i++ ) {
        QString currentArg = argv[i];
        // Open file parameters.
        if ( currentArg == "-f" || currentArg == "--file" ) {
            // Test whether a parameter follows the file parameter.
            if ( i + 1 >= argc ) {
                QMessageBox::information( NULL, "Need additional parameter", "The parameter -f / --file needs a following parameter defining the file to be opened at start." );
                fprintf(stderr, "The parameter -f / --file needs a following parameter defining the file to be opened at start.");
                exit(1);
            }
            // Set the file that shall be opened a start.
            else {
                i++;
                file2OpenOnStart = argv[i];
            }
        }
        else if ( currentArg == "-p" || currentArg == "--plugin" ) {
            startAsPlugin = true;
        }
        else if ( currentArg == "-s" || currentArg == "--server" ) {
            startAsServer = true;
        }
        else {
            QString message = "Invalid parameter found. Allowed parameters are:\n" \
                              "No parameters starts without server and full gui.\n" \
                              "-f filename --file filename   : Opens the by filename defined file on start.\n" \
                              "-p --plugin  : Run as plugin. Server will be startet with a simplified gui.\n" \
                              "-s --server  : Run as server only without gui.\n"
                              "If -p and -s are set, -p will be used.";
            QMessageBox::information( NULL, "Invalid parameter", message );
            fprintf(stderr, "Invalid parameter found. Allowed parameters are...");
            exit(1);
        }
    }


    // Set default values for all by UniversalIndentGUI used settings objects.
    QCoreApplication::setOrganizationName("UniversalIndentGUI");
    QCoreApplication::setOrganizationDomain("universalindent.sf.net");
    QCoreApplication::setApplicationName("UniversalIndentGUI");

    // Start normal with full gui and without server.
    if ( !startAsPlugin && !startAsServer ) {
        mainWindow = new MainWindow(file2OpenOnStart);
        mainWindow->show();
    }
    // Start as plugin with server.
    else if ( startAsPlugin ) {
        server.startServer();
        indentHandler = new IndentHandler(0);
        indentHandler->show();
    }
    // Start as server only without any gui.
    else if ( startAsServer ) {
        server.startServer();
    }

    int returnValue = app.exec();

    // Delete the correct objects.
    if ( !startAsPlugin && !startAsServer && mainWindow != NULL) {
        delete mainWindow;
    }
    else if ( startAsPlugin && indentHandler != NULL) {
        server.stopServer();
        delete indentHandler;
    }
    else if ( startAsServer ) {
        server.stopServer();
    }

    UiguiSettings::deleteInstance();

    return returnValue;
}
