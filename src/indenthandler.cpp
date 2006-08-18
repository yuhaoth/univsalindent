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

#include "indenthandler.h"


/*!
    Constructor of the GreatCode handler. This is a widget that is
    loaded into the main window if GreatCode is selected as indenter.
    Handles GreatCode config file read/write and calls GreatCode
    to reformat source text.
    Calls GreatCode each time a setting has been changed and informs
    the main window about the reformatted source code.
 */
IndentHandler::IndentHandler(QString dataDirPathStr, QWidget *parent)
    : QWidget(parent)
{
    // define this widgets size and resize behavior
    this->setMaximumWidth(263);
    this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

    // create vertical layout box, into which the toolbox will be added
	vboxLayout = new QVBoxLayout(this);

    // create a toolbox and set its resize behavior
    toolBox = new QToolBox(this);
	toolBox->setObjectName(QString::fromUtf8("toolBox"));
	toolBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
    toolBox->setMaximumSize(QSize(16777215, 16777215));
    // insert the toolbox into the vlayout
	vboxLayout->addWidget(toolBox);

    dataDirctoryStr = dataDirPathStr;
    QDir dataDirctory = QDir(dataDirPathStr);

    indenterIniFileList = dataDirctory.entryList( QStringList("uigui_*.ini") );
    noIndenterExecExistDialogAlreadyShown = false;

    // reads and parses first found indent ini file and creates toolbox entries
    readIndentIniFile( dataDirctoryStr + indenterIniFileList.first() );
}


/*!
    Constructor of the GreatCode handler. This is a widget that is
    loaded into the main window if GreatCode is selected as indenter.
    Handles GreatCode config file read/write and calls GreatCode
    to reformat source text.
    Calls GreatCode each time a setting has been changed and informs
    the main window about the reformatted source code.
 */
IndentHandler::IndentHandler(QString dataDirPathStr, int indenterID, QWidget *parent)
    : QWidget(parent)
{
    Q_ASSERT_X( indenterID >= 0, "IndentHandler", "the selected indenterID is < 0" );

	// define this widgets size and resize behavior
	this->setMaximumWidth(263);
	this->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );

	// create vertical layout box, into which the toolbox will be added
	vboxLayout = new QVBoxLayout(this);

	// create a toolbox and set its resize behavior
	toolBox = new QToolBox(this);
	toolBox->setObjectName(QString::fromUtf8("toolBox"));
	toolBox->setSizePolicy( QSizePolicy::Expanding, QSizePolicy::Expanding );
	toolBox->setMaximumSize(QSize(16777215, 16777215));
	// insert the toolbox into the vlayout
	vboxLayout->addWidget(toolBox);

	dataDirctoryStr = dataDirPathStr;
	QDir dataDirctory = QDir(dataDirPathStr);

	indenterIniFileList = dataDirctory.entryList( QStringList("uigui_*.ini") );
    noIndenterExecExistDialogAlreadyShown = false;

	// reads and parses first found indent ini file and creates toolbox entries
	readIndentIniFile( dataDirctoryStr + indenterIniFileList.at(indenterID) );
}


/*! 
   Format \a sourceCode by calling the indenter. The \a inputFileExtension has to be given as parameter
   so the called indenter can identify the programming language if needed.
 */
QString IndentHandler::callIndenter(QString sourceCode, QString inputFileExtension) {
    Q_ASSERT_X( !inputFileName.isEmpty(), "callIndenter", "inputFileName is empty" );
    Q_ASSERT_X( !outputFileName.isEmpty(), "callIndenter", "outputFileName is empty" );
    Q_ASSERT_X( !indenterFileName.isEmpty(), "callIndenter", "indenterFileName is empty" );

    // generate the parameter string that will be save to the indenters config file
    QString parameterString = getParameterString();
    writeConfigFile( parameterString );

    QString formattedSourceCode;

    // only add point to file extension if the string is not empty
    if ( !inputFileExtension.isEmpty() ) {
        inputFileExtension = "." + inputFileExtension;
    }

    QFile::remove(dataDirctoryStr + inputFileName + inputFileExtension);
    QFile outSrcFile(dataDirctoryStr + inputFileName + inputFileExtension);
	QString indentCallString = inputFileParameter + inputFileName + inputFileExtension;
    if ( outputFileParameter != "none" ) {
        indentCallString += " "+ outputFileParameter + outputFileName + inputFileExtension;
    }

    QProcess indentProcess;
    QString processReturnString;

    // Test if the indenter executable exists. If not show a dialog box once and return
    // the unformatted source code. Else continue calling the indenter.
    bool indenterExecutableExists = false; 
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

    // generate the indenter call string either for win32 or other systems
#if defined(Q_OS_WIN32)
	indentCallString = dataDirctoryStr + indenterFileName +".exe "+ indentCallString;
#else
	indentCallString = "./" + indenterFileName +" "+ indentCallString;
#endif

    // if needed add the parameter to the indenter call string where the config file can be found
    if ( !useCfgFileParameter.isEmpty() ) {
        indentCallString += " "+ useCfgFileParameter + configFilename;
    }

    // write the source code to the input file for the indenter
    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    outSrcFile.write( sourceCode.toAscii() );
    outSrcFile.close();
    
    // errors and standard outputs from the process call are merged together
    indentProcess.setReadChannelMode(QProcess::MergedChannels);

#if defined(Q_OS_LINUX)
	// if no linux binary exists to run the indenter, use wine to run the windows exe
	if ( !QFile::exists(dataDirctoryStr + indenterFileName) ) {
		indentCallString = "wine " + indentCallString;
	}
#endif

    // set the directory for the indenter execution
    indentProcess.setWorkingDirectory( QFileInfo(dataDirctoryStr).absoluteFilePath() );

    indentProcess.start(indentCallString);

    // test if there was an error during starting the process of the indenter, or in case of linux
    // there might also been an error starting wine
    if ( !indentProcess.waitForFinished() ) {
        processReturnString = indentProcess.errorString();

        switch ( indentProcess.error() ) {
            case QProcess::FailedToStart : 
                processReturnString += "\nThe process failed to start. Either the invoked program is missing, or you may have insufficient permissions to invoke the program.";
                break;
            case QProcess::Crashed : 
                processReturnString += "\nThe process crashed some time after starting successfully.";
                break;
            case QProcess::Timedout : 
                processReturnString += "\nThe last waitFor...() function timed out. The state of QProcess is unchanged, and you can try calling waitFor...() again.";
                break;
            case QProcess::WriteError : 
                processReturnString += "\nAn error occurred when attempting to write to the process. For example, the process may not be running, or it may have closed its input channel.";
                break;
            case QProcess::ReadError : 
                processReturnString += "\nAn error occurred when attempting to read from the process. For example, the process may not be running.";
                break;
            case QProcess::UnknownError : 
                processReturnString += "\nAn unknown error occurred. This is the default return value of error().";
                break;
            default :
                break;
        }
        QMessageBox::warning(NULL, tr("Error calling Indenter"), processReturnString
            +tr("\nCallstring was: ")+indentCallString);
    }
    // there was no problem starting  the process/indenter so fetch, what it returned
    else {
        processReturnString = indentProcess.readAll();
    }

    // if the indenter returned an errorcode != 0 show its output
    if ( indentProcess.exitCode() != 0 ) {
        QString exitCode;
        exitCode.setNum(indentProcess.exitCode());
        QMessageBox::warning(NULL, tr("Indenter returned error"), tr("Indenter returned with exit code ")+exitCode
            +tr(".\nIndent console output was: \n")+processReturnString
            +tr("\nCallstring was: ")+indentCallString);
    }

    // read the output file generated by the indenter call
    outSrcFile.setFileName(dataDirctoryStr + outputFileName + inputFileExtension);
    outSrcFile.open(QFile::ReadOnly | QFile::Text);
    formattedSourceCode = outSrcFile.readAll();
    outSrcFile.close();

    // delete the temporary input and output files
    QFile::remove(dataDirctoryStr + outputFileName + inputFileExtension);
    QFile::remove(dataDirctoryStr + inputFileName + inputFileExtension);

    return formattedSourceCode;
}

//! Generates and returns a string with all parameters needed to call GreatCode
QString IndentHandler::getParameterString() {

    QString parameterString = "";

    // generate parameter string for all boolean values
    foreach (ParamBoolean pBoolean, paramBooleans) {
        if ( pBoolean.checkBox->isChecked() ) {
            if ( !pBoolean.trueString.isEmpty() ) {
                parameterString += pBoolean.trueString + cfgFileParameterEnding;
            }
            gcSettings->setValue( pBoolean.paramName + "/Value", 1);
        }
        else {
            if ( !pBoolean.falseString.isEmpty() ) {
                parameterString += pBoolean.falseString + cfgFileParameterEnding;
            }
            gcSettings->setValue( pBoolean.paramName + "/Value", 0);
        }
    }

    // generate parameter string for all numeric values
    foreach (ParamNumeric pNumeric, paramNumerics) {
        if ( pNumeric.valueEnabledChkBox->isChecked() ) {
            parameterString += pNumeric.paramCallName + QString::number( pNumeric.spinBox->value() ) + cfgFileParameterEnding;
        }
        gcSettings->setValue( pNumeric.paramName + "/Value", pNumeric.spinBox->value() );
        gcSettings->setValue( pNumeric.paramName + "/Enabled", pNumeric.valueEnabledChkBox->isChecked() );
    }

    // generate parameter string for all string values
    foreach (ParamString pString, paramStrings) {
        if ( pString.lineEdit->text() != "" && pString.valueEnabledChkBox->isChecked() ) {
            parameterString += pString.paramCallName + pString.lineEdit->text() + cfgFileParameterEnding;
        }
        gcSettings->setValue( pString.paramName + "/Value", pString.lineEdit->text() );
        gcSettings->setValue( pString.paramName + "/Enabled", pString.valueEnabledChkBox->isChecked() );
    }

    // generate parameter string for all multiple choice values
    foreach (ParamMultiple pMultiple, paramMultiples) {
        if ( pMultiple.valueEnabledChkBox->isChecked() ) {
            parameterString += pMultiple.choicesStrings.at( pMultiple.comboBox->currentIndex () ) + cfgFileParameterEnding;
        }
        gcSettings->setValue( pMultiple.paramName + "/Value", pMultiple.comboBox->currentIndex () );
        gcSettings->setValue( pMultiple.paramName + "/Enabled", pMultiple.valueEnabledChkBox->isChecked() );
    }
    
    return parameterString;
}


//! Write settings for the indenter to a config file
void IndentHandler::writeConfigFile(QString paramString) {
    Q_ASSERT_X( !configFilename.isEmpty(), "writeConfigFile", "configFilename is empty" );

    QFile::remove( dataDirctoryStr + configFilename );
    QFile cfgFile( dataDirctoryStr + configFilename );

    cfgFile.open( QFile::ReadWrite | QFile::Text );
    cfgFile.write( paramString.toAscii() );
    cfgFile.close();
}

//! Load a GC config file
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
					paramValue = gcSettings->value(pBoolean.paramName + "/ValueDefault").toBool();
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
					paramValue = gcSettings->value(pBoolean.paramName + "/ValueDefault").toBool();
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
            QObject::disconnect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsCodeChanged()));
            pNumeric.spinBox->setValue( paramValue );
            QObject::connect(pNumeric.spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsCodeChanged()));
		}
		// parameter was not found in config file
		else {
			int defaultValue = gcSettings->value(pNumeric.paramName + "/ValueDefault").toInt();
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
		index = cfgFileData.indexOf( pString.paramCallName, 0 );
		// parameter was found in config file
		if ( index != -1 ) {
            // set index after the parameter name, so in front of the string
            index += pString.paramCallName.length();

            // find the line end by searching for carriage return
            crPos = cfgFileData.indexOf( cfgFileParameterEnding, index+1 );

            // get the number and convert it to int
            paramValueStr = QString( cfgFileData.mid( index, crPos - index ) );
            pString.lineEdit->setText( paramValueStr );
		}
		// parameter was not found in config file
		else {
			QString defaultValue = gcSettings->value(pString.paramName + "/ValueDefault").toString();
			// a value of -1 means that this parameter is disabled
			if ( defaultValue == "-1" ) {
				pString.valueEnabledChkBox->setChecked( false );
			}
			// if not disabled use the given default value
			else {
				pString.lineEdit->setText( defaultValue );
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
			int defaultValue = gcSettings->value(pMultiple.paramName + "/ValueDefault").toInt();
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
    \brief opens and parses an indenter ini file, handed as parameter
 */
void IndentHandler::readIndentIniFile(QString iniFilePath) {
    Q_ASSERT_X( !iniFilePath.isEmpty(), "readIndentIniFile", "iniFilePath is empty" );

    // open the ini-file that contains all available GreatCode settings with their additional infos
    gcSettings = new QSettings(iniFilePath, QSettings::IniFormat, this);
    
	QStringList categories;
    QString gcGroupString = "";
    QString paramToolTip = "";


    //
    //  parse ini file indenter header
    //

    indenterName = gcSettings->value(" header/indenterName").toString();
    indenterFileName = gcSettings->value(" header/indenterFileName").toString();
    configFilename = gcSettings->value(" header/configFilename").toString();
    useCfgFileParameter = gcSettings->value(" header/useCfgFileParameter").toString();
    cfgFileParameterEnding = gcSettings->value(" header/cfgFileParameterEnding").toString();
    if ( cfgFileParameterEnding == "cr" ) {
        cfgFileParameterEnding = "\n";
    }

    inputFileParameter = gcSettings->value(" header/inputFileParameter").toString();
    inputFileName = gcSettings->value(" header/inputFileName").toString();
    outputFileParameter = gcSettings->value(" header/outputFileParameter").toString();
	outputFileName = gcSettings->value(" header/outputFileName").toString();
    fileTypes = gcSettings->value(" header/fileTypes").toString();
    fileTypes.replace('|', " ");

    // read the categories names which are separated by "|"
    QString categoriesStr = gcSettings->value(" header/categories").toString();
    categories = categoriesStr.split("|");

    ToolBoxPage toolBoxPage;

    // create a page for each category and store its references in a toolboxpage-array
    foreach (QString category, categories) {
        //QString categoryName = gcSettings->value("Categories/" + category).toString();
        QString categoryName = category;

        toolBoxPage.page = new QWidget();
        toolBoxPage.page->setObjectName(categoryName);
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
    gcParamters = gcSettings->childGroups();

    // read each parameter to create the corresponding input field
    foreach (QString gcParameter, gcParamters) {

        // if it is not the indent header definition read the parameter and add it to
        // the corresponding category toolbox page
		if ( gcParameter != " header") {
            // read to which category the parameter belongs
            int category = gcSettings->value(gcParameter + "/Category").toInt();
            // read which type of input field the parameter needs
            QString editType = gcSettings->value(gcParameter + "/EditorType").toString();

            // edit type is numeric so create a spinbox with label
            if ( editType == "numeric" ) {
				// read the parameter name as it is used at the command line or in its config file
				QString parameterCallName = gcSettings->value(gcParameter + "/CallName").toString();

                // create checkbox which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( gcSettings->value(gcParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the spinbox
                QSpinBox *spinBox = new QSpinBox( toolBoxPages.at(category).page );
                spinBox->setValue( gcSettings->value(gcParameter + "/Value").toInt() );
                paramToolTip = gcSettings->value(gcParameter + "/Description").toString();
                spinBox->setToolTip( paramToolTip );
                spinBox->setMaximumWidth(50);
                spinBox->setMinimumWidth(50);
				if ( gcSettings->value(gcParameter + "/MinVal").toString() != "" ) {
					spinBox->setMinimum( gcSettings->value(gcParameter + "/MinVal").toInt() );
				}
				else {
                    spinBox->setMinimum( 0 );
				}
                if ( gcSettings->value(gcParameter + "/MaxVal").toString() != "" ) {
					spinBox->setMaximum( gcSettings->value(gcParameter + "/MaxVal").toInt() );
				}
				else {
                    spinBox->setMaximum( 2000 );
				}

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(gcParameter);
                label->setBuddy(spinBox);
                label->setToolTip( paramToolTip );

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(spinBox);
                hboxLayout->addWidget(label);                
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its spinbox
                ParamNumeric paramNumeric;
                paramNumeric.paramName = gcParameter;
				paramNumeric.paramCallName = parameterCallName;
                paramNumeric.spinBox = spinBox;
                paramNumeric.label = label;
                paramNumeric.valueEnabledChkBox = chkBox;
                paramNumerics.append(paramNumeric);

                QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SIGNAL(settingsCodeChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(settingsCodeChanged()));
            }
            // edit type is boolean so create a checkbox
            else if ( editType == "boolean" ) {
                // create the checkbox, make its settings and add it to the toolbox page
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setText(gcParameter);
                chkBox->setChecked( gcSettings->value(gcParameter + "/Value").toBool() );
				paramToolTip = gcSettings->value(gcParameter + "/Description").toString();
				chkBox->setToolTip( paramToolTip );
				toolBoxPages.at(category).vboxLayout->addWidget(chkBox);

                // remember parameter name and reference to its checkbox
                ParamBoolean paramBoolean;
                paramBoolean.paramName = gcParameter;
                paramBoolean.checkBox = chkBox;
				QStringList trueFalseStrings = gcSettings->value(gcParameter + "/TrueFalse").toString().split("|");
				paramBoolean.trueString = trueFalseStrings.at(0);
				paramBoolean.falseString = trueFalseStrings.at(1);
                paramBooleans.append(paramBoolean);

                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(settingsCodeChanged()));
            }
            // edit type is numeric so create a lineedit with label
            else if ( editType == "string" ) {
				// read the parameter name as it is used at the command line or in its config file
				QString parameterCallName = gcSettings->value(gcParameter + "/CallName").toString();

                // create checkbox which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( gcSettings->value(gcParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the line edit
                QLineEdit *lineEdit = new QLineEdit( toolBoxPages.at(category).page );
                lineEdit->setText( gcSettings->value(gcParameter + "/Value").toString() );
                paramToolTip = gcSettings->value(gcParameter + "/Description").toString();
                lineEdit->setToolTip( paramToolTip );
				lineEdit->setMaximumWidth(50);
				lineEdit->setMinimumWidth(50);

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(gcParameter);
				label->setBuddy(lineEdit);
				label->setSizePolicy(QSizePolicy::MinimumExpanding, QSizePolicy::Preferred);
                label->setToolTip( paramToolTip );

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(lineEdit);
                hboxLayout->addWidget(label);  
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its lineedit
                ParamString paramString;
                paramString.paramName = gcParameter;
				paramString.paramCallName = parameterCallName;
                paramString.lineEdit = lineEdit;
                paramString.label = label;
                paramString.valueEnabledChkBox = chkBox;
                paramStrings.append(paramString);

                QObject::connect(lineEdit, SIGNAL(editingFinished()), this, SIGNAL(settingsCodeChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(settingsCodeChanged()));
            }
            // edit type is multiple so create a combobox with label
            else if ( editType == "multiple" ) {
				// read the parameter name as it is used at the command line or in its config file
				QString parameterCallName = gcSettings->value(gcParameter + "/CallName").toString();

                // create checkbox which enables or disables the parameter
                QCheckBox *chkBox = new QCheckBox( toolBoxPages.at(category).page );
                chkBox->setChecked( gcSettings->value(gcParameter + "/Enabled").toBool() );
                chkBox->setToolTip( "Enables/disables the parameter. If disabled the indenters default value will be used." );
                chkBox->setSizePolicy( QSizePolicy::Fixed, QSizePolicy::Fixed );
                int left, top, right, bottom;
                chkBox->getContentsMargins( &left, &top, &right, &bottom );
                chkBox->setContentsMargins( left, top, 0, bottom );

                // create the combo box
                QComboBox *comboBox = new QComboBox( toolBoxPages.at(category).page );
                QStringList choicesStrings = gcSettings->value(gcParameter + "/Choices").toString().split("|");
                comboBox->addItems( choicesStrings );
                comboBox->setCurrentIndex( gcSettings->value(gcParameter + "/Value").toInt() );
                paramToolTip = gcSettings->value(gcParameter + "/Description").toString();
                comboBox->setToolTip( paramToolTip );

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(chkBox);
                hboxLayout->addWidget(comboBox);
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its lineedit
                ParamMultiple paramMultiple;
                paramMultiple.paramName = gcParameter;
				paramMultiple.paramCallName = parameterCallName;
                paramMultiple.comboBox = comboBox;
                paramMultiple.choicesStrings = choicesStrings;
                paramMultiple.valueEnabledChkBox = chkBox;
                paramMultiples.append(paramMultiple);

                QObject::connect(comboBox, SIGNAL(activated(int)), this, SIGNAL(settingsCodeChanged()));
                QObject::connect(chkBox, SIGNAL(clicked()), this, SIGNAL(settingsCodeChanged()));
            }

        }
    }

    // put a spacer at each page end
    foreach (ToolBoxPage tbp, toolBoxPages) {
        tbp.vboxLayout->addStretch();
    }
}


/*!
    Opens all uigui ini files found in the list \a indenterIniFileList, opens each ini file
    and reads the there defined real name of the indenter. These names are being returned as QStringList
 */
QStringList IndentHandler::getAvailableIndenters() {
    QSettings *indenterSettings;
    QStringList indenterNamesList;

    foreach (QString indenterIniFile, indenterIniFileList) {
        indenterSettings = new QSettings(dataDirctoryStr + indenterIniFile, QSettings::IniFormat, NULL);
        indenterNamesList << indenterSettings->value(" header/indenterName").toString();
        delete indenterSettings;
    }
    return indenterNamesList;
}


void IndentHandler::setIndenter(int indenterID) {
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
    delete gcSettings;

    readIndentIniFile( dataDirctoryStr + indenterIniFileList.at(indenterID) );
}

//! Returns a string containing by the indenter supported file types/extensions devided by a space
QString IndentHandler::getPossibleIndenterFileExtensions() {
	return fileTypes;
}

//! Returns the path and filename of the current indenter config file
QString IndentHandler::getIndenterCfgFile() {
    QFileInfo fileInfo( dataDirctoryStr + configFilename );
    return fileInfo.absoluteFilePath();
}
