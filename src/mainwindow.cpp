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

#include <QFileDialog>
#include <QMessageBox>
#include <QTextStream>

#include <QtGui/QCheckBox>

/*!
	\class MainWindow
	\brief Is the main window of UniversalIndentGUI

	The MainWindow class is responsible for generating and displaying most of the gui elements.
	Its look is set in the file "indentgui.ui". An object for the indent handler is generated here
	and user actions are being controlled. Is responsible for file open dialogs and indenter selection.
 */

/*!
	Constructs the main window.
 */
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //QFont font;
    //font.setFamily("Courier");
    //font.setFixedPitch(true);
    //font.setPointSize(10);

	// generate gui a it is build in the file "indentgui.ui"
    setupUi(this);

	// set the program version, which is shown in the main window title
    QString version = "0.2.1.2 Alpha revision 51";
    this->setWindowTitle( this->windowTitle() +"  "+ version );

    connect( pbOpenFile, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( actionOpen_Source_File, SIGNAL(activated()), this, SLOT(openSourceFileDialog()) );
    //connect( pbLoadIndentCfg, SIGNAL(clicked()), this, SLOT(openConfigFileDialog()) );
    connect( actionLoad_Indenter_Config_File, SIGNAL(activated()), this, SLOT(openConfigFileDialog()) );
    connect( cbLivePreview, SIGNAL(clicked()), this, SLOT(updateSourceView()) );
    connect( cbHighlight, SIGNAL(clicked(bool)), this, SLOT(turnHighlightOnOff(bool)) );
    connect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceViewChanged()) );

    sourceFileContent = loadFile("./data/example.cpp");

    textEditVScrollBar = txtedSourceCode->verticalScrollBar();
    textEdit2VScrollBar = txtedLineNumbers->verticalScrollBar();

    connect( textEditVScrollBar, SIGNAL(valueChanged(int)), textEdit2VScrollBar, SLOT(setValue(int)));
    connect( textEdit2VScrollBar, SIGNAL(valueChanged(int)), textEditVScrollBar, SLOT(setValue(int)));

    highlighter = new CppHighlighter(txtedSourceCode->document());

    gcHandler = 0;
    currentIndenterID = -1;

    // selects the first found indenter
    selectIndenter(0);

    connect( cmbBoxIndenters, SIGNAL(activated(int)), this, SLOT(selectIndenter(int)) );

    // generate about dialog box
    aboutDialog = new AboutDialog(this);
    connect( pbAbout, SIGNAL(clicked()), aboutDialog, SLOT(exec()) );
    connect( actionAbout_UniversalIndentGUI, SIGNAL(activated()), aboutDialog, SLOT(exec()) );
}


/*!
	Creates the by \a indenterID selected indent handler object and adds the indent widget to its layout
 */
void MainWindow::selectIndenter(int indenterID) {
	GcHandler *oldGcHandler = gcHandler;

    // prevent unnecessarry updates if same indenter as current has been selected
    if ( indenterID == currentIndenterID ) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

	if ( oldGcHandler != 0 ) {
		gcHandler = new GcHandler("./data/", indenterID, centralwidget);
        gcHandler->hide();
        vboxLayout1->insertWidget(0, gcHandler);
        oldGcHandler->hide();
        gcHandler->show();
		vboxLayout1->removeWidget(oldGcHandler);
		delete oldGcHandler;
        cmbBoxIndenters->clear();
	}
	else {
		gcHandler = new GcHandler("./data/", centralwidget);
        vboxLayout1->addWidget(gcHandler);
	}

	cmbBoxIndenters->addItems( gcHandler->getAvailableIndenters() );
	cmbBoxIndenters->setCurrentIndex(indenterID);
	sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
	QObject::connect(gcHandler, SIGNAL(settingsCodeChanged()), this, SLOT(callIndenter()));

    currentIndenterID = indenterID;
    updateSourceView();
    QApplication::restoreOverrideCursor();
}

/*!
	Tries to load the by \a filePath defined file and returns its content as QString.
	If the file could not be loaded a error dialog will be shown.
 */
QString MainWindow::loadFile(QString filePath) {

    QFile inSrcFile(filePath);
    QString fileContent = "";

    if ( !inSrcFile.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::warning(NULL, tr("Error opening file"), tr("Cannot read the file ")+"\""+filePath+"\"." );
    }
    else {
        QTextStream inSrcStrm(&inSrcFile);
        QApplication::setOverrideCursor(Qt::WaitCursor);
        fileContent = inSrcStrm.readAll();
        QApplication::restoreOverrideCursor();
        inSrcFile.close();
    }
    return fileContent;
}

/*!
	Calls the source file open dialog to load a source file for the formatting preview.
	If the file was successfully loaded the indenter will be called to generate the formatted source code.
 */
void MainWindow::openSourceFileDialog() {
	QString fileExtensions = gcHandler->getPossibleIndenterFileExtensions();
    QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );

    if (openedSourceFileContent != "") {
        sourceFileContent = openedSourceFileContent;
        sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
        updateSourceView();
        textEditLastScrollPos = 0;
        textEditVScrollBar->setValue( textEditLastScrollPos );
    }
}

/*!
	Shows a file open dialog to open an existing config file for the currently selected indenter.
	If the file was successfully opened the indent handler is called to load the settings and update itself.
 */
void MainWindow::openConfigFileDialog() {
    QString configFilePath; 

    configFilePath = QFileDialog::getOpenFileName( NULL, tr("Choose indenter config file"), "./", "(*.cfg *.ini *.txt)" );

    if (configFilePath != "") {
        gcHandler->loadConfigFile(configFilePath);
    }
}

/*!
	Shows a file open dialog with the title \a dialogHeaderStr starting in the directory \a startPath 
	and with a file mask defined by \a fileMaskStr. Returns the contents of the file as QString.
 */
QString MainWindow::openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr) {

    QString fileContent = "";

    QString fileName = QFileDialog::getOpenFileName( NULL, dialogHeaderStr, startPath, fileMaskStr);

    if (fileName != "") {
        fileContent = loadFile(fileName);
    }

    return fileContent;
}

/*!
	Updates the text edit field, which is showing the loaded, and if preview is enabled formatted, source code.
	Reassigns the line numbers and in case of switch between preview and none preview keeps the text field
	at the same line number.
 */
void MainWindow::updateSourceView()
{
    QString lineNumbers = "";
    int i;
    int numberOfLines = 0;

    QTextCursor savedCursor = txtedSourceCode->textCursor();
    int cursorPos = savedCursor.position();

    textEditLastScrollPos = textEditVScrollBar->value();

    if ( cbLivePreview->isChecked() ) {
        sourceViewContent = sourceFormattedContent;
    }
    else {
        sourceViewContent = sourceFileContent;
    }

    disconnect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceViewChanged()) );
// because under linux the courier font is always set bold
#if defined(Q_OS_LINUX)
    txtedSourceCode->setFontFamily("freemono");
    txtedLineNumbers->setFontFamily("freemono");
#endif

    txtedSourceCode->setPlainText(sourceViewContent);
    connect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceViewChanged()) );

    numberOfLines = sourceViewContent.count(QRegExp("\n"));
    for (i = 1; i <= numberOfLines+1; i++) {
        lineNumbers.append(QString::number(i)+"\n");
    }
    txtedLineNumbers->setPlainText(lineNumbers);
    txtedLineNumbers->setAlignment(Qt::AlignRight);

    textEditVScrollBar->setValue( textEditLastScrollPos );
    savedCursor = txtedSourceCode->textCursor();
    if ( cursorPos >= txtedSourceCode->toPlainText().count() ) {
        cursorPos = txtedSourceCode->toPlainText().count() - 1;
    }
    savedCursor.setPosition( cursorPos );
    txtedSourceCode->setTextCursor( savedCursor );
}

/*!
	Calls the selected indenter with the currently loaded source code to retrieve the formatted source code.
	The original loaded source code file will not be changed.
 */
void MainWindow::callIndenter() {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
    updateSourceView();
    QApplication::restoreOverrideCursor();
}

/*!
	Switches the syntax highlightning corresponding to the value \a turnOn either on or off.
 */
void MainWindow::turnHighlightOnOff(bool turnOn) {
    if ( turnOn ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
    updateSourceView();
}

/*!
    Is emitted whenever the text inside the source view window changes. Calls the indenter
    to format the changed source code.
 */
void MainWindow::sourceViewChanged() {
    //if ( !cbLivePreview->isChecked() ) {
        sourceFileContent = txtedSourceCode->toPlainText();
        QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
        sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
        updateSourceView();
        QApplication::restoreOverrideCursor();
    //}
}
