/***************************************************************************
 *   Copyright (C) 2006-2010 by Thomas Schweitzer                          *
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

#include "IndentHandler.h"


#include <QtDebug>

#include "UiGuiSettings.h"

#ifdef Q_OS_WIN32
#include <Windows.h>
#endif

//! \defgroup grp_Indenter All concerning handling of the indenter.

/*!
    \class IndentHandler
    \ingroup grp_Indenter
    \brief A widget for handling many indenters that are configured by an ini file.

    This is a widget that is used by the main window. It handles access to the
    indenter config file and calls the chosen indenter to reformat the source text.
    Calls the indenter each time a setting has been changed and informs
    the main window about the reformatted source code.
*/


/*!
    \brief Constructor of the indent handler.

    By calling this constructor the indenter to be loaded, can be selected by setting
    its \a indenterID, which is the number of found indenter ini files in alphabetic
    order starting at index 0.
 */
IndentHandler::IndentHandler(int indenterID, QWidget *mainWindow, QWidget *parent) : QWidget(parent) {
    Q_ASSERT_X( indenterID >= 0, "IndentHandler", "the selected indenterID is < 0" );

    setObjectName(QString::fromUtf8("indentHandler"));

    this->mainWindow = mainWindow;

    parameterChangedCallback = NULL;
    windowClosedCallback = NULL;
    indenterSettings = NULL;
    menuIndenter = NULL;
    actionLoad_Indenter_Config_File = NULL;
    actionSave_Indenter_Config_File = NULL;
    actionCreateShellScript = NULL;
    initIndenterMenu();

    connect( actionLoad_Indenter_Config_File, SIGNAL(triggered()), this, SLOT(openConfigFileDialog()) );
    connect( actionSave_Indenter_Config_File, SIGNAL(triggered()), this, SLOT(saveasIndentCfgFileDialog()) );
    connect( actionCreateShellScript, SIGNAL(triggered()), this, SLOT(createIndenterCallShellScript()) );
    connect( actionResetIndenterParameters, SIGNAL(triggered()), this, SLOT(resetIndenterParameter()) );

    // define this widgets size and resize behavior
    //this->setMaximumWidth(263);
    this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // create vertical layout box, into which the toolbox will be added
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setMargin(2);

    // Create horizontal layout for indenter selector and help button.
    QHBoxLayout *hboxLayout = new QHBoxLayout();
    //hboxLayout->setMargin(2);
    vboxLayout->addLayout( hboxLayout );

    // Create the indenter selection combo box.
    indenterSelectionCombobox = new QComboBox(this);
    indenterSelectionCombobox->setObjectName(QString::fromUtf8("indenterSelectionCombobox"));
    indenterSelectionCombobox->setProperty("connectedSettingName", "DONOTTRANSLATE:SelectedIndenter");
    indenterSelectionCombobox->setSizeAdjustPolicy(QComboBox::AdjustToMinimumContentsLengthWithIcon);
    indenterSelectionCombobox->setMinimumContentsLength(20);
    connect( indenterSelectionCombobox, SIGNAL(activated(int)), UiGuiSettings::getInstance(), SLOT(handleValueChangeFromExtern(int)) );
    connect( UiGuiSettings::getInstance(), SIGNAL(selectedIndenter(int)), this, SLOT(setIndenter(int)) );
    hboxLayout->addWidget( indenterSelectionCombobox );

    // Create the indenter parameter help button.
    indenterParameterHelpButton = new QToolButton(this);
    indenterParameterHelpButton->setObjectName(QString::fromUtf8("indenterParameterHelpButton"));
    indenterParameterHelpButton->setIcon(QIcon(QString::fromUtf8(":/mainWindow/help.png")));
    hboxLayout->addWidget( indenterParameterHelpButton );
    // Handle if the indenter parameter help button is pressed.
    connect( indenterParameterHelpButton, SIGNAL(clicked()), this, SLOT(showIndenterManual()) );

    // create a toolbox and set its resize behavior
    toolBox = new QToolBox(this);
    toolBox->setObjectName(QString::fromUtf8("toolBox"));

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    connect( toolBox, SIGNAL(currentChanged(int)), this, SLOT(updateDrawing()) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS

    //toolBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    //toolBox->setMaximumSize(QSize(16777215, 16777215));
    // insert the toolbox into the vlayout
    vboxLayout->addWidget(toolBox);

    indenterExecutableCallString = "";
    indenterExecutableSuffix = "";

    indenterDirctoryStr = SettingsPaths::getIndenterPath();
    tempDirctoryStr = SettingsPaths::getTempPath();
    settingsDirctoryStr = SettingsPaths::getSettingsPath();
    QDir indenterDirctory = QDir(indenterDirctoryStr);

    if ( mainWindow != NULL ) {
        errorMessageDialog = new UiGuiErrorMessage(mainWindow);
    }
    else {
        errorMessageDialog = new UiGuiErrorMessage(this);
    }

    indenterIniFileList = indenterDirctory.entryList( QStringList("uigui_*.ini") );
    if ( indenterIniFileList.count() > 0 ) {
        // Take care if the selected indenterID is smaller or greater than the number of existing indenters
        if ( indenterID < 0 ) {
            indenterID = 0;
        }
        if ( indenterID >= indenterIniFileList.count() ) {
            indenterID = indenterIniFileList.count() - 1;
        }

        // Reads and parses the by indenterID defined indent ini file and creates toolbox entries
        readIndentIniFile( indenterDirctoryStr + "/" + indenterIniFileList.at(indenterID) );

        // Find out how the indenter can be executed.
        createIndenterCallString();

        // Load the users last settings made for this indenter.
        loadConfigFile( settingsDirctoryStr + "/" + indenterFileName + ".cfg" );

        // Fill the indenter selection combo box with the list of available indenters.
        if ( !getAvailableIndenters().isEmpty() ) {
            indenterSelectionCombobox->addItems( getAvailableIndenters() );
            indenterSelectionCombobox->setCurrentIndex( indenterID );
            connect( indenterSelectionCombobox, SIGNAL(currentIndexChanged(int)), this, SIGNAL(selectedIndenterIndexChanged(int)) );
        }
    }
    else {
        errorMessageDialog->showMessage(tr("No indenter ini files"), tr("There exists no indenter ini files in the directory \"") + QDir(indenterDirctoryStr).absolutePath() + "\".");
    }

    retranslateUi();
}


/*!
    \brief Implicitly writes the current indenter parameters to the indenters config file.
 */
IndentHandler::~IndentHandler() {
    // Generate the parameter string that will be saved to the indenters config file.
    QString parameterString = getParameterString();
    if ( !indenterFileName.isEmpty() ) {
        saveConfigFile( settingsDirctoryStr + "/" + indenterFileName + ".cfg", parameterString );
    }

    delete errorMessageDialog;
}


/*!
    \brief Initializes the context menu used for some actions like saving the indenter config file.
 */
void IndentHandler::initIndenterMenu() {
    if ( menuIndenter == NULL ) {
        actionLoad_Indenter_Config_File = new QAction(this);
        actionLoad_Indenter_Config_File->setObjectName(QString::fromUtf8("actionLoad_Indenter_Config_File"));
        actionLoad_Indenter_Config_File->setIcon(QIcon(QString::fromUtf8(":/mainWindow/load_indent_cfg.png")));

        actionSave_Indenter_Config_File = new QAction(this);
        actionSave_Indenter_Config_File->setObjectName(QString::fromUtf8("actionSave_Indenter_Config_File"));
        actionSave_Indenter_Config_File->setIcon(QIcon(QString::fromUtf8(":/mainWindow/save_indent_cfg.png")));

        actionCreateShellScript = new QAction(this);
        actionCreateShellScript->setObjectName(QString::fromUtf8("actionCreateShellScript"));
        actionCreateShellScript->setIcon(QIcon(QString::fromUtf8(":/mainWindow/shell.png")));

        actionResetIndenterParameters = new QAction(this);
        actionResetIndenterParameters->setObjectName(QString::fromUtf8("actionResetIndenterParameters"));
        actionResetIndenterParameters->setIcon(QIcon(QString::fromUtf8(":/mainWindow/view-refresh.png")));

        menuIndenter = new QMenu(this);
        menuIndenter->setObjectName(QString::fromUtf8("menuIndenter"));
        menuIndenter->addAction(actionLoad_Indenter_Config_File);
        menuIndenter->addAction(actionSave_Indenter_Config_File);
        menuIndenter->addAction(actionCreateShellScript);
        menuIndenter->addAction(actionResetIndenterParameters);
    }
}


/*!
    \brief Returns the context menu used for some actions like saving the indenter config file.
 */
QMenu* IndentHandler::getIndenterMenu() {
    return menuIndenter;
}


/*!
    \brief Returns the actions of the context menu used for some actions like saving the indenter config file.
 */
QList<QAction*> IndentHandler::getIndenterMenuActions() {
    QList<QAction*> actionList;
    actionList << actionLoad_Indenter_Config_File << actionSave_Indenter_Config_File << actionCreateShellScript << actionResetIndenterParameters;
    return actionList;
}


/*!
    \brief Opens the context menu, used for some actions like saving the indenter config file, at the event position.
 */
void IndentHandler::contextMenuEvent( QContextMenuEvent *event ) {
    getIndenterMenu()->exec( event->globalPos() );
}


/*!
    \brief Creates the content for a shell script that can be used as a external tool call
    to indent an as parameter defined file.
 */
QString IndentHandler::generateShellScript(const QString &configFilename) {
    QString indenterCompleteCallString;
    QString parameterInputFile;
    QString parameterOuputFile;
    QString parameterParameterFile;
    QString replaceInputFileCommand;

    // Define the placeholder for parameter variables either in batch or bash programming.
#if defined(Q_OS_WIN32)
    QString shellParameterPlaceholder = "%1";
#else
    QString shellParameterPlaceholder = "$1";
#endif

    parameterInputFile = " " + inputFileParameter + "\"" + shellParameterPlaceholder + "\"";

    if ( outputFileParameter != "none" && outputFileParameter != "stdout" ) {
        if ( outputFileName == inputFileName ) {
            parameterOuputFile = " " + outputFileParameter + "\"" + shellParameterPlaceholder + "\"";
        }
        else {
            parameterOuputFile = " " + outputFileParameter + outputFileName + ".tmp";
        }
    }

    // If the config file name is empty it is assumed that all parameters are sent via command line call
    if ( globalConfigFilename_.isEmpty() ) {
        parameterParameterFile = " " + getParameterString();
    }
    // else if needed add the parameter to the indenter call string where the config file can be found.
    else if (useCfgFileParameter != "none") {
        parameterParameterFile = " " + useCfgFileParameter + "\"./" + configFilename + "\"";
    }

    // Assemble indenter call string for parameters according to the set order.
    if ( parameterOrder == "ipo" ) {
        indenterCompleteCallString = parameterInputFile + parameterParameterFile + parameterOuputFile;
    }
    else if ( parameterOrder == "pio" ) {
        indenterCompleteCallString = parameterParameterFile + parameterInputFile + parameterOuputFile;
    }
    else if ( parameterOrder == "poi" ) {
        indenterCompleteCallString = parameterParameterFile + parameterOuputFile + parameterInputFile;
    }
    else {
        indenterCompleteCallString = parameterInputFile + parameterOuputFile + parameterParameterFile;
    }

    // Generate the indenter call string either for win32 or other systems.
#if defined(Q_OS_WIN32)
    indenterCompleteCallString = indenterExecutableCallString + indenterCompleteCallString;
#else
    indenterCompleteCallString = "#!/bin/bash\n" + indenterExecutableCallString + indenterCompleteCallString;
#endif

    // If the indenter writes to stdout pipe the output into a file
    if ( outputFileParameter == "stdout" ) {
        indenterCompleteCallString = indenterCompleteCallString + " >" + outputFileName + ".tmp";
    }

    // If the output filename is not the same as the input filename copy the output over the input.
    if ( outputFileName != inputFileName ) {
#if defined(Q_OS_WIN32)
        replaceInputFileCommand = "move /Y " + outputFileName + ".tmp \"" + shellParameterPlaceholder + "\"\n";
#else
        replaceInputFileCommand = "mv " + outputFileName + ".tmp \"" + shellParameterPlaceholder + "\"\n";
#endif
    }

#if defined(Q_OS_WIN32)
    QString shellScript( TemplateBatchScript::getTemplateBatchScript() );
    shellScript = shellScript.replace("__INDENTERCALLSTRING2__", indenterCompleteCallString + "\n" + replaceInputFileCommand);
    indenterCompleteCallString = indenterCompleteCallString.replace("%1", "%%G");
    replaceInputFileCommand = replaceInputFileCommand.replace("%1", "%%G");
    shellScript = shellScript.replace("__INDENTERCALLSTRING1__", indenterCompleteCallString + "\n" + replaceInputFileCommand);
#else
    QString shellScript( TemplateBatchScript::getTemplateBatchScript() );
    shellScript = shellScript.replace("__INDENTERCALLSTRING2__", indenterCompleteCallString + "\n" + replaceInputFileCommand);
    indenterCompleteCallString = indenterCompleteCallString.replace("$1", "$file2indent");
    replaceInputFileCommand = replaceInputFileCommand.replace("$1", "$file2indent");
    shellScript = shellScript.replace("__INDENTERCALLSTRING1__", indenterCompleteCallString + "\n" + replaceInputFileCommand);
#endif

    return shellScript;
}


/*!
    \brief Format \a sourceCode by calling the indenter.

    The \a inputFileExtension has to be given as parameter so the called indenter
    can identify the programming language if needed.
 */
QString IndentHandler::callIndenter(QString sourceCode, QString inputFileExtension) {
    if ( indenterExecutableSuffix == ".js" ) {
        return callJavaScriptIndenter(sourceCode);
    }
    else {
        return callExecutableIndenter(sourceCode, inputFileExtension);
    }
}


/*!
    \brief Format \a sourceCode by calling the interpreted JavaScript code of the indenter.

    The \a inputFileExtension has to be given as parameter so the called indenter
    can identify the programming language if needed.
 */
QString IndentHandler::callJavaScriptIndenter(QString sourceCode) {
    QScriptEngine engine;

    QScriptValue unformattedCode(&engine, sourceCode);
    engine.globalObject().setProperty("unformattedCode", unformattedCode);

    QFile jsDecoderFile( indenterExecutableCallString );
    QString jsDecoderCode;
    if (jsDecoderFile.open(QFile::ReadOnly)) {
        jsDecoderCode = jsDecoderFile.readAll();
    }
    jsDecoderFile.close();

    QScriptValue value = engine.evaluate(jsDecoderCode);
    return value.toString();
}


/*!
    \brief Format \a sourceCode by calling the binary executable of the indenter.

    The \a inputFileExtension has to be given as parameter so the called indenter
    can identify the programming language if needed.
 */
QString IndentHandler::callExecutableIndenter(QString sourceCode, QString inputFileExtension) {
    Q_ASSERT_X( !inputFileName.isEmpty(), "callIndenter", "inputFileName is empty" );
//    Q_ASSERT_X( !outputFileName.isEmpty(), "callIndenter", "outputFileName is empty" );
    Q_ASSERT_X( !indenterFileName.isEmpty(), "callIndenter", "indenterFileName is empty" );

    if ( indenterFileName.isEmpty() ) {
        return "";
    }

    QString formattedSourceCode;
    QString indenterCompleteCallString;
    QString parameterInputFile;
    QString parameterOuputFile;
    QString parameterParameterFile;
    QProcess indentProcess;
    QString processReturnString;

    // Generate the parameter string that will be saved to the indenters config file
    QString parameterString = getParameterString();

    if ( !globalConfigFilename_.isEmpty() ) {
        saveConfigFile( tempDirctoryStr + "/" + globalConfigFilename_, parameterString );
    }

    // Only add a dot to file extension if the string is not empty
    if ( !inputFileExtension.isEmpty() ) {
        inputFileExtension = "." + inputFileExtension;
    }

    // Delete any previously used input src file and create a new input src file.
    QFile::remove(tempDirctoryStr + "/" + inputFileName + inputFileExtension);
    QFile inputSrcFile(tempDirctoryStr + "/" + inputFileName + inputFileExtension);
    // Write the source code to the input file for the indenter
    if ( inputSrcFile.open( QFile::ReadWrite | QFile::Text ) ) {
        inputSrcFile.write( sourceCode.toUtf8() );
        inputSrcFile.close();
        qDebug() << __LINE__ << " " << __FUNCTION__ << ": Wrote to be indented source code to file " << inputSrcFile.fileName();
    }
    else {
        qCritical() << __LINE__ << " " << __FUNCTION__ << ": Couldn't write to be indented source code to file " << inputSrcFile.fileName();
    }

    // Set the input file for the to be called indenter.
    if ( inputFileParameter.trimmed() == "<" || inputFileParameter == "stdin" ) {
        parameterInputFile = "";
        indentProcess.setStandardInputFile( inputSrcFile.fileName() );
    }
    else {
        parameterInputFile = " " + inputFileParameter + inputFileName + inputFileExtension;
    }

    // Set the output file for the to be called indenter.
    if ( outputFileParameter != "none" && outputFileParameter != "stdout" ) {
        parameterOuputFile = " " + outputFileParameter + outputFileName + inputFileExtension;
    }

#ifdef Q_OS_WIN32
    // Paths may contain Unicode or other foreign characters. Windows commands line tools will
    // receive als falsely encoded path string by QProcess or they connot correctly handle
    // the Unicode path on their own.
    // Because of this the path gets converted to Windows short paths using the 8.3 notation.

    qDebug() << __LINE__ << " " << __FUNCTION__ << ": Temp dir before trying to convert it to short Windows path is" << tempDirctoryStr;

    // At first convert the temp path to Windows like separators.
    QString tempDirctoryStrHelper = QDir::toNativeSeparators(tempDirctoryStr).replace("\\", "\\\\");
    // Then convert the QString to a WCHAR array and NULL terminate it.
    WCHAR *tempDirctoryWindowsStr = new WCHAR[ tempDirctoryStrHelper.length()+1 ];
    tempDirctoryStrHelper.toWCharArray( tempDirctoryWindowsStr );
    tempDirctoryWindowsStr[ tempDirctoryStrHelper.length() ] = (WCHAR)NULL;

    // Get the length of the resulting short path.
    long length = 0;
    TCHAR *buffer = NULL;
    length = GetShortPathName((LPCTSTR)tempDirctoryWindowsStr, NULL, 0);

    // If the short path could be retrieved, create a correct sized buffer, store the
    // short path in it and convert all back to QString.
    if ( length != 0 ) {
#ifdef UNICODE
        buffer = new WCHAR[length];
        length = GetShortPathName((LPCTSTR)tempDirctoryWindowsStr, buffer, length);
        tempDirctoryStrHelper = QString::fromWCharArray( buffer );
#else
        buffer = new TCHAR[length];
        length = GetShortPathName((LPCTSTR)tempDirctoryWindowsStr, buffer, length);
        tempDirctoryStrHelper = buffer;
#endif
        tempDirctoryStr = QDir::fromNativeSeparators(tempDirctoryStrHelper).replace("//", "/");
        delete [] buffer;

        // Check whether the short path still contains some kind of non ascii characters.
        if ( tempDirctoryStr.length() != tempDirctoryStr.toAscii().length() ) {
            qWarning() << __LINE__ << " " << __FUNCTION__ << ": Shortened path still contains non ascii characters. Could cause some indenters not to work properly!";
        }
    }
    else {
        qWarning() << __LINE__ << " " << __FUNCTION__ << ": Couldn't retrieve a short version of the temporary path!";
    }

    qDebug() << __LINE__ << " " << __FUNCTION__ << ": Temp dir after trying to convert it to short Windows path is " << tempDirctoryStr;

    delete [] tempDirctoryWindowsStr;
#endif

    // If the config file name is empty it is assumed that all parameters are sent via command line call
    if ( globalConfigFilename_.isEmpty() ) {
        parameterParameterFile = " " + parameterString;
    }
    // if needed add the parameter to the indenter call string where the config file can be found
    else if (useCfgFileParameter != "none") {
        parameterParameterFile = " " + useCfgFileParameter + "\"" + tempDirctoryStr + "/" + globalConfigFilename_ + "\"";
    }

    // Assemble indenter call string for parameters according to the set order.
    if ( parameterOrder == "ipo" ) {
        indenterCompleteCallString = parameterInputFile + parameterParameterFile + parameterOuputFile;
    }
    else if ( parameterOrder == "pio" ) {
        indenterCompleteCallString = parameterParameterFile + parameterInputFile + parameterOuputFile;
    }
    else if ( parameterOrder == "poi" ) {
        indenterCompleteCallString = parameterParameterFile + parameterOuputFile + parameterInputFile;
    }
    else {
        indenterCompleteCallString = parameterInputFile + parameterOuputFile + parameterParameterFile;
    }

    // If no indenter executable call string could be created before, show an error message.
    if ( indenterExecutableCallString.isEmpty() ) {
        errorMessageDialog->showMessage(tr("No indenter executable"),
            tr("There exists no indenter executable with the name \"%1\" in the directory \"%2\" nor in the global environment.").arg(indenterFileName).arg(indenterDirctoryStr) );
        return sourceCode;
    }

    // Generate the indenter call string either for win32 or other systems.
    indenterCompleteCallString = indenterExecutableCallString + indenterCompleteCallString;

    // errors and standard outputs from the process call are merged together
    //indentProcess.setReadChannelMode(QProcess::MergedChannels);

    // Set the directory for the indenter execution
    indentProcess.setWorkingDirectory( QFileInfo(tempDirctoryStr).absoluteFilePath() );

    qDebug() << __LINE__ << " " << __FUNCTION__ << ": Will call the indenter in the directory " << indentProcess.workingDirectory() << " using this commandline call: " << indenterCompleteCallString;

    indentProcess.start(indenterCompleteCallString);

    processReturnString = "";
    bool calledProcessSuccessfully = indentProcess.waitForFinished(10000);
    // test if there was an error during starting the process of the indenter
    if ( !calledProcessSuccessfully ) {
        processReturnString = "<html><body>";
        processReturnString += tr("<b>Returned error message:</b> ") + indentProcess.errorString() + "<br>";

        switch ( indentProcess.error() ) {
            case QProcess::FailedToStart :
                processReturnString += tr("<b>Reason could be:</b> ") + "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.<br>";
                break;
            case QProcess::Crashed :
                processReturnString += "The process crashed some time after starting successfully.<br>";
                break;
            case QProcess::Timedout :
                processReturnString += "The called indenter did not response for over 10 seconds, so aborted its execution.<br>";
                break;
            case QProcess::WriteError :
                processReturnString += "An error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.<br>";
                break;
            case QProcess::ReadError :
                processReturnString += "An error occurred when attempting to read from the process. For example, the process may not be running.<br>";
                break;
            case QProcess::UnknownError :
                processReturnString += "An unknown error occurred. This is the default return value of error().<br>";
                break;
            default :
                break;
        }
        processReturnString += tr("<br><b>Callstring was:</b> ") + encodeToHTML(indenterCompleteCallString);
        processReturnString += tr("<br><br><b>Indenter output was:</b><pre>") + "<br>" +
            "(STDOUT):" + encodeToHTML( indentProcess.readAllStandardOutput() ) + "<br>" +
            "(STDERR):" + encodeToHTML( indentProcess.readAllStandardError() ) + "<br>" +
            "</pre></html></body>";
        qWarning() << __LINE__ << " " << __FUNCTION__ << processReturnString;
        QApplication::restoreOverrideCursor();
        errorMessageDialog->showMessage(tr("Error calling Indenter"), processReturnString);
    }

    // If the indenter returned an error code != 0 show its output.
    if ( indentProcess.exitCode() != 0 ) {
        QString exitCode;
        exitCode.setNum(indentProcess.exitCode());
        processReturnString = tr("<b>Indenter returned with exit code:</b> ") + exitCode + "<br>" +
            tr("<b>Indent console output was:</b> ") + "<br>" +
            "(STDOUT):" + encodeToHTML( indentProcess.readAllStandardOutput() ) + "<br>" +
            "(STDERR):" + encodeToHTML( indentProcess.readAllStandardError() ) + "<br>" +
            tr("<br><b>Callstring was:</b> ") + encodeToHTML(indenterCompleteCallString) +
            "</html></body>";
        qWarning() << __LINE__ << " " << __FUNCTION__ << processReturnString;
        QApplication::restoreOverrideCursor();
        errorMessageDialog->showMessage( tr("Indenter returned error"), processReturnString );
    }

    // Only get the formatted source code, if calling the indenter did succeed.
    if ( calledProcessSuccessfully ) {
        // If the indenter results are written to stdout, read them from there...
        if ( indentProcess.exitCode() == 0 && outputFileParameter == "stdout"  ) {
            formattedSourceCode = indentProcess.readAllStandardOutput();
            qDebug() << __LINE__ << " " << __FUNCTION__ << ": Read indenter output from StdOut.";
        }
        // ... else read the output file generated by the indenter call.
        else {
            QFile outSrcFile(tempDirctoryStr + "/" + outputFileName + inputFileExtension);
            if ( outSrcFile.open(QFile::ReadOnly | QFile::Text) ) {
                QTextStream outSrcStrm(&outSrcFile);
                outSrcStrm.setCodec( QTextCodec::codecForName("UTF-8") );
                formattedSourceCode = outSrcStrm.readAll();
                outSrcFile.close();
                qDebug() << __LINE__ << " " << __FUNCTION__ << ": Read indenter output from file " << outSrcFile.fileName();
            }
            else {
                qCritical() << __LINE__ << " " << __FUNCTION__ << ": Couldn't read indenter output from file " << outSrcFile.fileName();
            }
        }
    }
    else {
        return sourceCode;
    }

    // Delete the temporary input and output files.
    QFile::remove(tempDirctoryStr + "/" + outputFileName + inputFileExtension);
    QFile::remove(tempDirctoryStr + "/" + inputFileName + inputFileExtension);

    return formattedSourceCode;
}


/*!
    \brief Generates and returns a string with all parameters needed to call the indenter.
 */
QString IndentHandler::getParameterString() {
    QString parameterString = "";

    // generate parameter string for all boolean values
    foreach (ParamBoolean pBoolean, paramBooleans) {
        if ( pBoolean.checkBox->isChecked() ) {
            if ( !pBoolean.trueString.isEmpty() ) {
                parameterString += pBoolean.trueString + cfgFileParameterEnding;
            }
        }
        else {
            if ( !pBoolean.falseString.isEmpty() ) {
                parameterString += pBoolean.falseString + cfgFileParameterEnding;
            }
        }
    }

    // generate parameter string for all numeric values
    foreach (ParamNumeric pNumeric, paramNumerics) {
        if ( pNumeric.valueEnabledChkBox->isChecked() ) {
            parameterString += pNumeric.paramCallName + QString::number( pNumeric.spinBox->value() ) + cfgFileParameterEnding;
        }
    }

    // generate parameter string for all string values
    foreach (ParamString pString, paramStrings) {
        if ( !pString.lineEdit->text().isEmpty() && pString.valueEnabledChkBox->isChecked() ) {
            // Create parameter definition for each value devided by a | sign.
            foreach (QString paramValue, pString.lineEdit->text().split("|")) {
                parameterString += pString.paramCallName + paramValue + cfgFileParameterEnding;
            }
        }
    }

    // generate parameter string for all multiple choice values
    foreach (ParamMultiple pMultiple, paramMultiples) {
        if ( pMultiple.valueEnabledChkBox->isChecked() ) {
            parameterString += pMultiple.choicesStrings.at( pMultiple.comboBox->currentIndex () ) + cfgFileParameterEnding;
        }
    }

    return parameterString;
}


/*!
    \brief Write settings for the indenter to a config file.
 */
void IndentHandler::saveConfigFile(QString filePathName, QString paramString) {
    QFile::remove( filePathName );
    QFile cfgFile( filePathName );

    cfgFile.open( QFile::ReadWrite | QFile::Text );
    cfgFile.write( paramString.toAscii() );
    cfgFile.close();
}


/*!
    \brief Load the config file for the indenter and apply the settings made there.
 */
bool IndentHandler::loadConfigFile(QString filePathName) {
    QFile cfgFile(filePathName);
    int index;
    int crPos;
    int paramValue = 0;
    QString paramValueStr = "";
    QString cfgFileData = "";

    // If the to be loaded config file does not exist leave all values as they are and return false.
    if ( !cfgFile.exists() ) {
        return false;
    }
    // else if the to be read config file exists, retrieve its whole content.
    else {
        // Open the config file and read all data
        cfgFile.open( QFile::ReadOnly | QFile::Text );
        cfgFileData = cfgFile.readAll();
        cfgFile.close();
    }

    // Search for name of each boolean parameter and set its value if found.
    foreach (ParamBoolean pBoolean, paramBooleans) {
        // boolean value that will be assigned to the checkbox
        bool paramValue = false;

        // first search for the longer parameter string
        // the true parameter string is longer than the false string
        if ( pBoolean.trueString.length() > pBoolean.falseString.length() ) {
            // search for the true string
            index = cfgFileData.indexOf( pBoolean.trueString, 0, Qt::CaseInsensitive );
            // if true string found set the parameter value to true
            if ( index != -1 ) {
                paramValue = true;
            }
            // if true string not found, search for false string
            else {
                index = cfgFileData.indexOf( pBoolean.falseString, 0, Qt::CaseInsensitive );
                // if false string found set the parameter value to false
                if ( index != -1 ) {
                    paramValue = false;
                }
                // neither true nor false parameter found so use default value
                else {
                    paramValue = indenterSettings->value(pBoolean.paramName + "/ValueDefault").toBool();
                }
            }
        }
        // the false parameter string is longer than the true string
        else {
            // search for the false string
            index = cfgFileData.indexOf( pBoolean.falseString, 0, Qt::CaseInsensitive );
            // if false string found set the parameter value to false
            if ( index != -1 ) {
                paramValue = false;
            }
            // if false string not found, search for true string
            else {
                index = cfgFileData.indexOf( pBoolean.trueString, 0, Qt::CaseInsensitive );
                // if true string found set the parameter value to true
                if ( index != -1 ) {
                    paramValue = true;
                }
                // neither true nor false parameter found so use default value
                else {
                    paramValue = indenterSettings->value(pBoolean.paramName + "/ValueDefault").toBool();
                }
            }
        }
        pBoolean.checkBox->setChecked(paramValue);
    }

    // Search for name of each numeric parameter and set the value found behind it.
    foreach (ParamNumeric pNumeric, paramNumerics) {
        index = cfgFileData.indexOf( pNumeric.paramCallName, 0, Qt::CaseInsensitive );
        // parameter was found in config file
        if ( index != -1 ) {
            // set index after the parameter name, so in front of the number
            index += pNumeric.paramCallName.length();

            // Find the end of the parameter by searching for set config file parameter ending. Most of time this is a carriage return.
            crPos = cfgFileData.indexOf( cfgFileParameterEnding, index+1 );

            // get the number and convert it to int
            QString test = cfgFileData.mid( index, crPos - index );
            paramValue = cfgFileData.mid( index, crPos - index ).toInt(NULL);

            // disable the signal-slot connection. Otherwise signal is emmitted each time when value is set
            QObject::disconnect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SLOT(handleChangedIndenterSettings()));
            pNumeric.spinBox->setValue( paramValue );
            pNumeric.valueEnabledChkBox->setChecked( true );
            QObject::connect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SLOT(handleChangedIndenterSettings()));
        }
        // parameter was not found in config file
        else {
            int defaultValue = indenterSettings->value(pNumeric.paramName + "/ValueDefault").toInt();
            pNumeric.spinBox->setValue( defaultValue );
            pNumeric.valueEnabledChkBox->setChecked( false );
        }
    }

    // Search for name of each string parameter and set it.
    foreach (ParamString pString, paramStrings) {
        paramValueStr = "";
        // The number of the found values for this parameter name.
        int numberOfValues = 0;
        index = cfgFileData.indexOf( pString.paramCallName, 0, Qt::CaseInsensitive );
        // If parameter was found in config file
        if ( index != -1 ) {
            while ( index != -1 ) {
                numberOfValues++;

                // Set index after the parameter name, so it points to the front of the string value.
                index += pString.paramCallName.length();

                // Find the end of the parameter by searching for set config file parameter ending. Most of time this is a carriage return.
                crPos = cfgFileData.indexOf( cfgFileParameterEnding, index+1 );

                // Get the string and remember it.
                if ( numberOfValues < 2 ) {
                    paramValueStr = QString( cfgFileData.mid( index, crPos - index ) );
                }
                // If the same parameter has been set multiple times, concatenate the strings dvivided by a |.
                else {
                    paramValueStr = paramValueStr + "|" + QString( cfgFileData.mid( index, crPos - index ) );
                }

                // Get next value for this setting, if one exists.
                index = cfgFileData.indexOf( pString.paramCallName, crPos+1, Qt::CaseInsensitive );
            }
            // Set the text for the line edit.
            pString.lineEdit->setText( paramValueStr );
            pString.valueEnabledChkBox->setChecked( true );
        }
        // Parameter was not found in config file
        else {
            paramValueStr = indenterSettings->value(pString.paramName + "/ValueDefault").toString();
            pString.lineEdit->setText( paramValueStr );
            pString.valueEnabledChkBox->setChecked( false );
        }
    }

    // search for name of each multiple choice parameter and set it
    foreach (ParamMultiple pMultiple, paramMultiples) {
        int i = 0;
        index = -1;

        // search for all parameter names of the multiple choice list
        // if one is found, set it and leave the while loop
        while ( i < pMultiple.choicesStrings.count() && index == -1 ) {
            index = cfgFileData.indexOf( pMultiple.choicesStrings.at(i), 0, Qt::CaseInsensitive );
            if ( index != -1 ) {
                pMultiple.comboBox->setCurrentIndex( i );
                pMultiple.valueEnabledChkBox->setChecked( true );
            }
            i++;
        }

        // parameter was not set in config file, so use default value
        if ( index == -1 ) {
            int defaultValue = indenterSettings->value(pMultiple.paramName + "/ValueDefault").toInt();
            pMultiple.comboBox->setCurrentIndex( defaultValue );
            pMultiple.valueEnabledChkBox->setChecked( false );
        }
    }

    return true;
}


/*!
    \brief Sets all indenter parameters to their default values defined in the ini file.
 */
void IndentHandler::resetToDefaultValues() {
    // Search for name of each boolean parameter and set its value if found.
    foreach (ParamBoolean pBoolean, paramBooleans) {
        // Boolean value that will be assigned to the checkbox.
        bool defaultValue = indenterSettings->value(pBoolean.paramName + "/ValueDefault").toBool();
        pBoolean.checkBox->setChecked( defaultValue );
    }

    // Search for name of each numeric parameter and set the value found behind it.
    foreach (ParamNumeric pNumeric, paramNumerics) {
        int defaultValue = indenterSettings->value(pNumeric.paramName + "/ValueDefault").toInt();
        pNumeric.spinBox->setValue( defaultValue );
        pNumeric.valueEnabledChkBox->setChecked( indenterSettings->value(pNumeric.paramName + "/Enabled").toBool() );
    }

    // Search for name of each string parameter and set it.
    foreach (ParamString pString, paramStrings) {
        QString defaultValue = indenterSettings->value(pString.paramName + "/ValueDefault").toString();
        pString.lineEdit->setText( defaultValue );
        pString.valueEnabledChkBox->setChecked( indenterSettings->value(pString.paramName + "/Enabled").toBool() );
    }

    // Search for name of each multiple choice parameter and set it.
    foreach (ParamMultiple pMultiple, paramMultiples) {
        int defaultValue = indenterSettings->value(pMultiple.paramName + "/ValueDefault").toInt();
        pMultiple.comboBox->setCurrentIndex( defaultValue );
        pMultiple.valueEnabledChkBox->setChecked( indenterSettings->value(pMultiple.paramName + "/Enabled").toBool() );
    }
}


/*!
    \brief Opens and parses the indenter ini file that is declared by \a iniFilePath.
 */
void IndentHandler::readIndentIniFile(QString iniFilePath) {
    Q_ASSERT_X( !iniFilePath.isEmpty(), "readIndentIniFile", "iniFilePath is empty" );

    // open the ini-file that contains all available indenter settings with their additional infos
    indenterSettings = new UiGuiIniFileParser(iniFilePath);

    QStringList categories;
    //QString indenterGroupString = "";
    QString paramToolTip = "";


    //
    //  parse ini file indenter header
    //

    indenterName = indenterSettings->value("header/indenterName").toString();
    indenterFileName = indenterSettings->value("header/indenterFileName").toString();
    globalConfigFilename_ = indenterSettings->value("header/configFilename").toString();
    useCfgFileParameter = indenterSettings->value("header/useCfgFileParameter").toString();
    cfgFileParameterEnding = indenterSettings->value("header/cfgFileParameterEnding").toString();
    if ( cfgFileParameterEnding == "cr" ) {
        cfgFileParameterEnding = "\n";
    }
    indenterShowHelpParameter = indenterSettings->value("header/showHelpParameter").toString();

    if ( indenterFileName.isEmpty() ) {
        errorMessageDialog->showMessage( tr("Indenter ini file header error"),
            tr("The loaded indenter ini file \"%1\"has a faulty header. At least the indenters file name is not set.").arg(iniFilePath) );
    }

    // Read the parameter order. Possible values are (p=parameter[file] i=inputfile o=outputfile)
    // pio, ipo, iop
    parameterOrder = indenterSettings->value("header/parameterOrder", "pio").toString();
    inputFileParameter = indenterSettings->value("header/inputFileParameter").toString();
    inputFileName = indenterSettings->value("header/inputFileName").toString();
    outputFileParameter = indenterSettings->value("header/outputFileParameter").toString();
    outputFileName = indenterSettings->value("header/outputFileName").toString();
    fileTypes = indenterSettings->value("header/fileTypes").toString();
    fileTypes.replace('|', " ");

    // read the categories names which are separated by "|"
    QString categoriesStr = indenterSettings->value("header/categories").toString();
    categories = categoriesStr.split("|");
    // Assure that the category list is never empty. At least contain a "general" section.
    if ( categories.isEmpty() ) {
        categories.append("General");
    }

    ToolBoxPage toolBoxPage;

    // create a page for each category and store its references in a toolboxpage-array
    foreach (QString category, categories) {
        toolBoxPage.page = new QWidget();
        toolBoxPage.page->setObjectName(category);
        toolBoxPage.page->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        toolBoxPage.vboxLayout = new QVBoxLayout(toolBoxPage.page);
        toolBoxPage.vboxLayout->setSpacing(6);
        toolBoxPage.vboxLayout->setMargin(9);
        toolBoxPage.vboxLayout->setObjectName(category);
        toolBoxPages.append(toolBoxPage);
        toolBox->addItem(toolBoxPage.page, category);
    }


    //
    //  parse ini file indenter parameters
    //

    // read all possible parameters written in brackets []
    indenterParameters = indenterSettings->childGroups();

    // read each parameter to create the corresponding input field
    foreach (QString indenterParameter, indenterParameters) {
        // if it is not the indent header definition read the parameter and add it to
        // the corresponding category toolbox page
        if ( indenterParameter != "header") {
            // read to which category the parameter belongs
            int category = indenterSettings->value(indenterParameter + "/Category").toInt();
            // Assure that the category number is never greater than the available categories.
            if ( category > toolBoxPages.size()-1 ) {
                category = toolBoxPages.size()-1;
            }
            // read which type of input field the parameter needs
            QString editType = indenterSettings->value(indenterParameter + "/EditorType").toString();

            // edit type is numeric so create a spinbox with label
            if ( editType == "numeric" ) {
                // read the parameter name as it is used at the command line or in its config file
                QString parameterCallName = indenterSettings->value(indenterParameter + "/CallName").toString();

                // create checkbox which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( indenterSettings->value(indenterParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the spinbox
                QSpinBox *spinBox = new QSpinBox( toolBoxPages.at(category).page );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                spinBox->setToolTip( paramToolTip );
                spinBox->setMaximumWidth(50);
                spinBox->setMinimumWidth(50);
                if ( mainWindow != NULL ) {
                    spinBox->installEventFilter( mainWindow );
                }
                if ( indenterSettings->value(indenterParameter + "/MinVal").toString() != "" ) {
                    spinBox->setMinimum( indenterSettings->value(indenterParameter + "/MinVal").toInt() );
                }
                else {
                    spinBox->setMinimum( 0 );
                }
                if ( indenterSettings->value(indenterParameter + "/MaxVal").toString() != "" ) {
                    spinBox->setMaximum( indenterSettings->value(indenterParameter + "/MaxVal").toInt() );
                }
                else {
                    spinBox->setMaximum( 2000 );
                }

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(indenterParameter);
                label->setBuddy(spinBox);
                label->setToolTip( paramToolTip );
                if ( mainWindow != NULL ) {
                    label->installEventFilter( mainWindow );
                }

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(spinBox);
                hboxLayout->addWidget(label);
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its spinbox
                ParamNumeric paramNumeric;
                paramNumeric.paramName = indenterParameter;
                paramNumeric.paramCallName = parameterCallName;
                paramNumeric.spinBox = spinBox;
                paramNumeric.label = label;
                paramNumeric.valueEnabledChkBox = chkBox;
                paramNumeric.spinBox->setValue( indenterSettings->value(paramNumeric.paramName + "/ValueDefault").toInt() );
                paramNumerics.append(paramNumeric);

                QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(handleChangedIndenterSettings()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SLOT(handleChangedIndenterSettings()));
#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
                connect( spinBox, SIGNAL(valueChanged(int)), this, SLOT(updateDrawing()) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
            }
            // edit type is boolean so create a checkbox
            else if ( editType == "boolean" ) {
                // create the checkbox, make its settings and add it to the toolbox page
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setText(indenterParameter);
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                chkBox->setToolTip( paramToolTip );
                if ( mainWindow != NULL ) {
                    chkBox->installEventFilter( mainWindow );
                }
                toolBoxPages.at(category).vboxLayout->addWidget(chkBox);

                // remember parameter name and reference to its checkbox
                ParamBoolean paramBoolean;
                paramBoolean.paramName = indenterParameter;
                paramBoolean.checkBox = chkBox;
                QStringList trueFalseStrings = indenterSettings->value(indenterParameter + "/TrueFalse").toString().split("|");
                paramBoolean.trueString = trueFalseStrings.at(0);
                paramBoolean.falseString = trueFalseStrings.at(1);
                paramBoolean.checkBox->setChecked( indenterSettings->value(paramBoolean.paramName + "/ValueDefault").toBool() );
                paramBooleans.append(paramBoolean);

                QObject::connect(chkBox, SIGNAL(clicked()), this, SLOT(handleChangedIndenterSettings()));
            }
            // edit type is numeric so create a line edit with label
            else if ( editType == "string" ) {
                // read the parameter name as it is used at the command line or in its config file
                QString parameterCallName = indenterSettings->value(indenterParameter + "/CallName").toString();

                // create check box which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( indenterSettings->value(indenterParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the line edit
                QLineEdit *lineEdit = new QLineEdit( toolBoxPages.at(category).page );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                lineEdit->setToolTip( paramToolTip );
                lineEdit->setMaximumWidth(50);
                lineEdit->setMinimumWidth(50);
                if ( mainWindow != NULL ) {
                    lineEdit->installEventFilter( mainWindow );
                }

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(indenterParameter);
                label->setBuddy(lineEdit);
                label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
                label->setToolTip( paramToolTip );
                if ( mainWindow != NULL ) {
                    label->installEventFilter( mainWindow );
                }

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(lineEdit);
                hboxLayout->addWidget(label);
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its line edit
                ParamString paramString;
                paramString.paramName = indenterParameter;
                paramString.paramCallName = parameterCallName;
                paramString.lineEdit = lineEdit;
                paramString.label = label;
                paramString.valueEnabledChkBox = chkBox;
                paramString.lineEdit->setText( indenterSettings->value(paramString.paramName + "/ValueDefault").toString() );
                paramStrings.append(paramString);

                QObject::connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(handleChangedIndenterSettings()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SLOT(handleChangedIndenterSettings()));
#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
                connect( lineEdit, SIGNAL(textChanged(const QString)), this, SLOT(updateDrawing()) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
            }
            // edit type is multiple so create a combobox with label
            else if ( editType == "multiple" ) {
                // read the parameter name as it is used at the command line or in its config file
                QString parameterCallName = indenterSettings->value(indenterParameter + "/CallName").toString();

                // create checkbox which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( indenterSettings->value(indenterParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the combo box
                QComboBox *comboBox = new QComboBox( toolBoxPages.at(category).page );
                QStringList choicesStrings = indenterSettings->value(indenterParameter + "/Choices").toString().split("|");
                QStringList choicesStringsReadable = indenterSettings->value(indenterParameter + "/ChoicesReadable").toString().split("|", QString::SkipEmptyParts);
                if ( choicesStringsReadable.isEmpty() ) {
                    comboBox->addItems( choicesStrings );
                }
                else {
                    comboBox->addItems( choicesStringsReadable );
                }
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                comboBox->setToolTip( paramToolTip );
                if ( mainWindow != NULL ) {
                    comboBox->installEventFilter( mainWindow );
                }

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(comboBox);
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its lineedit
                ParamMultiple paramMultiple;
                paramMultiple.paramName = indenterParameter;
                paramMultiple.paramCallName = parameterCallName;
                paramMultiple.comboBox = comboBox;
                paramMultiple.choicesStrings = choicesStrings;
                paramMultiple.choicesStringsReadable = choicesStringsReadable;
                paramMultiple.valueEnabledChkBox = chkBox;
                paramMultiple.comboBox->setCurrentIndex( indenterSettings->value(paramMultiple.paramName + "/ValueDefault").toInt() );
                paramMultiples.append(paramMultiple);

                QObject::connect(comboBox, SIGNAL(activated(int)), this, SLOT(handleChangedIndenterSettings()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SLOT(handleChangedIndenterSettings()));
#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
                connect( comboBox, SIGNAL(activated(int)), this, SLOT(updateDrawing()) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
            }
        }
    }

    // put a spacer at each page end
    foreach (ToolBoxPage tbp, toolBoxPages) {
        tbp.vboxLayout->addStretch();
    }
}


/*!
    \brief Searches and returns all indenters a configuration file is found for.

    Opens all uigui ini files found in the list \a indenterIniFileList, opens each ini file
    and reads the there defined real name of the indenter. These names are being returned as QStringList.
 */
QStringList IndentHandler::getAvailableIndenters() {
    QStringList indenterNamesList;

    // Loop for every existing uigui ini file
    foreach (QString indenterIniFile, indenterIniFileList) {
        // Open the ini file and search for the indenter name
        QFile file(indenterDirctoryStr + "/" + indenterIniFile);
        if ( file.open(QIODevice::ReadOnly | QIODevice::Text) ) {
            int index = -1;
            QByteArray line;
            // Search for the string "indenterName=" and get the following string until line end.
            while ( index == -1 &&  !file.atEnd() ) {
                line = file.readLine();
                index = line.indexOf( "indenterName=", 0);
            }

            if ( index == 0 ) {
                line = line.remove(0, 13);
                indenterNamesList << line.trimmed();
            }
        }
    }
    return indenterNamesList;
}


/*!
    \brief Deletes all elements in the toolbox and initializes the indenter selected by \a indenterID.
 */
void IndentHandler::setIndenter(int indenterID) {
    QApplication::setOverrideCursor(Qt::WaitCursor);

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    disconnect( toolBox, SIGNAL(currentChanged(int)), this, SLOT(updateDrawing()) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS

    // Generate the parameter string that will be saved to the indenters config file.
    QString parameterString = getParameterString();
    if ( !indenterFileName.isEmpty() ) {
        saveConfigFile( settingsDirctoryStr + "/" + indenterFileName + ".cfg", parameterString );
    }

    // Take care if the selected indenterID is smaller or greater than the number of existing indenters
    if ( indenterID < 0 ) {
        indenterID = 0;
    }
    if ( indenterID >= indenterIniFileList.count() ) {
        indenterID = indenterIniFileList.count() - 1;
    }

    // remove all pages from the toolbox
    for (int i = 0; i < toolBox->count(); i++) {
        toolBox->removeItem(i);
    }

    // delete all toolbox pages and by this its children
    foreach (ToolBoxPage toolBoxPage, toolBoxPages) {
        delete toolBoxPage.page;
    }

    // empty all lists, which stored infos for the toolbox pages and its widgets
    toolBoxPages.clear();
    paramStrings.clear();
    paramNumerics.clear();
    paramBooleans.clear();
    paramMultiples.clear();
    delete indenterSettings;

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    QWidget dummyWidget;
    toolBox->addItem(&dummyWidget, "dummyText");
#endif

    readIndentIniFile( indenterDirctoryStr + "/" + indenterIniFileList.at(indenterID) );

    // Find out how the indenter can be executed.
    createIndenterCallString();

    // Load the users last settings made for this indenter.
    loadConfigFile( settingsDirctoryStr + "/" + indenterFileName + ".cfg" );

    handleChangedIndenterSettings();

    QApplication::restoreOverrideCursor();

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    connect( toolBox, SIGNAL(currentChanged(int)), this, SLOT(updateDrawing()) );
    toolBox->removeItem( toolBox->indexOf(&dummyWidget) );
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
}


/*!
    \brief Returns a string containing by the indenter supported file types/extensions divided by a space.
 */
QString IndentHandler::getPossibleIndenterFileExtensions() {
    return fileTypes;
}


/*!
    \brief Returns the path and filename of the current indenter config file.
 */
QString IndentHandler::getIndenterCfgFile() {
    QFileInfo fileInfo( indenterDirctoryStr + "/" + globalConfigFilename_ );
    return fileInfo.absoluteFilePath();
}


/*!
    \brief Tries to create a call path string for the indenter executable. If successful returns true.
 */
bool IndentHandler::createIndenterCallString() {
    QProcess indentProcess;

    if ( indenterFileName.isEmpty() ) {
        return false;
    }

    // First try to call the indenter inside of the data dir, using some suffix
    // ------------------------------------------------------------------------

    // Set the directory for the indenter execution
    indentProcess.setWorkingDirectory( QFileInfo(indenterDirctoryStr).absoluteFilePath() );

    foreach ( QString suffix, QStringList() << "" << ".exe" << ".bat" << ".com" << ".sh" ) {
        indenterExecutableSuffix = suffix;
        indenterExecutableCallString = QFileInfo(indenterDirctoryStr).absoluteFilePath() + "/" + indenterFileName;
        indenterExecutableCallString += suffix;

        // Only try to call the indenter, if the file exists.
        if ( QFile::exists(indenterExecutableCallString) ) {
            // Only try to call the indenter directly if it is no php file
            if ( QFileInfo(indenterExecutableCallString).suffix().toLower() != "php" ) {
                indentProcess.start( "\"" + indenterExecutableCallString +  + "\" " + indenterShowHelpParameter );
                if ( indentProcess.waitForFinished(2000) ) {
                    indenterExecutableCallString = "\"" + indenterExecutableCallString + "\"";
                    return true;
                }
                else if ( indentProcess.error() == QProcess::Timedout ) {
                    indenterExecutableCallString = "\"" + indenterExecutableCallString + "\"";
                    return true;
                }
            }

            // Test for needed interpreters
            // ----------------------------
            // If the file could not be executed, try to find a shebang at its start or test if its a php file.
            QString interpreterName = "";
            QFile indenterExecutable( indenterExecutableCallString );

            // If indenter executable file has .php as suffix, use php as default interpreter
            if ( QFileInfo(indenterExecutableCallString).suffix().toLower() == "php" ) {
                interpreterName = "php -f";
            }
            // Else try to open the file and read the shebang.
            else if ( indenterExecutable.open(QFile::ReadOnly) ) {
                // Read the first line of the file.
                QTextStream indenterExecutableContent(&indenterExecutable);
                QString firstLineOfIndenterExe = indenterExecutableContent.readLine(75);
                indenterExecutable.close();

                // If the initial shebang is found, read the named intepreter. e.g. perl
                if ( firstLineOfIndenterExe.startsWith("#!") ) {
                    // Get the rightmost word. by splitting the string into only full words.
                    interpreterName = firstLineOfIndenterExe.split( "/" ).last();
                }
            }

            // Try to call the interpreter, if it exists.
            if ( !interpreterName.isEmpty() ) {
                indenterExecutableCallString = interpreterName + " \"" + indenterExecutableCallString + "\"";
                indentProcess.start( interpreterName + " -h");
                if ( indentProcess.waitForFinished(2000) ) {
                    return true;
                }
                else if ( indentProcess.error() == QProcess::Timedout ) {
                    return true;
                }
                // now we know an interpreter is needed but it could not be called, so inform the user.
                else {
                    errorMessageDialog->showMessage( tr("Interpreter needed"),
                        tr("To use the selected indenter the program \"%1\" needs to be available in the global environment. You should add an entry to your path settings.").arg(interpreterName) );
                    return true;
                }
            }
        }
    }


    // If unsuccessful try if the indenter executable is a JavaScript file
    // -------------------------------------------------------------------
    indenterExecutableSuffix = ".js";
    indenterExecutableCallString = QFileInfo(indenterDirctoryStr).absoluteFilePath() + "/" + indenterFileName;
    indenterExecutableCallString += indenterExecutableSuffix;
    if ( QFile::exists(indenterExecutableCallString) ) {
        return true;
    }


    // If unsuccessful try to call the indenter global, using some suffix
    // ------------------------------------------------------------------
    foreach ( QString suffix, QStringList() << "" << ".exe" << ".bat" << ".com" << ".sh" ) {
        indenterExecutableSuffix = suffix;
        indenterExecutableCallString = indenterFileName + suffix;
        indentProcess.start( indenterExecutableCallString + " " + indenterShowHelpParameter );
        if ( indentProcess.waitForFinished(2000) ) {
            return true;
        }
        else if ( indentProcess.error() == QProcess::Timedout ) {
            return true;
        }
    }


    // If even globally calling the indenter fails, try calling .com and .exe via wine
    // -------------------------------------------------------------------------------
    indenterExecutableCallString = "\"" + QFileInfo(indenterDirctoryStr).absoluteFilePath() + "/" + indenterFileName;

    foreach ( QString suffix, QStringList() << ".exe" << ".com" ) {
        indenterExecutableSuffix = suffix;
        if ( QFile::exists(indenterDirctoryStr + "/" + indenterFileName + suffix) ) {
            QProcess wineTestProcess;
            wineTestProcess.start("wine --version");
            // if the process of wine was not callable assume that wine is not installed
            if ( !wineTestProcess.waitForFinished(2000) ) {
                errorMessageDialog->showMessage(tr("wine not installed"), tr("There exists only a win32 executable of the indenter and wine does not seem to be installed. Please install wine to be able to run the indenter.") );
                indenterExecutableCallString = "";
                return false;
            }
            else {
                indenterExecutableCallString = "\"" + QFileInfo(indenterDirctoryStr).absoluteFilePath() + "/";
                indenterExecutableCallString += indenterFileName + suffix + "\"";
                indenterExecutableCallString = "wine " + indenterExecutableCallString;

                return true;
            }
        }
    }

    indenterExecutableCallString = "";
    indenterExecutableSuffix = "";
    return false;
}


/*!
    \brief Returns a string that points to where the indenters manual can be found.
 */
QString IndentHandler::getManual() {
    if ( indenterSettings != NULL ) {
        return indenterSettings->value("header/manual").toString();
    }
    else {
        return "";
    }
}


/*!
    \brief This slot gets the reference to the indenters manual and opens it.
 */
void IndentHandler::showIndenterManual() {
    QString manualReference = getManual();
    QDesktopServices::openUrl( manualReference );
}


/*!
    \brief Can be called to update all widgets text to the currently selected language.
 */
void IndentHandler::retranslateUi() {
    indenterSelectionCombobox->setToolTip( tr("<html><head><meta name=\"qrichtext\" content=\"1\" /></head><body style=\" white-space: pre-wrap; font-family:MS Shell Dlg; font-size:8.25pt; font-weight:400; font-style:normal; text-decoration:none;\"><p style=\" margin-top:0px; margin-bottom:0px; margin-left:0px; margin-right:0px; -qt-block-indent:0; text-indent:0px;\">Shows the currently chosen indenters name and lets you choose other available indenters</p></body></html>") );
    indenterParameterHelpButton->setToolTip( tr("Brings you to the online manual of the currently selected indenter, where you can get further help on the possible parameters.") );

    actionLoad_Indenter_Config_File->setText(QApplication::translate("IndentHandler", "Load Indenter Config File", 0, QApplication::UnicodeUTF8));
    actionLoad_Indenter_Config_File->setStatusTip(QApplication::translate("IndentHandler", "Opens a file dialog to load the original config file of the indenter.", 0, QApplication::UnicodeUTF8));
    actionLoad_Indenter_Config_File->setShortcut(QApplication::translate("IndentHandler", "Alt+O", 0, QApplication::UnicodeUTF8));

    actionSave_Indenter_Config_File->setText(QApplication::translate("IndentHandler", "Save Indenter Config File", 0, QApplication::UnicodeUTF8));
    actionSave_Indenter_Config_File->setStatusTip(QApplication::translate("IndentHandler", "Opens a dialog to save the current indenter configuration to a file.", 0, QApplication::UnicodeUTF8));
    actionSave_Indenter_Config_File->setShortcut(QApplication::translate("IndentHandler", "Alt+S", 0, QApplication::UnicodeUTF8));

    actionCreateShellScript->setText(QApplication::translate("IndentHandler", "Create Indenter Call Shell Script", 0, QApplication::UnicodeUTF8));
    actionCreateShellScript->setToolTip(QApplication::translate("IndentHandler", "Create a shell script that calls the current selected indenter for formatting an as parameter given file with the current indent settings.", 0, QApplication::UnicodeUTF8));
    actionCreateShellScript->setStatusTip(QApplication::translate("IndentHandler", "Create a shell script that calls the current selected indenter for formatting an as parameter given file with the current indent settings.", 0, QApplication::UnicodeUTF8));

    actionResetIndenterParameters->setText(QApplication::translate("IndentHandler", "Reset indenter parameters", 0, QApplication::UnicodeUTF8));
    actionResetIndenterParameters->setToolTip(QApplication::translate("IndentHandler", "Resets all indenter parameters to the default values.", 0, QApplication::UnicodeUTF8));
    actionResetIndenterParameters->setStatusTip(QApplication::translate("IndentHandler", "Resets all indenter parameters to the default values.", 0, QApplication::UnicodeUTF8));
}


/*!
    \brief Returns the name of the currently selected indenter.
 */
QString IndentHandler::getCurrentIndenterName() {
    QString currentIndenterName = indenterSelectionCombobox->currentText();

    // Remove the supported programming languages from indenters name, which are set in braces.
    if ( currentIndenterName.indexOf("(") > 0 ) {
        // Using index-1 to also leave out the blank before the brace.
        currentIndenterName = currentIndenterName.left( currentIndenterName.indexOf("(")-1 );
    }

    return currentIndenterName;
}


/*!
    \brief Shows a file open dialog to open an existing config file for the currently selected indenter.

    If the file was successfully opened the indent handler is called to load the settings and update itself.
*/
void IndentHandler::openConfigFileDialog() {
    QString configFilePath;

    configFilePath = QFileDialog::getOpenFileName( NULL, tr("Choose indenter config file"), getIndenterCfgFile(), "All files (*.*)" );

    if (configFilePath != "") {
        // If the config file was loaded successfully, inform any who is interested about it.
        if ( loadConfigFile(configFilePath) )
            handleChangedIndenterSettings();
    }
}


/*!
    \brief Calls the indenter config file save as dialog to save the config file under a chosen name.

    If the file already exists and it should be overwritten, a warning is shown before.
*/
void IndentHandler::saveasIndentCfgFileDialog() {
    QString fileExtensions = tr("All files")+" (*.*)";

    //QString openedSourceFileContent = openFileDialog( tr("Choose source code file"), "./", fileExtensions );
    QString fileName = QFileDialog::getSaveFileName( this, tr("Save indent config file"), getIndenterCfgFile(), fileExtensions);

    if (fileName != "") {
        QFile::remove(fileName);
        QFile outCfgFile(fileName);
        outCfgFile.open( QFile::ReadWrite | QFile::Text );
        outCfgFile.write( getParameterString().toAscii() );
        outCfgFile.close();
    }
}


/*!
    \brief Invokes the indenter to create a shell script.

    Lets the indenter create a shell script for calling the indenter out of any
    other application and open a save dialog for saving the shell script.
*/
void IndentHandler::createIndenterCallShellScript() {
    QString shellScriptExtension;
#if defined(Q_OS_WIN32)
    shellScriptExtension = "bat";
#else
    shellScriptExtension = "sh";
#endif

    QString fileExtensions = tr("Shell Script")+" (*."+shellScriptExtension+");;"+tr("All files")+" (*.*)";

    QString currentIndenterName = getCurrentIndenterName();
    currentIndenterName = currentIndenterName.replace(" ", "_");

    QString shellScriptFileName = QFileDialog::getSaveFileName( this, tr("Save shell script"), "call_"+currentIndenterName+"."+shellScriptExtension, fileExtensions);

    // Saving has been canceled if the filename is empty
    if ( shellScriptFileName.isEmpty() ) {
        return;
    }

    // Delete any old file, write the new contents and set executable permissions.
    QFile::remove(shellScriptFileName);
    QFile outSrcFile(shellScriptFileName);
    if ( outSrcFile.open( QFile::ReadWrite | QFile::Text ) ) {
        QString shellScriptConfigFilename = QFileInfo(shellScriptFileName).baseName() + "." + QFileInfo(globalConfigFilename_).suffix();

        // Get the content of the shell/batch script.
        QString indenterCallShellScript = generateShellScript(shellScriptConfigFilename);

        // Replace placeholder for script name in script template.
        indenterCallShellScript = indenterCallShellScript.replace( "__INDENTERCALLSTRINGSCRIPTNAME__", QFileInfo(shellScriptFileName).fileName() );

        outSrcFile.write( indenterCallShellScript.toAscii() );
#if !defined(Q_OS_WIN32)
        // For none Windows systems set the files executable flag
        outSrcFile.setPermissions( outSrcFile.permissions() | QFile::ExeOwner | QFile::ExeUser| QFile::ExeGroup );
#endif
        outSrcFile.close();

        // Save the indenter config file to the same directory, where the shell srcipt was saved to,
        // because the script will reference it there via "./".
        if ( !globalConfigFilename_.isEmpty() ) {
            saveConfigFile( QFileInfo(shellScriptFileName).path() + "/" + shellScriptConfigFilename, getParameterString() );
        }
    }
}


/*!
    \brief Resets all parameters to the indenters default values as they are specified in the uigui ini file
    but asks the user whether to do it really.
 */
void IndentHandler::resetIndenterParameter() {
    int messageBoxAnswer = QMessageBox::question(this, tr("Really reset parameters?"), tr("Do you really want to reset the indenter parameters to the default values?"), QMessageBox::Yes | QMessageBox::Abort );
    if ( messageBoxAnswer == QMessageBox::Yes ) {
        resetToDefaultValues();
    }
}


/*!
    \brief Catch some events and let some other be handled by the super class.

    Is needed for use as Notepad++ plugin.
 */
bool IndentHandler::event( QEvent *event ) {
    if ( event->type() == QEvent::WindowActivate ) {
        event->accept();
        return true;
    }
    else if ( event->type() == QEvent::WindowDeactivate ) {
        event->accept();
        return true;
    }
    else {
        event->ignore();
        return QWidget::event(event);
    }
}


/*!
    \brief Sets the function pointer \a parameterChangedCallback to the given callback
    function \a paramChangedCallback.

    Is needed for use as Notepad++ plugin.
 */
void IndentHandler::setParameterChangedCallback( void(*paramChangedCallback)(void) ) {
    parameterChangedCallback = paramChangedCallback;
}


/*!
    \brief Emits the \a indenterSettingsChanged signal and if set executes the \a parameterChangedCallback function.

   Is needed for use as Notepad++ plugin.
 */
void IndentHandler::handleChangedIndenterSettings() {
    emit( indenterSettingsChanged() );

    if ( parameterChangedCallback != NULL ) {
        parameterChangedCallback();
    }
}


/*!
    \brief Sets a callback function that shall be called, when the this indenter parameter window gets closed.

   Is needed for use as Notepad++ plugin.
 */
void IndentHandler::setWindowClosedCallback( void(*winClosedCallback)(void) ) {
    windowClosedCallback = winClosedCallback;
}


/*!
    \brief Is called on this indenter parameter window close and if set calls the function \a windowClosedCallback.

    Is needed for use as Notepad++ plugin.
 */
void IndentHandler::closeEvent(QCloseEvent *event) {
    if ( windowClosedCallback != NULL ) {
        windowClosedCallback();
    }
    event->accept();
}


/*!
    \brief Returns the id (list index) of the currently selected indenter.
 */
int IndentHandler::getIndenterId() {
    return indenterSelectionCombobox->currentIndex();
}


void IndentHandler::updateDrawing() {
#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    if ( isVisible() ) {
        QRect savedGeometry = geometry();
        setGeometry( savedGeometry.adjusted(0,0,0,1) );
        repaint();
        setGeometry( savedGeometry );
    }
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
}

void IndentHandler::wheelEvent( QWheelEvent *event ) {
#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    QWidget::wheelEvent( event );
    updateDrawing();
#endif // UNIVERSALINDENTGUI_NPP_EXPORTS
}


/*!
    \brief Converts characters < > and & in the \a text to HTML codes &lt &gt and &amp.
 */
QString IndentHandler::encodeToHTML(const QString &text) {
    QString htmlText = text;
    htmlText.replace("&", "&amp;");
    htmlText.replace("<", "&lt;");
    htmlText.replace(">", "&gt;");
    htmlText.replace('"', "&quot;");
    htmlText.replace("'", "&#39;");
    htmlText.replace("^", "&circ;");
    htmlText.replace("~", "&tilde;");
    htmlText.replace("€", "&euro;");
    htmlText.replace("©", "&copy;");
    return htmlText;
}
