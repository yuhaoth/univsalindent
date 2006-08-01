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

#ifndef INDENTHANDLER_H
#define INDENTHANDLER_H

#include <QWidget>
#include <QToolBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QComboBox>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QByteArray>
#include <QDir>
#include <QMessageBox>

class IndentHandler : public QWidget
{
    Q_OBJECT

public:
    IndentHandler(QString dataDirPathStr, QWidget *parent = 0);

	IndentHandler(QString dataDirPathStr, int indenterID, QWidget *parent = 0);

    //! Format source code with GreatCode
    QString callGreatCode(QString sourceCode, QString fileExtension);

	void loadConfigFile(QString filePathName);

    QStringList getAvailableIndenters();

	QString getPossibleIndenterFileExtensions();

private:
    void writeConfigFile(QString parameterString);
    void readIndentIniFile(QString iniFilePath);

    // holds a reference to all created pages of the toolbox and the pages boxlayout
    struct ToolBoxPage
	{
        QWidget *page;
		QVBoxLayout *vboxLayout;
	};
	QVector<ToolBoxPage> toolBoxPages;

    // hold a reference to all checkboxes needed for boolean parameter setting and the parameters name
    struct ParamBoolean
	{
        QString paramName;
		QString trueString;
		QString falseString;
        QCheckBox *checkBox;
	};
	QVector<ParamBoolean> paramBooleans;

    // hold a reference to all lineedits needed for parameter setting and the parameters name
    struct ParamString
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QLineEdit *lineEdit;
        QLabel *label;
	};
	QVector<ParamString> paramStrings;

    // hold a reference to all spinboxes needed for parameter setting and the parameters name
    struct ParamNumeric
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QSpinBox *spinBox;
        QLabel *label;
	};
	QVector<ParamNumeric> paramNumerics;

    // hold a reference to all comboboxes needed for parameter setting and the parameters name
    struct ParamMultiple
	{
        QString paramName;
		QString paramCallName;
        QCheckBox *valueEnabledChkBox;
        QComboBox *comboBox;
        QStringList choicesStrings;
	};
	QVector<ParamMultiple> paramMultiples;

	QVBoxLayout *vboxLayout;
    QToolBox *toolBox;
    QSettings *gcSettings;
    QStringList gcParamters;
    QString parameterString;
	QByteArray cfgFileData;
    // the indenters name in a descriptive form
    QString indenterName;
    // the indenters file name, that is being called (w/o extension)
    QString indenterFileName;
    QString dataDirctoryStr;
    QStringList indenterIniFileList;
    QString configFilename;
    QString cfgFileParameterEnding;
    QString inputFileParameter;
    QString inputFileName;
    QString outputFileParameter;
	QString outputFileName;
    QString fileTypes;
    QString useCfgFileParameter;

private slots:
    void generateParameterString();
public slots:
    void setIndenter(int indenterID);

signals:
    void settingsCodeChanged();

};

#endif // INDENTHANDLER_H
