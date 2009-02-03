/***************************************************************************
 *   Copyright (C) 2006-2009 by Thomas Schweitzer                          *
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

#ifndef UPDATECHECKDIALOG_H
#define UPDATECHECKDIALOG_H

#include <QDialog>
#include <QMessageBox>
#include <QDesktopServices>
#include <QHttp>
#include <QUrl>
#include <QDate>
#include <QHBoxLayout>
#include <QLabel>
#include <QTimer>

#include "ui_UpdateCheckDialog.h"
#include "UiGuiSettings.h"

class UpdateCheckDialog : public QDialog, public Ui::UpdateCheckDialog
{
    Q_OBJECT

public:
    UpdateCheckDialog(QString currentVersion, UiGuiSettings *settings, QWidget *parent=0);

public slots:
    void checkForUpdateAndShowDialog();
    void checkForUpdate();

private:
    void getPadXMLFile();
    void showCheckingForUpdateDialog();
    void showNewVersionAvailableDialog(QString newVersion);
    void showNoNewVersionAvailableDialog();

    UiGuiSettings *settings;
    bool manualUpdateRequested;
    QHttp *http;
    QString currentVersion;
    QDialogButtonBox::ButtonRole roleOfClickedButton;
    QTimer *updateCheckProgressTimer;
    int updateCheckProgressCounter;
    int convertVersionStringToNumber(QString versionString);

private slots:
    void checkResultsOfFetchedPadXMLFile(bool errorOccurred);
    void handleUpdateCheckDialogButtonClicked(QAbstractButton *clickedButton);
    void updateUpdateCheckProgressBar();
};

#endif // UPDATECHECKDIALOG_H
