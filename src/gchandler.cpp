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

#include "gchandler.h"

/*!
    Constructor of the GreatCode handler. This is a widget that is
    loaded into the main window if GreatCode is selected as indenter.
    Handles GreatCode config file read/write and calls GreatCode
    to reformat source text.
    Calls GreatCode each time a setting has been changed and informs
    the main window about the reformatted source code.
 */
GcHandler::GcHandler(QString dataDirPathStr, QWidget *parent) : QWidget(parent)
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

    // reads and parses first found indent ini file and creates toolbox entries
    readIndentIniFile();
}

//! Format source code with GreatCode
QString GcHandler::callGreatCode(QString sourceCode) {

    QString formattedSourceCode;
    QFile::remove(dataDirctoryStr + "gcout.cpp");
    QFile outSrcFile(dataDirctoryStr + "gcout.cpp");

    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    outSrcFile.write( sourceCode.toAscii() );
    outSrcFile.close();

#if defined(Q_OS_LINUX)
    QProcess::execute("wine " + dataDirctoryStr + indenterProgramName + " -file-" + dataDirctoryStr + "gcout.cpp -output_test-");
#else
    QProcess::execute(dataDirctoryStr + indenterProgramName + " -file-" + dataDirctoryStr + "gcout.cpp -output_test-");
#endif

    outSrcFile.setFileName(dataDirctoryStr + "gcout_test.cpp");
    outSrcFile.open(QFile::ReadOnly | QFile::Text);
    formattedSourceCode = outSrcFile.readAll();
    outSrcFile.close();

    return formattedSourceCode;
}

//! Generates a string with all parameters needed to call GreatCode and write it to the indenter config file
void GcHandler::generateParameterString() {

    parameterString = "";

    // generate parameter string for all boolean values
    foreach (ParamCheckBox pChkBox, paramCheckBoxes) {
        if ( pChkBox.checkBox->isChecked() ) {
            parameterString += "-" + pChkBox.paramName + " \n";
            gcSettings->setValue( pChkBox.paramName + "/Value", 1);
        }
        else {
            parameterString += "-no-" + pChkBox.paramName + " \n";
            gcSettings->setValue( pChkBox.paramName + "/Value", 0);
        }
    }

    // generate parameter string for all numeric values
    foreach (ParamSpinBox pSpinBox, paramSpinBoxes) {
        parameterString += "-" + pSpinBox.paramName + QString::number( pSpinBox.spinBox->value() ) + " \n";
        gcSettings->setValue( pSpinBox.paramName + "/Value", pSpinBox.spinBox->value());
    }

    // generate parameter string for all string values
    foreach (ParamLineEdit pLineEdit, paramLineEdits) {
        if ( pLineEdit.lineEdit->text() != "" ) {
            if ( pLineEdit.paramName == "cmt_fixme-" ) {
                parameterString += "-" + pLineEdit.paramName + "\"" + pLineEdit.lineEdit->text() + "\" \n";
            }
            else {
                parameterString += "-" + pLineEdit.paramName + pLineEdit.lineEdit->text() + " \n";
            }
            gcSettings->setValue( pLineEdit.paramName + "/Value", pLineEdit.lineEdit->text());
        }
    }

    writeConfigFile(parameterString);

    emit settingsCodeChanged();
}


//! Write config file
void GcHandler::writeConfigFile(QString paramString) {

    QFile::remove( dataDirctoryStr + "gc.cfg" );
    QFile outSrcFile( dataDirctoryStr + "gc.cfg");

    outSrcFile.open( QFile::ReadWrite | QFile::Text );
    outSrcFile.write( paramString.toAscii() );
    outSrcFile.close();
}

//! Load a GC config file
void GcHandler::loadConfigFile(QString filePathName) {

	QFile cfgFile(filePathName);
	int index;
    int crPos;
    int paramValue = 0;
    QString paramValueStr;

    // open the config file and read all data
	cfgFile.open( QFile::ReadOnly | QFile::Text );
	cfgFileData = cfgFile.readAll();
	cfgFile.close();

    // search for name of each boolean parameter and set/or not if "no-" is found in front of it
	foreach (ParamCheckBox pChkBox, paramCheckBoxes) {
		index = cfgFileData.indexOf( pChkBox.paramName, 0 );
		if ( index != -1 ) {
            // get the three charcters in front of the found index
			if ( cfgFileData.mid(index-3, 3) == "no-" ) {
				pChkBox.checkBox->setChecked(false);
			}
			else {
				pChkBox.checkBox->setChecked(true);
			}
		}
	}

    // search for name of each numeric parameter and set the value found behind it
	foreach (ParamSpinBox pSpinBox, paramSpinBoxes) {
		index = cfgFileData.indexOf( pSpinBox.paramName, 0 );
		if ( index != -1 ) {
            // set index after the parameter name, so in front of the number
            index += pSpinBox.paramName.length();

            // find the line end by searching for carriage return
            crPos = cfgFileData.indexOf( '\n', index+1 );

            // get the number and convert it to int
            paramValue = cfgFileData.mid( index, crPos - index ).toInt(NULL);

            // disable the signal-slot connection. Otherwise signal is emmitted each time when value is set
            QObject::disconnect(pSpinBox.spinBox, SIGNAL(valueChanged(int)), this, SLOT(generateParameterString()));
            pSpinBox.spinBox->setValue( paramValue );
            QObject::connect(pSpinBox.spinBox, SIGNAL(valueChanged(int)), this, SLOT(generateParameterString()));
		}
	}

    // search for name of each string parameter and set/or not if "no-" is found in front of it
	foreach (ParamLineEdit pLineEdit, paramLineEdits) {
		index = cfgFileData.indexOf( pLineEdit.paramName, 0 );
		if ( index != -1 ) {
            // set index after the parameter name, so in front of the string
            index += pLineEdit.paramName.length();

            // find the line end by searching for carriage return
            crPos = cfgFileData.indexOf( '\n', index+1 );

            // get the number and convert it to int
            paramValueStr = QString( cfgFileData.mid( index, crPos - index ) );
            pLineEdit.lineEdit->setText( paramValueStr );
		}
	}

	generateParameterString();
}

void GcHandler::readIndentIniFile() {
    QString iniFilePath = dataDirctoryStr + indenterIniFileList.first();

    // open the ini-file that contains all available GreatCode settings with their additional infos
    gcSettings = new QSettings(iniFilePath, QSettings::IniFormat, NULL);
    // read all possible parameters written in brackets []
    gcParamters = gcSettings->childGroups();
	QStringList gcCategories;
    QString gcGroupString = "";
    QString paramToolTip = "";

    indenterName = gcSettings->value("_header/name").toString();
    indenterProgramName = gcSettings->value("_header/filename").toString();

    // read the categories names which are separated by "|"
    QString gcCategoriesStr = gcSettings->value("_header/categories").toString();
    gcCategories = gcCategoriesStr.split("|");

    ToolBoxPage toolBoxPage;

    // create a page for each category and store its references in a toolboxpage-array
    foreach (QString category, gcCategories) {
        //QString categoryName = gcSettings->value("Categories/" + category).toString();
        QString categoryName = category;

        toolBoxPage.page = new QWidget();
        toolBoxPage.page->setObjectName(categoryName);
        toolBoxPage.vboxLayout = new QVBoxLayout(toolBoxPage.page);
        toolBoxPage.vboxLayout->setSpacing(6);
        toolBoxPage.vboxLayout->setMargin(9);
        toolBoxPage.vboxLayout->setObjectName(categoryName);
        toolBoxPages.append(toolBoxPage);
        toolBox->addItem(toolBoxPage.page, QApplication::translate("GcHandler", categoryName.toAscii(), 0, QApplication::UnicodeUTF8));
    }

    // read each parameter to create the corresponding input field
    foreach (QString gcParameter, gcParamters) {

        // if it is not the indent header definition read the parameter and add it to
        // the corresponding category toolbox page
		if ( gcParameter != "_header") {
            // read to which category the parameter belongs
            int category = gcSettings->value(gcParameter + "/Category").toInt();
            // read which type of input field the parameter needs
            QString editType = gcSettings->value(gcParameter + "/EditorType").toString();

            // edit type is numeric so create a spinbox with label
            if ( editType == "numeric" ) {
                // create the spinbox
                QSpinBox *spinBox = new QSpinBox( toolBoxPages.at(category).page );
                spinBox->setValue( gcSettings->value(gcParameter + "/Value").toInt() );
                paramToolTip = gcSettings->value(gcParameter + "/Description").toString();
                spinBox->setToolTip( paramToolTip );
                spinBox->setMaximumWidth(50);
                spinBox->setMinimumWidth(50);
				if ( gcSettings->value(gcParameter + "/MinMax").toString() == "" ) {
					spinBox->setMaximum(2000);
				}
				else {
					// a negative number sets the min value, a positive the max value
					int minMax = gcSettings->value(gcParameter + "/MinMax").toInt();
					if ( minMax < 0 ) {
						spinBox->setMinimum( minMax * (-1) );
					}
					else {
						spinBox->setMaximum( minMax );
					}
				}

                // create the label
                QLabel *label = new QLabel( toolBoxPages.at(category).page );
                label->setText(gcParameter);
                label->setBuddy(spinBox);
                label->setToolTip( paramToolTip );

                // put all into a layout and add it to the toolbox page
                QHBoxLayout *hboxLayout = new QHBoxLayout();
                hboxLayout->addWidget(spinBox);
                hboxLayout->addWidget(label);                
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its spinbox
                ParamSpinBox paramSpinBox;
                paramSpinBox.paramName = gcParameter;
                paramSpinBox.spinBox = spinBox;
                paramSpinBoxes.append(paramSpinBox);

                QObject::connect(spinBox, SIGNAL(valueChanged(int)), this, SLOT(generateParameterString()));
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
                ParamCheckBox paramCheckBox;
                paramCheckBox.paramName = gcParameter;
                paramCheckBox.checkBox = chkBox;
                paramCheckBoxes.append(paramCheckBox);

                QObject::connect(chkBox, SIGNAL(clicked()), this, SLOT(generateParameterString()));
            }
            // edit type is numeric so create a lineedit with label
            else {
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
                hboxLayout->addWidget(lineEdit);
                hboxLayout->addWidget(label);  
                toolBoxPages.at(category).vboxLayout->addLayout(hboxLayout);

                // remember parameter name and reference to its lineedit
                ParamLineEdit paramLineEdit;
                paramLineEdit.paramName = gcParameter;
                paramLineEdit.lineEdit = lineEdit;
                paramLineEdits.append(paramLineEdit);

                QObject::connect(lineEdit, SIGNAL(editingFinished()), this, SLOT(generateParameterString()));
            }
        }
    }

    // put a spacer at each page end
    foreach (ToolBoxPage tbp, toolBoxPages) {
        spacerItem = new QSpacerItem(40, 20, QSizePolicy::Minimum, QSizePolicy::Expanding);
        tbp.vboxLayout->addItem(spacerItem);
    }
}
