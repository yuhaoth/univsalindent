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

//! Constructor
MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent)
{
    //QFont font;
    //font.setFamily("Courier");
    //font.setFixedPitch(true);
    //font.setPointSize(10);

    setupUi(this);

    connect( pbOpenFile, SIGNAL(clicked()), this, SLOT(openSourceFileDialog()) );
    connect( pbLoadIndentCfg, SIGNAL(clicked()), this, SLOT(openConfigFileDialog()) );
    connect( cbLivePreview, SIGNAL(clicked()), this, SLOT(updateSourceView()) );
    connect( cbHighlight, SIGNAL(clicked(bool)), this, SLOT(turnHighlightOnOff(bool)) );

    sourceFileContent = loadFile("./data/example.cpp");

    gcHandler = new GcHandler(centralwidget);
    vboxLayout1->addWidget(gcHandler);
    sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
    QObject::connect(gcHandler, SIGNAL(settingsCodeChanged()), this, SLOT(callIndenter()));


    textEditVScrollBar = txtedSourceCode->verticalScrollBar();
    textEdit2VScrollBar = txtedLineNumbers->verticalScrollBar();

    connect( textEditVScrollBar, SIGNAL(valueChanged(int)), textEdit2VScrollBar, SLOT(setValue(int)));
    connect( textEdit2VScrollBar, SIGNAL(valueChanged(int)), textEditVScrollBar, SLOT(setValue(int)));

    highlighter = new CppHighlighter(txtedSourceCode->document());

    updateSourceView();

}


//! Tries to load a file and returns its content as QString
QString MainWindow::loadFile(QString filePath) {

    QFile inSrcFile(filePath);
    QString fileContent = "";

    if ( !inSrcFile.open(QFile::ReadOnly | QFile::Text) ) {
        QMessageBox::warning(NULL, tr("Error open file"), tr("Cannot read file \"%s\".").arg(filePath));
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

void MainWindow::openSourceFileDialog() {
    sourceFileContent = openFileDialog( tr("Choose source code file"), "./", "(*.c *.cpp *.h *.hpp)" );

    if (sourceFileContent != "") {
        sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
        updateSourceView();
        textEditLastScrollPos = 0;
        textEditVScrollBar->setValue( textEditLastScrollPos );
    }
}

void MainWindow::openConfigFileDialog() {
    QString configFilePath; 

    configFilePath = QFileDialog::getOpenFileName( NULL, tr("Choose indenter config file"), "./", "(*.cfg *.ini *.txt)" );

    if (configFilePath != "") {
        gcHandler->loadConfigFile(configFilePath);
    }
}

QString MainWindow::openFileDialog(QString dialogHeaderStr, QString startPath, QString fileMaskStr) {

    QString fileContent = "";

    QString fileName = QFileDialog::getOpenFileName( NULL, dialogHeaderStr, startPath, fileMaskStr);

    if (fileName != "") {
        fileContent = loadFile(fileName);
    }

    return fileContent;
}


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

    //txtedSourceCode->setLineWrapMode( QTextEdit::NoWrap );
    //txtedSourceCode->setFontFamily("Courier");
    //txtedSourceCode->setFontWeight(QFont::Black);
    txtedSourceCode->setPlainText(sourceViewContent);

    numberOfLines = sourceViewContent.count(QRegExp("\n"));
    for (i = 1; i <= numberOfLines+1; i++) {
        lineNumbers.append(QString::number(i)+"\n");
    }
    txtedLineNumbers->setPlainText(lineNumbers);
    txtedLineNumbers->setAlignment(Qt::AlignRight);

    textEditVScrollBar->setValue( textEditLastScrollPos );
}

void MainWindow::callIndenter() {
    sourceFormattedContent = gcHandler->callGreatCode(sourceFileContent);
    updateSourceView();
}

void MainWindow::turnHighlightOnOff(bool turnOn) {
    if ( turnOn ) {
        highlighter->turnHighlightOn();
    }
    else {
        highlighter->turnHighlightOff();
    }
    updateSourceView();
}
