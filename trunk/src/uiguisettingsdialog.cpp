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

#include "uiguisettingsdialog.h"

/*!
	\class UiGuiSettingsDialog
	\brief Displays a dialog window with settings for UniversalIndentGUI
*/

/*!
	The constructor calls the setup function for the ui created by uic. and adds
*/
UiGuiSettingsDialog::UiGuiSettingsDialog(QWidget* parent, UiGuiSettings* settings) : QDialog(parent)
{
    // Remember pointer to the UiGuiSettings object.
    this->settings = settings;

    // Init the user interface created by the UIC.
	setupUi(this);

    // Get all check boxes that are used for settings.
    checkBoxes = findChildren<QCheckBox*>( QRegExp("uiGui*") );

    // Get all spin boxes that are used for settings.
    spinBoxes = findChildren<QSpinBox*>( QRegExp("uiGui*") );

    // Connect the accepted signal to own function, to write values back to the UiGuiSettings object.
    connect(this, SIGNAL(accepted()), this, SLOT(handleAccepted()) );

	// Init the language selection combobox.
	initTranslationSelection();
}

/*!
	By calling this function the combobox for selecting the application language will
	be initialized. Also the translation itself will be reinitialized.
 */
void UiGuiSettingsDialog::initTranslationSelection() {
	// First empty the combo box.
	uiGuiLanguage->clear();
	
	// Now add an entry into the box for every language short.
	foreach (QString languageShort, settings->getAvailableTranslations() ) {
		// Identify the language mnemonic and set the full name.
		if ( languageShort == "en" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("English") );
		}
		else if ( languageShort == "de" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("German") );
		}
		else if ( languageShort == "zh" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Chinese") );
		}
		else if ( languageShort == "ja" ) {
			uiGuiLanguage->addItem( QIcon(QString(":/language/language-"+languageShort+".png")), tr("Japanese") );
		}
		else {
			uiGuiLanguage->addItem( tr("Unknown language mnemonic ") + languageShort );
		}
	}
}


/*!
    Displays the dialog by calling the dialogs exec function. Before it
    gets all the values needed from the UiGuiSettings object.
 */
int UiGuiSettingsDialog::showDialog() {
    // Get the values for the check boxes from the settings object.
    foreach (QCheckBox* checkBox, checkBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = checkBox->objectName();
        objectName.remove(0,5);

        // Get value from settings and assign it to the checkbox.
        bool value = settings->getValueByName( objectName ).toBool();
        checkBox->setChecked(value);
    }

    // Get the values for the spin boxes from the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = spinBox->objectName();
        objectName.remove(0,5);

        // Get value from settings and assign it to the checkbox.
        int value = settings->getValueByName( objectName ).toInt();
        spinBox->setValue(value);
    }

    // Execute the dialog.
    return exec();
}


/*!
    This slot is called when the dialog box is closed by pressing the Ok button.
    Writes all settings to the UiGuiSettings object.
 */
void UiGuiSettingsDialog::handleAccepted() {
    // Write the values of the check boxes to the settings object.
    foreach (QCheckBox* checkBox, checkBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = checkBox->objectName();
        objectName.remove(0,5);

        // Write the check box value to the settings.
        settings->setValueByName( objectName, checkBox->isChecked() );
    }

    // Write the values for the spin boxes to the settings object.
    foreach (QSpinBox* spinBox, spinBoxes) {
        // Get the objects name and remove "uiGui" from its beginning.
        QString objectName = spinBox->objectName();
        objectName.remove(0,5);

        // Write the spin box value to the settings.
        settings->setValueByName( objectName, spinBox->value() );
    }
}


/*!
    Calls the retranslate function generated by UIC. So the complete dialog is retranslated.
 */
void UiGuiSettingsDialog::retranslate() {
	retranslateUi(this);

	QStringList languageShortList = settings->getAvailableTranslations();

	// Now retranslate every entry in the language selection box.
	for (int i = 0; i < languageShortList.size(); i++ ) {
		QString languageShort = languageShortList.at(i);

		// Identify the language mnemonic and set the full name.
		if ( languageShort == "en" ) {
			uiGuiLanguage->setItemText( i, tr("English") );
		}
		else if ( languageShort == "de" ) {
			uiGuiLanguage->setItemText( i, tr("German") );
		}
		else if ( languageShort == "zh" ) {
			uiGuiLanguage->setItemText( i, tr("Chinese") );
		}
		else if ( languageShort == "ja" ) {
			uiGuiLanguage->setItemText( i, tr("Japanese") );
		}
		else {
			uiGuiLanguage->setItemText( i, tr("Unknown language mnemonic ") + languageShort );
		}
	}
}
