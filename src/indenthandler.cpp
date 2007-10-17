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

#include "indenthandler.h"

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
IndentHandler::IndentHandler(QString dataDirPathStr, int indenterID, QMainWindow *mainWindow, QWidget *parent)
    : QWidget(parent)
{
    Q_ASSERT_X( indenterID >= 0, "IndentHandler", "the selected indenterID is < 0" );

    this->mainWindow = mainWindow;

    // define this widgets size and resize behavior
    //this->setMaximumWidth(263);
    this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // create vertical layout box, into which the toolbox will be added
    vboxLayout = new QVBoxLayout(this);
    vboxLayout->setMargin(2);

    // create a toolbox and set its resize behavior
    toolBox = new QToolBox(this);
    toolBox->setObjectName(QString::fromUtf8("toolBox"));
    //toolBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    //toolBox->setMaximumSize(QSize(16777215, 16777215));
    // insert the toolbox into the vlayout
    vboxLayout->addWidget(toolBox);

    dataDirctoryStr = dataDirPathStr;
    QDir dataDirctory = QDir(dataDirPathStr);

    indenterIniFileList = dataDirctory.entryList( QStringList("uigui_*.ini") );
    if ( indenterIniFileList.count() > 0 ) {
        // Take care if the selected indenterID is smaller or greater than the number of existing indenters
        if ( indenterID < 0 ) {
            indenterID = 0;
        }
        if ( indenterID >= indenterIniFileList.count() ) {
            indenterID = indenterIniFileList.count() - 1;
        }

        // reads and parses first found indent ini file and creates toolbox entries
        readIndentIniFile( dataDirctoryStr + indenterIniFileList.at(indenterID) );
    }
    noIndenterExecExistDialogAlreadyShown = false;

    errorMessageDialog = new UiGuiErrorMessage(mainWindow);
}


IndentHandler::~IndentHandler() {
    delete errorMessageDialog;
}


/*!
    \brief Creates the content for a shell script that can be used as a external too call
    to indent a as parameter defined file.
 */
QString IndentHandler::generateCommandlineCall(QString inputFileExtension) {

	QString indentCallString;
	QString parameterInputFile;
	QString parameterOuputFile;
	QString parameterParameterFile;
    QString replaceInputFileCommand;

    // Define the placeholder for variable either in batch or bash programming.
#if defined(Q_OS_WIN32)
    QString shellPlaceholder = "%1";
#else
    QString shellPlaceholder = "$1";
#endif

    // Generate the parameter string that will be save to the indenters config file.
    QString parameterString = getParameterString();

	if ( !configFilename.isEmpty() ) {
		writeConfigFile( parameterString );
	}

    // Only add point to file extension if the string is not empty.
    if ( !inputFileExtension.isEmpty() ) {
        inputFileExtension = "." + inputFileExtension;
    }

    parameterInputFile = " " + inputFileParameter + "\"" + shellPlaceholder + "\"";

    if ( outputFileParameter != "none" && outputFileParameter != "stdout" ) {
        if ( outputFileName == inputFileName ) {
            parameterOuputFile = " " + outputFileParameter + "\"" + shellPlaceholder + "\"";
        }
        else {
            parameterOuputFile = " " + outputFileParameter + outputFileName + inputFileExtension;
        }
    }

	// If the config file name is empty it is assumed that all parameters are sent via command line call
	if ( configFilename.isEmpty() ) {
		parameterParameterFile = " " + parameterString;
	}
	// Else if needed add the parameter to the indenter call string where the config file can be found.
	else if (useCfgFileParameter != "none") {
#if defined(Q_OS_WIN32)
		parameterParameterFile = " " + useCfgFileParameter + "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + "/" + configFilename + "\"";
#else
        parameterParameterFile = " " + useCfgFileParameter + "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + configFilename + "\"";
#endif
	}

	// Assemble indenter call string for parameters according to the set order.
	if ( parameterOrder == "ipo" ) {
		indentCallString = parameterInputFile + parameterParameterFile + parameterOuputFile;
	} 
	else if ( parameterOrder == "pio" ) {
		indentCallString = parameterParameterFile + parameterInputFile + parameterOuputFile;
	}
    else if ( parameterOrder == "poi" ) {
		indentCallString = parameterParameterFile + parameterOuputFile + parameterInputFile;
	}
	else {
		indentCallString = parameterInputFile + parameterOuputFile + parameterParameterFile;
	}

    // Generate the indenter call string either for win32 or other systems.
#if defined(Q_OS_WIN32)
    indentCallString = "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + "/" + indenterFileName +".exe\""+ indentCallString;
#else
    indentCallString = "#!/bin/bash\n\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + indenterFileName + "\"" + indentCallString;
#endif

#ifndef Q_OS_WIN32
    // If no Linux binary exists to run the indenter, use wine to run the Windows exe and test if wine is installed.
    if ( !QFile::exists(dataDirctoryStr + indenterFileName) ) {
        indentCallString = "wine " + indentCallString;
    }
#endif

    // If the indenter writes to stdout pipe the output into a file
    if ( outputFileParameter == "stdout" ) {
        indentCallString = indentCallString + " >" + outputFileName + inputFileExtension;
    }

    // If the output filename is not the same as the input filename copy the output over the input.
    if ( outputFileName != inputFileName ) {
#if defined(Q_OS_WIN32)
        replaceInputFileCommand = "move /Y " + outputFileName + inputFileExtension + " \"" + shellPlaceholder + "\"\n";
#else
        replaceInputFileCommand = "mv " + outputFileName + inputFileExtension + " \"" + shellPlaceholder + "\"\n";
#endif
    }

    return indentCallString + "\n" + replaceInputFileCommand;
}


/*!
   \brief Format \a sourceCode by calling the indenter. 
   
   The \a inputFileExtension has to be given as parameter so the called indenter 
   can identify the programming language if needed.
 */
QString IndentHandler::callIndenter(QString sourceCode, QString inputFileExtension) {
    Q_ASSERT_X( !inputFileName.isEmpty(), "callIndenter", "inputFileName is empty" );
//    Q_ASSERT_X( !outputFileName.isEmpty(), "callIndenter", "outputFileName is empty" );
    Q_ASSERT_X( !indenterFileName.isEmpty(), "callIndenter", "indenterFileName is empty" );

	QString formattedSourceCode;
	bool wineInstalled = true;
	QString indentCallString;
	QString parameterInputFile;
	QString parameterOuputFile;
	QString parameterParameterFile;
	QProcess indentProcess;
	QString processReturnString;
	bool indenterExecutableExists = false;

    // generate the parameter string that will be save to the indenters config file
    QString parameterString = getParameterString();

	if ( !configFilename.isEmpty() ) {
		writeConfigFile( parameterString );
	}

    // only add point to file extension if the string is not empty
    if ( !inputFileExtension.isEmpty() ) {
        inputFileExtension = "." + inputFileExtension;
    }
	QFile::remove(dataDirctoryStr + inputFileName + inputFileExtension);
	QFile outSrcFile(dataDirctoryStr + inputFileName + inputFileExtension);
	parameterInputFile = " " + inputFileParameter + inputFileName + inputFileExtension;

    if ( outputFileParameter != "none" && outputFileParameter != "stdout" ) {
        parameterOuputFile = " " + outputFileParameter + outputFileName + inputFileExtension;
    }

	// If the config file name is empty it is assumed that all parameters are sent via command line call
	if ( configFilename.isEmpty() ) {
		parameterParameterFile = " " + parameterString;
	}
	// if needed add the parameter to the indenter call string where the config file can be found
	else if (useCfgFileParameter != "none") {
		parameterParameterFile = " " + useCfgFileParameter + "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + "/" + configFilename + "\"";
	}

	// Assemble indenter call string for parameters according to the set order.
	if ( parameterOrder == "ipo" ) {
		indentCallString = parameterInputFile + parameterParameterFile + parameterOuputFile;
	} 
	else if ( parameterOrder == "pio" ) {
		indentCallString = parameterParameterFile + parameterInputFile + parameterOuputFile;
	}
    else if ( parameterOrder == "poi" ) {
		indentCallString = parameterParameterFile + parameterOuputFile + parameterInputFile;
	}
	else {
		indentCallString = parameterInputFile + parameterOuputFile + parameterParameterFile;
	}

    // Test if the indenter executable exists. If not show a dialog box once and return
    // the unformatted source code. Else continue calling the indenter.
#if defined(Q_OS_WIN32)
    indenterExecutableExists = QFile::exists(dataDirctoryStr + indenterFileName+".exe");
#else
    indenterExecutableExists = QFile::exists(dataDirctoryStr + indenterFileName) || QFile::exists(dataDirctoryStr + indenterFileName+".exe");
#endif
    if ( !indenterExecutableExists ) {
        if ( !noIndenterExecExistDialogAlreadyShown ) {
            QMessageBox::warning(NULL, tr("No indenter executable"), tr("There exists no indenter executable with the name \"")
                +indenterFileName+ tr("\" in the directory \"") +dataDirctoryStr+"\".");
            noIndenterExecExistDialogAlreadyShown = true;
        }
        return sourceCode;
    }

    // Generate the indenter call string either for win32 or other systems.
#if defined(Q_OS_WIN32)
    indentCallString = "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + "/" + indenterFileName +".exe\""+ indentCallString;
#else
    indentCallString = "\"" + QFileInfo(dataDirctoryStr).absoluteFilePath() + "/" + indenterFileName + "\"" + indentCallString;
#endif

    // write the source code to the input file for the indenter
    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    outSrcFile.write( sourceCode.toUtf8() );
    outSrcFile.close();

    // errors and standard outputs from the process call are merged together
    indentProcess.setReadChannelMode(QProcess::MergedChannels);

#ifndef Q_OS_WIN32
    // if no linux binary exists to run the indenter, use wine to run the windows exe and test if wine is installed
    if ( !QFile::exists(dataDirctoryStr + indenterFileName) ) {
        QProcess wineTestProcess;
        wineTestProcess.start("wine --version");
        // if the process of wine was not callable assume that wine is not installed
        if ( !wineTestProcess.waitForFinished() ) {
            wineInstalled = false;
        }
        indentCallString = "wine " + indentCallString;
    }
#endif

    if ( !wineInstalled ) {
        QApplication::restoreOverrideCursor();
        QMessageBox::warning(NULL, tr("wine not installed"), tr("There exists only a win32 executable of the indenter and wine does not seem to be installed. Please install wine to be able to run the indenter.") );
    }
    else {
        // set the directory for the indenter execution
        indentProcess.setWorkingDirectory( QFileInfo(dataDirctoryStr).absoluteFilePath() );

        indentProcess.start(indentCallString);

        processReturnString = "<html><body>";
        // test if there was an error during starting the process of the indenter, or in case of linux
        // there might also been an error starting wine
        if ( !indentProcess.waitForFinished(5000) ) {
            processReturnString += tr("<b>Returned error message:</b> ") + indentProcess.errorString() + "<br>";

            switch ( indentProcess.error() ) {
                case QProcess::FailedToStart :
                    processReturnString += tr("<b>Reason could be:</b> ") + "The process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.<br>";
                    break;
                case QProcess::Crashed :
                    processReturnString += "The process crashed some time after starting successfully.<br>";
                    break;
                case QProcess::Timedout :
                    processReturnString += "The last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.<br>";
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
            processReturnString += tr("<b>Callstring was:</b> ") + indentCallString + "</html></body>";
            QApplication::restoreOverrideCursor();
            QMessageBox::warning(NULL, tr("Error calling Indenter"), processReturnString);
        }
        // there was no problem starting the process/indenter so fetch, what it returned
        else {
            processReturnString += indentProcess.readAll();
        }

        // if the indenter returned an errorcode != 0 show its output
        if ( indentProcess.exitCode() != 0 ) {
            QString exitCode;
            exitCode.setNum(indentProcess.exitCode());
            processReturnString = tr("<b>Indenter returned with exit code:</b> ") + exitCode + "<br>" +
                                tr("<b>Indent console output was:</b> ") + processReturnString + "<br>" +
                                tr("<b>Callstring was:</b> ") + indentCallString + "</html></body>";
            QApplication::restoreOverrideCursor();
            //QMessageBox::warning(NULL, tr("Indenter returned error"), processReturnString);
            errorMessageDialog->showMessage(processReturnString);
        }
    }


    // If the indenter results are written to stdout, read them from there.
	if ( indentProcess.exitCode() == 0 && outputFileParameter == "stdout"  ) {
		formattedSourceCode = processReturnString;
	}
    // Else read the output file generated by the indenter call.
	else {
		outSrcFile.setFileName(dataDirctoryStr + outputFileName + inputFileExtension);
		outSrcFile.open(QFile::ReadOnly | QFile::Text);
		QTextStream outSrcStrm(&outSrcFile);
		outSrcStrm.setCodec( QTextCodec::codecForName("UTF-8") );
		formattedSourceCode = outSrcStrm.readAll();
		outSrcFile.close();
	}

    // Delete the temporary input and output files.
    QFile::remove(dataDirctoryStr + outputFileName + inputFileExtension);
    QFile::remove(dataDirctoryStr + inputFileName + inputFileExtension);

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
            indenterSettings->setValue( pBoolean.paramName + "/Value", 1);
        }
        else {
            if ( !pBoolean.falseString.isEmpty() ) {
                parameterString += pBoolean.falseString + cfgFileParameterEnding;
            }
            indenterSettings->setValue( pBoolean.paramName + "/Value", 0);
        }
    }

    // generate parameter string for all numeric values
    foreach (ParamNumeric pNumeric, paramNumerics) {
        if ( pNumeric.valueEnabledChkBox->isChecked() ) {
            parameterString += pNumeric.paramCallName + QString::number( pNumeric.spinBox->value() ) + cfgFileParameterEnding;
        }
        indenterSettings->setValue( pNumeric.paramName + "/Value", pNumeric.spinBox->value() );
        indenterSettings->setValue( pNumeric.paramName + "/Enabled", pNumeric.valueEnabledChkBox->isChecked() );
    }

    // generate parameter string for all string values
    foreach (ParamString pString, paramStrings) {
        if ( !pString.lineEdit->text().isEmpty() && pString.valueEnabledChkBox->isChecked() ) {
            // Create parameter definition for each value devided by a | sign.
            foreach (QString paramValue, pString.lineEdit->text().split("|")) {
                parameterString += pString.paramCallName + paramValue + cfgFileParameterEnding;
            }
        }
        indenterSettings->setValue( pString.paramName + "/Value", pString.lineEdit->text() );
        indenterSettings->setValue( pString.paramName + "/Enabled", pString.valueEnabledChkBox->isChecked() );
    }

    // generate parameter string for all multiple choice values
    foreach (ParamMultiple pMultiple, paramMultiples) {
        if ( pMultiple.valueEnabledChkBox->isChecked() ) {
            parameterString += pMultiple.choicesStrings.at( pMultiple.comboBox->currentIndex () ) + cfgFileParameterEnding;
        }
        indenterSettings->setValue( pMultiple.paramName + "/Value", pMultiple.comboBox->currentIndex () );
        indenterSettings->setValue( pMultiple.paramName + "/Enabled", pMultiple.valueEnabledChkBox->isChecked() );
    }

    return parameterString;
}


/*!
    \brief Write settings for the indenter to a config file.
 */
void IndentHandler::writeConfigFile(QString paramString) {
    Q_ASSERT_X( !configFilename.isEmpty(), "writeConfigFile", "configFilename is empty" );

    QFile::remove( dataDirctoryStr + configFilename );
    QFile cfgFile( dataDirctoryStr + configFilename );

    cfgFile.open( QFile::ReadWrite | QFile::Text );
    cfgFile.write( paramString.toAscii() );
    cfgFile.close();
}


/*!
    \brief Load the config file for the indenter and apply the settings made there.
 */
void IndentHandler::loadConfigFile(QString filePathName) {
    Q_ASSERT_X( !filePathName.isEmpty(), "loadConfigFile", "filePathName is empty" );

    QFile cfgFile(filePathName);
    int index;
    int crPos;
    int paramValue = 0;
    QString paramValueStr;

    // open the config file and read all data
    cfgFile.open( QFile::ReadOnly | QFile::Text );
    cfgFileData = cfgFile.readAll();
    cfgFile.close();

    // search for name of each boolean parameter and set its value if found
    foreach (ParamBoolean pBoolean, paramBooleans) {

        // boolean value that will be assigned to the checkbox
        bool paramValue = false;

        // first search for the longer parameter string
        // the true parameter string is longer than the false string
        if ( pBoolean.trueString.length() > pBoolean.falseString.length() ) {
            // search for the true string
            index = cfgFileData.indexOf( pBoolean.trueString, 0 );
            // if true string found set the parameter value to true
            if ( index != -1 ) {
                paramValue = true;
            }
            // if true string not found, search for false string
            else {
                index = cfgFileData.indexOf( pBoolean.falseString, 0 );
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
            index = cfgFileData.indexOf( pBoolean.falseString, 0 );
            // if false string found set the parameter value to false
            if ( index != -1 ) {
                paramValue = false;
            }
            // if false string not found, search for true string
            else {
                index = cfgFileData.indexOf( pBoolean.trueString, 0 );
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

    // search for name of each numeric parameter and set the value found behind it
    foreach (ParamNumeric pNumeric, paramNumerics) {
        index = cfgFileData.indexOf( pNumeric.paramCallName, 0 );
        // parameter was found in config file
        if ( index != -1 ) {
            // set index after the parameter name, so in front of the number
            index += pNumeric.paramCallName.length();

            // find the line end by searching for carriage return
            crPos = cfgFileData.indexOf( cfgFileParameterEnding, index+1 );

            // get the number and convert it to int
            QString test = cfgFileData.mid( index, crPos - index );
            paramValue = cfgFileData.mid( index, crPos - index ).toInt(NULL);

            // disable the signal-slot connection. Otherwise signal is emmitted each time when value is set
            QObject::disconnect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(indenterSettingsChanged()));
            pNumeric.spinBox->setValue( paramValue );
            QObject::connect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(indenterSettingsChanged()));
        }
        // parameter was not found in config file
        else {
            int defaultValue = indenterSettings->value(pNumeric.paramName + "/ValueDefault").toInt();
            // a value of -1 means that this parameter is disabled
            if ( defaultValue == -1 ) {
                pNumeric.valueEnabledChkBox->setChecked( false );
            }
            // if not disabled use the given default value
            else {
                pNumeric.spinBox->setValue( defaultValue );
            }
        }
    }

    // search for name of each string parameter and set it
    foreach (ParamString pString, paramStrings) {
        paramValueStr = "";
        // The number of the found values for this parameter name.
        int numberOfValues = 0;
        index = cfgFileData.indexOf( pString.paramCallName, 0 );
        // parameter was found in config file
        if ( index != -1 ) {
            while ( index != -1 ) {
                numberOfValues++;

                // set index after the parameter name, so in front of the string
                index += pString.paramCallName.length();

                // find the line end by searching for carriage return
                crPos = cfgFileData.indexOf( cfgFileParameterEnding, index+1 );

                // Get the string and eventually add it to the line edit.
                if ( numberOfValues < 2 ) {
                    paramValueStr = QString( cfgFileData.mid( index, crPos - index ) );
                }
                else {
                    paramValueStr = paramValueStr + "|" + QString( cfgFileData.mid( index, crPos - index ) );
                }
                
                // Get next value for this setting, if one exists.
                index = cfgFileData.indexOf( pString.paramCallName, crPos+1 );
            }
            // Set the text for the line edit.
            pString.lineEdit->setText( paramValueStr );
        }
        // parameter was not found in config file
        else {
            paramValueStr = indenterSettings->value(pString.paramName + "/ValueDefault").toString();
            // a value of -1 means that this parameter is disabled
            if ( paramValueStr == "-1" ) {
                pString.valueEnabledChkBox->setChecked( false );
            }
            // if not disabled use the given default value
            else {
                pString.lineEdit->setText( paramValueStr );
            }
        }
    }

    // search for name of each multiple choice parameter and set it
    foreach (ParamMultiple pMultiple, paramMultiples) {
        int i = 0;
        index = -1;

        // search for all parameter names of the multiple choice list
        // if one is found, set it and leave the while loop
        while ( i < pMultiple.choicesStrings.count() && index == -1 ) {
            index = cfgFileData.indexOf( pMultiple.choicesStrings.at(i), 0 );
            if ( index != -1 ) {
                pMultiple.comboBox->setCurrentIndex( i );
            }
            i++;
        }

        // parameter was not set in config file, so use default value
        if ( index == -1 ) {
            int defaultValue = indenterSettings->value(pMultiple.paramName + "/ValueDefault").toInt();
            // a value of -1 means that this parameter is disabled
            if ( defaultValue == -1 ) {
                pMultiple.valueEnabledChkBox->setChecked( false );
            }
            // if not disabled use the given default value
            else {
                pMultiple.comboBox->setCurrentIndex( defaultValue );
            }
        }
    }
}

/*!
    \brief Opens and parses the indenter ini file that is declared by \a iniFilePath.
 */
void IndentHandler::readIndentIniFile(QString iniFilePath) {
    Q_ASSERT_X( !iniFilePath.isEmpty(), "readIndentIniFile", "iniFilePath is empty" );

    // open the ini-file that contains all available indenter settings with their additional infos
    indenterSettings = new QSettings(iniFilePath, QSettings::IniFormat, this);

    QStringList categories;
    //QString indenterGroupString = "";
    QString paramToolTip = "";


    //
    //  parse ini file indenter header
    //

    indenterName = indenterSettings->value(" header/indenterName").toString();
    indenterFileName = indenterSettings->value(" header/indenterFileName").toString();
    configFilename = indenterSettings->value(" header/configFilename").toString();
    useCfgFileParameter = indenterSettings->value(" header/useCfgFileParameter").toString();
    cfgFileParameterEnding = indenterSettings->value(" header/cfgFileParameterEnding").toString();
    if ( cfgFileParameterEnding == "cr" ) {
        cfgFileParameterEnding = "\n";
    }

	// Read the parameter order. Possible values are (p=parameter[file] i=inputfile o=outputfile)
	// pio, ipo, iop
	parameterOrder = indenterSettings->value(" header/parameterOrder", "pio").toString();
    inputFileParameter = indenterSettings->value(" header/inputFileParameter").toString();
    inputFileName = indenterSettings->value(" header/inputFileName").toString();
    outputFileParameter = indenterSettings->value(" header/outputFileParameter").toString();
    outputFileName = indenterSettings->value(" header/outputFileName").toString();
    fileTypes = indenterSettings->value(" header/fileTypes").toString();
    fileTypes.replace('|', " ");

    // read the categories names which are separated by "|"
    QString categoriesStr = indenterSettings->value(" header/categories").toString();
    categories = categoriesStr.split("|");
	// Assure that the category list is never empty. At least contain a "general" section.
	if ( categories.isEmpty() ) {
		categories.append("General");
	}

    ToolBoxPage toolBoxPage;

    // create a page for each category and store its references in a toolboxpage-array
    foreach (QString category, categories) {
        //QString categoryName = indenterSettings->value("Categories/" + category).toString();
        QString categoryName = category;

        toolBoxPage.page = new QWidget();
        toolBoxPage.page->setObjectName(categoryName);
        toolBoxPage.page->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::MinimumExpanding);
        toolBoxPage.vboxLayout = new QVBoxLayout(toolBoxPage.page);
        toolBoxPage.vboxLayout->setSpacing(6);
        toolBoxPage.vboxLayout->setMargin(9);
        toolBoxPage.vboxLayout->setObjectName(categoryName);
        toolBoxPages.append(toolBoxPage);
        toolBox->addItem(toolBoxPage.page, QApplication::translate("IndentHandler", categoryName.toAscii(), 0, QApplication::UnicodeUTF8));
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
        if ( indenterParameter != " header") {
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
                spinBox->setValue( indenterSettings->value(indenterParameter + "/Value").toInt() );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                spinBox->setToolTip( paramToolTip );
                spinBox->setMaximumWidth(50);
                spinBox->setMinimumWidth(50);
                spinBox->installEventFilter( mainWindow );
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
                label->installEventFilter( mainWindow );

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
                paramNumerics.append(paramNumeric);

                QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(indenterSettingsChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(indenterSettingsChanged()));
            }
            // edit type is boolean so create a checkbox
            else if ( editType == "boolean" ) {
                // create the checkbox, make its settings and add it to the toolbox page
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setText(indenterParameter);
                chkBox->setChecked( indenterSettings->value(indenterParameter + "/Value").toBool() );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                chkBox->setToolTip( paramToolTip );
                chkBox->installEventFilter( mainWindow );
                toolBoxPages.at(category).vboxLayout->addWidget(chkBox);

                // remember parameter name and reference to its checkbox
                ParamBoolean paramBoolean;
                paramBoolean.paramName = indenterParameter;
                paramBoolean.checkBox = chkBox;
                QStringList trueFalseStrings = indenterSettings->value(indenterParameter + "/TrueFalse").toString().split("|");
                paramBoolean.trueString = trueFalseStrings.at(0);
                paramBoolean.falseString = trueFalseStrings.at(1);
                paramBooleans.append(paramBoolean);

                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(indenterSettingsChanged()));
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
                lineEdit->setText( indenterSettings->value(indenterParameter + "/Value").toString() );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                lineEdit->setToolTip( paramToolTip );
                lineEdit->setMaximumWidth(50);
                lineEdit->setMinimumWidth(50);
                lineEdit->installEventFilter( mainWindow );

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(indenterParameter);
                label->setBuddy(lineEdit);
                label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
                label->setToolTip( paramToolTip );
                label->installEventFilter( mainWindow );

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
                paramStrings.append(paramString);

                QObject::connect(lineEdit, SIGNAL(editingFinished()), this, SIGNAL(indenterSettingsChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(indenterSettingsChanged()));
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
                comboBox->setCurrentIndex( indenterSettings->value(indenterParameter + "/Value").toInt() );
                paramToolTip = indenterSettings->value(indenterParameter + "/Description").toString();
                comboBox->setToolTip( paramToolTip );
                comboBox->installEventFilter( mainWindow );

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
                paramMultiples.append(paramMultiple);

                QObject::connect(comboBox, SIGNAL(activated(int)), this, SIGNAL(indenterSettingsChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(indenterSettingsChanged()));
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
        QFile file(dataDirctoryStr + indenterIniFile);
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
    // TODO: This function is never called because out of the mainwindow object a new indenthandler object is created if another indenter is selected.

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
    delete indenterSettings;

    readIndentIniFile( dataDirctoryStr + indenterIniFileList.at(indenterID) );
}


/*!
    \brief Returns a string containing by the indenter supported file types/extensions devided by a space.
 */
QString IndentHandler::getPossibleIndenterFileExtensions() {
    return fileTypes;
}

/*!
    \brief Returns the path and filename of the current indenter config file.
 */
QString IndentHandler::getIndenterCfgFile() {
    QFileInfo fileInfo( dataDirctoryStr + configFilename );
    return fileInfo.absoluteFilePath();
}
