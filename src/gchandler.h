#ifndef GCHANDLER_H
#define GCHANDLER_H

#include <QWidget>
#include <QToolBox>
#include <QVBoxLayout>
#include <QApplication>
#include <QCheckBox>
#include <QFile>
#include <QProcess>
#include <QSettings>
#include <QStringList>
#include <QLineEdit>
#include <QSpinBox>
#include <QLabel>
#include <QByteArray>

class GcHandler : public QWidget
{
    Q_OBJECT

public:
    GcHandler(QWidget *parent = 0);

    //! Format source code with GreatCode
    QString callGreatCode(QString sourceCode);

	void loadConfigFile(QString filePathName);

private:
    void writeConfigFile(QString parameterString);

    // holds a reference to all created pages of the toolbox and the pages boxlayout
    struct ToolBoxPage
	{
        QWidget *page;
		QVBoxLayout *vboxLayout;
	};
	QVector<ToolBoxPage> toolBoxPages;

    // hold a reference to all checkboxes needed for parameter setting and the parameters name
    struct ParamCheckBox
	{
        QString paramName;
        QCheckBox *checkBox;
	};
	QVector<ParamCheckBox> paramCheckBoxes;

    // hold a reference to all lineedits needed for parameter setting and the parameters name
    struct ParamLineEdit
	{
        QString paramName;
        QLineEdit *lineEdit;
	};
	QVector<ParamLineEdit> paramLineEdits;

    // hold a reference to all spinboxes needed for parameter setting and the parameters name
    struct ParamSpinBox
	{
        QString paramName;
        QSpinBox *spinBox;
	};
	QVector<ParamSpinBox> paramSpinBoxes;

	QSpacerItem *spacerItem;
	QVBoxLayout *vboxLayout;
    QToolBox *toolBox;
    QSettings *gcSettings;
    QStringList gcParamters;
    QString parameterString;
	QByteArray cfgFileData;

private slots:
    void generateParameterString();

signals:
    void settingsCodeChanged();

};

#endif
