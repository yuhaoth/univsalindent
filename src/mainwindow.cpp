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
    version = "UniversalIndentGUI 0.2.5 Alpha";

    connect( pbOpenFile, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( actionOpen_Source_File, SIGNAL(activated()), this, SLOT(openSourceFileDialog()) );
    //connect( pbLoadIndentCfg, SIGNAL(clicked()), this, SLOT(openConfigFileDialog()) );
    connect( actionLoad_Indenter_Config_File, SIGNAL(activated()), this, SLOT(openConfigFileDialog()) );
    connect( actionSave_Source_File_As, SIGNAL(activated()), this, SLOT(saveasSourceFileDialog()) );
    connect( actionSave_Source_File, SIGNAL(activated()), this, SLOT(saveSourceFile()) );
    connect( actionSave_Indenter_Config_File, SIGNAL(activated()), this, SLOT(saveasIndentCfgFileDialog()) );
    connect( actionExportPDF, SIGNAL(activated()), this, SLOT(exportToPDF()) );
    connect( actionExportHTML, SIGNAL(activated()), this, SLOT(exportToHTML()) );
    connect( cbHighlight, SIGNAL(clicked(bool)), this, SLOT(turnHighlightOnOff(bool)) );

    currentSourceFile = "./data/example.cpp";
    QFileInfo fileInfo(currentSourceFile);
    currentSourceFile = fileInfo.absoluteFilePath();
    sourceFileContent = loadFile(currentSourceFile);
    currentSourceFileExtension = fileInfo.suffix();

    updateWindowTitle();

    textEditVScrollBar = txtedSourceCode->verticalScrollBar();
    textEdit2VScrollBar = txtedLineNumbers->verticalScrollBar();

    connect( textEditVScrollBar, SIGNAL(valueChanged(int)), textEdit2VScrollBar, SLOT(setValue(int)));
    connect( textEdit2VScrollBar, SIGNAL(valueChanged(int)), textEditVScrollBar, SLOT(setValue(int)));

    connect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedSlot()) );
    connect( cbLivePreview, SIGNAL(clicked(bool)), this, SLOT(previewTurnedOnOff(bool)) );

    highlighter = new CppHighlighter(txtedSourceCode->document());

    indentHandler = 0;
    currentIndenterID = -1;
    sourceCodeChanged = false;
    scrollPositionChanged = false;
    indentSettingsChanged = false;
    previewToggled = true;

    // selects the first found indenter
    selectIndenter(0);

    connect( cmbBoxIndenters, SIGNAL(activated(int)), this, SLOT(selectIndenter(int)) );

    // generate about dialog box
    aboutDialog = new AboutDialog(this);
    connect( pbAbout, SIGNAL(clicked()), aboutDialog, SLOT(exec()) );
    connect( actionAbout_UniversalIndentGUI, SIGNAL(activated()), aboutDialog, SLOT(exec()) );

    //QAction *actionAStyle;
    //QMenu *menuSelect_Indenter;
    //menuSelect_Indenter = new QMenu(menuIndenter);
    //menuSelect_Indenter->setObjectName(QString::fromUtf8("menuSelect_Indenter"));
    //menuIndenter->addAction(menuSelect_indenter->menuAction());
    //actionAStyle = new QAction(this);
    //actionAStyle->setObjectName(QString::fromUtf8("actionAStyle"));
    //menuSelect_Indenter->addAction(actionAStyle);
    //retranslateUi(this);
}


/*!
	Creates the by \a indenterID selected indent handler object and adds the indent widget to its layout
 */
void MainWindow::selectIndenter(int indenterID) {
	IndentHandler *oldIndentHandler = indentHandler;

    // prevent unnecessarry updates if same indenter as current has been selected
    if ( indenterID == currentIndenterID ) {
        return;
    }

    QApplication::setOverrideCursor(Qt::WaitCursor);

	if ( oldIndentHandler != 0 ) {
		indentHandler = new IndentHandler("./data/", indenterID, centralwidget);
        indentHandler->hide();
        vboxLayout1->insertWidget(0, indentHandler);
        oldIndentHandler->hide();
        indentHandler->show();
		vboxLayout1->removeWidget(oldIndentHandler);
		delete oldIndentHandler;
        cmbBoxIndenters->clear();
	}
	else {
		indentHandler = new IndentHandler("./data/", centralwidget);
        vboxLayout1->addWidget(indentHandler);
	}

	cmbBoxIndenters->addItems( indentHandler->getAvailableIndenters() );
	cmbBoxIndenters->setCurrentIndex(indenterID);
	QObject::connect(indentHandler, SIGNAL(settingsCodeChanged()), this, SLOT(indentSettingsChangedSlot()));

    currentIndenterID = indenterID;
    if ( cbLivePreview->isChecked() ) {
        callIndenter();
    }
    previewToggled = true;
    indentSettingsChanged = true;
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
    QString openedSourceFileContent = "";
	QString fileExtensions = "Supported by indenter ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;All files (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getOpenFileName( this, tr("Choose source code file"), currentSourceFile, fileExtensions);

    if (fileName != "") {
        currentSourceFile = fileName;
        QFileInfo fileInfo(fileName);
        currentSourceFileExtension = fileInfo.suffix();

        openedSourceFileContent = loadFile(fileName);
        sourceFileContent = openedSourceFileContent;
        if ( cbLivePreview->isChecked() ) {
            callIndenter();
        }
        sourceCodeChanged = true;
        previewToggled = true;
        updateSourceView();
        updateWindowTitle();
        textEditLastScrollPos = 0;
        textEditVScrollBar->setValue( textEditLastScrollPos );
    }
}


/*!
	Calls the source file save as dialog to save a source file under a choosen name.
	If the file already exists and it should be overwritten, a warning is shown before.
 */
void MainWindow::saveasSourceFileDialog() {
	QString fileExtensions = "Supported by indenter ("+indentHandler->getPossibleIndenterFileExtensions()+
                             ");;All files (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save source code file"), currentSourceFile, fileExtensions);

    if (fileName != "") {
        currentSourceFile = fileName;
        QFile::remove(fileName);
        QFile outSrcFile(fileName);
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        outSrcFile.write( txtedSourceCode->toPlainText().toAscii() );
        outSrcFile.close();

        QFileInfo fileInfo(fileName);
        currentSourceFileExtension = fileInfo.suffix();
        updateWindowTitle();
    }
}


/*!
    Saves the currently shown source code to the last save or opened source file.
    If no source file has been opened, because only the static example has been loaded,
    the save as file dialog will be shown.
 */
void MainWindow::saveSourceFile() {
    if ( currentSourceFile.isEmpty() ) {
        saveasSourceFileDialog();
    }
    else {
        QFile::remove(currentSourceFile);
        QFile outSrcFile(currentSourceFile);
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        outSrcFile.write( txtedSourceCode->toPlainText().toAscii() );
        outSrcFile.close();
    }
}


/*!
	Calls the indenter config file save as dialog to save the config file under a choosen name.
	If the file already exists and it should be overwritten, a warning is shown before.
 */
void MainWindow::saveasIndentCfgFileDialog() {
	QString fileExtensions = "All files (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save indent config file"), indentHandler->getIndenterCfgFile(), fileExtensions);

    if (fileName != "") {
        QFile::remove(fileName);
        QFile outCfgFile(fileName);
        outCfgFile.open( QFile::ReadWrite | QFile::Text );
        outCfgFile.write( indentHandler->getParameterString().toAscii() );
        outCfgFile.close();
    }
}


/*!
	Shows a file open dialog to open an existing config file for the currently selected indenter.
	If the file was successfully opened the indent handler is called to load the settings and update itself.
 */
void MainWindow::openConfigFileDialog() {
    QString configFilePath; 

    configFilePath = QFileDialog::getOpenFileName( NULL, tr("Choose indenter config file"), indentHandler->getIndenterCfgFile(), "All files (*.*)" );

    if (configFilePath != "") {
        indentHandler->loadConfigFile(configFilePath);
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

    textEditLastScrollPos = textEditVScrollBar->value();


    if ( cbLivePreview->isChecked() ) {
        sourceViewContent = sourceFormattedContent;
    }
    else {
        sourceViewContent = sourceFileContent;
    }

    if (previewToggled) {
        disconnect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedSlot()) );
        // because under linux the courier font is always set bold
#if defined(Q_OS_LINUX)
        txtedSourceCode->setFontFamily("freemono");
        txtedLineNumbers->setFontFamily("freemono");
#endif

        txtedSourceCode->setPlainText(sourceViewContent);
        connect( txtedSourceCode, SIGNAL(textChanged ()), this, SLOT(sourceCodeChangedSlot()) );
        previewToggled = false;
    }

    numberOfLines = sourceViewContent.count(QRegExp("\n"));
    for (i = 1; i <= numberOfLines+1; i++) {
        lineNumbers.append(QString::number(i)+"\n");
    }
    txtedLineNumbers->setPlainText(lineNumbers);
    txtedLineNumbers->setAlignment(Qt::AlignRight);

    textEditVScrollBar->setValue( textEditLastScrollPos );
}

/*!
	Calls the selected indenter with the currently loaded source code to retrieve the formatted source code.
	The original loaded source code file will not be changed.
 */
void MainWindow::callIndenter() {
    QApplication::setOverrideCursor(QCursor(Qt::WaitCursor));
    sourceFormattedContent = indentHandler->callIndenter(sourceFileContent, currentSourceFileExtension);
    //updateSourceView();
    QApplication::restoreOverrideCursor();
}

/*!
	Switches the syntax highlighting corresponding to the value \a turnOn either on or off.
 */
void MainWindow::turnHighlightOnOff(bool turnOn) {
    if ( turnOn ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
    previewToggled = true;
    updateSourceView();
}

/*!
    Is emitted whenever the text inside the source view window changes. Calls the indenter
    to format the changed source code.
 */
void MainWindow::sourceCodeChangedSlot() {
    QChar enteredCharacter;

    sourceCodeChanged = true;
    if ( scrollPositionChanged ) {
        scrollPositionChanged = false;
    }

    QTextCursor savedCursor = txtedSourceCode->textCursor();
    int cursorPos = savedCursor.position();

    sourceFileContent = txtedSourceCode->toPlainText();

    if ( sourceFileContent.count() == 0 || sourceFileContent.at(sourceFileContent.count()-1) != '\n' ) {
        sourceFileContent += "\n";
    }

    if ( cursorPos <= 0 ) {
        cursorPos = 1;
    } 
    enteredCharacter = sourceFileContent.at(cursorPos-1);

    if ( cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;
    }
    updateSourceView();

    QString text = txtedSourceCode->toPlainText();
    int lineBreakCounter = 0;
    while ( cursorPos <= text.count() && text.at(cursorPos-1) != enteredCharacter && lineBreakCounter < 5 ) {
        if ( text.at(cursorPos-1) == '\n' ) {
            lineBreakCounter++;
        }
        cursorPos++;
    }

    savedCursor = txtedSourceCode->textCursor();
    if ( cursorPos > txtedSourceCode->toPlainText().count() ) {
        cursorPos = txtedSourceCode->toPlainText().count() - 1;
    }
    savedCursor.setPosition( cursorPos );
    txtedSourceCode->setTextCursor( savedCursor );

    if ( cbLivePreview->isChecked() ) {
        sourceCodeChanged = false;
    }
}



void MainWindow::indentSettingsChangedSlot() {
    indentSettingsChanged = true;

    QTextCursor savedCursor = txtedSourceCode->textCursor();
    int cursorPos = savedCursor.position();
    
    if ( cbLivePreview->isChecked() ) {
        callIndenter();
        previewToggled = true;

        updateSourceView();
        if (sourceCodeChanged) {
            savedCursor = txtedSourceCode->textCursor();
            if ( cursorPos >= txtedSourceCode->toPlainText().count() ) {
                cursorPos = txtedSourceCode->toPlainText().count() - 1;
            }
            savedCursor.setPosition( cursorPos );
            txtedSourceCode->setTextCursor( savedCursor );

            sourceCodeChanged = false;
        }
        indentSettingsChanged = false;
    }
    else {
        updateSourceView();
    }
}

void MainWindow::previewTurnedOnOff(bool turnOn) {
    previewToggled = true;
    QTextCursor savedCursor = txtedSourceCode->textCursor();
    int cursorPos = savedCursor.position();

    if ( turnOn && (indentSettingsChanged || sourceCodeChanged) ) {
        callIndenter();
    }
    updateSourceView();
    if (sourceCodeChanged) {
        savedCursor = txtedSourceCode->textCursor();
        if ( cursorPos >= txtedSourceCode->toPlainText().count() ) {
            cursorPos = txtedSourceCode->toPlainText().count() - 1;
        }
        savedCursor.setPosition( cursorPos );
        txtedSourceCode->setTextCursor( savedCursor );

        sourceCodeChanged = false;
    }
    indentSettingsChanged = false;
}


void MainWindow::updateWindowTitle() {
    this->setWindowTitle( version +" "+ currentSourceFile);
}


void MainWindow::exportToPDF() {
	QString fileExtensions = "PDF Document (*.pdf)";

    QString fileName = currentSourceFile;
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix();

    fileName.replace( fileName.length()-fileExtension.length(), fileExtension.length(), "pdf" );
    fileName = QFileDialog::getSaveFileName( this, tr("Export source code file"), fileName, fileExtensions);

    if ( !fileName.isEmpty() ) {
        QPrinter printer(QPrinter::HighResolution);
        printer.setOutputFormat(QPrinter::PdfFormat);
        printer.setOutputFileName(fileName);

        txtedSourceCode->document()->print(&printer);
    }
}


void MainWindow::exportToHTML() {
	QString fileExtensions = "HTML Document (*.html)";

    QString fileName = currentSourceFile;
    QFileInfo fileInfo(fileName);
    QString fileExtension = fileInfo.suffix();

    fileName.replace( fileName.length()-fileExtension.length(), fileExtension.length(), "html" );
    fileName = QFileDialog::getSaveFileName( this, tr("Export source code file"), fileName, fileExtensions);

    if ( !fileName.isEmpty() ) {
        QFile::remove(fileName);
        QFile outSrcFile(fileName);
        outSrcFile.open( QFile::ReadWrite | QFile::Text );
        outSrcFile.write( txtedSourceCode->toHtml().toAscii() );
        outSrcFile.close();
    }
}