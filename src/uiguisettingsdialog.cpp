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
UiGuiSettingsDialog::UiGuiSettingsDialog(QWidget *parent) :
QDialog(parent)
{
	setupUi(this);

    connect(this, SIGNAL(accepted()), this, SLOT(handleAccepted()) );
}


void UiGuiSettingsDialog::retranslate() {
	retranslateUi(this);
}

int UiGuiSettingsDialog::showDialog() {
    return exec();
}

void UiGuiSettingsDialog::handleAccepted() {
}
