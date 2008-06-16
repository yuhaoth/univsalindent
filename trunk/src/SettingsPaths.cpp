/***************************************************************************
 *   Copyright (C) 2006-2008 by Thomas Schweitzer                          *
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


#include "SettingsPaths.h"


bool SettingsPaths::alreadyInitialized = false;
QString SettingsPaths::applicationBinaryPath = "";
QString SettingsPaths::settingsPath = "";
QString SettingsPaths::globalFilesPath = "";
QString SettingsPaths::indenterPath = "";
QString SettingsPaths::tempPath = "";
bool SettingsPaths::portableMode = false;


void SettingsPaths::init() {
    // Get the applications binary path, with respect to MacOSXs use of the .app folder. 
	applicationBinaryPath = QCoreApplication::applicationDirPath();

#ifdef UNIVERSALINDENTGUI_NPP_EXPORTS
    applicationBinaryPath += "/plugins/uigui";
#endif

#ifdef Q_OS_MAC
    // Because on Mac universal binaries are used, the binary path is not equal
	// to the applications (.app) path. So get the .apps path here.
    int indexOfDotApp = applicationBinaryPath.indexOf(".app");
    if ( indexOfDotApp != -1 ) {
		// Cut off after the dot of ".app".
	    applicationBinaryPath = applicationBinaryPath.left( indexOfDotApp-1 );
		// Cut off after the first slash that was in front of ".app" (normally this is the word "UniversalIndentGUI")
	    applicationBinaryPath = applicationBinaryPath.left( applicationBinaryPath.lastIndexOf("/") );
	}
#endif

    // If the "config" directory is a subdir of the applications binary path, use this one (portable mode)
    settingsPath = applicationBinaryPath + "/config";
    if ( QFile::exists( settingsPath ) ) {
        portableMode = true;
        QDir dirCreator;
        globalFilesPath = applicationBinaryPath;
        indenterPath = applicationBinaryPath + "/indenters";
        dirCreator.mkpath( settingsPath );
        tempPath = applicationBinaryPath + "/temp";
        //TODO: If the portable drive has write protection, use local temp path and clean it up on exit.
        dirCreator.mkpath( tempPath );
    }
    // ... otherwise use the system specific global application data path.
    else {
        portableMode = false;
        QDir dirCreator;
#ifdef Q_OS_WIN
        // Get the local users application settings directory.
        settingsPath = QDir::fromNativeSeparators( qgetenv("APPDATA") ) + "/UniversalIndentGUI";
        // On windows systems the directories "indenters", "translations" are subdirs of the applicationBinaryPath.
        globalFilesPath = applicationBinaryPath;
#else
        settingsPath = QDir::homePath() + "/.universalindentgui";
        globalFilesPath = "/usr/share/universalindentgui";
#endif
        dirCreator.mkpath( settingsPath );
        // If a highlighter config file does not exist in the users home config dir
        // copy the default config file over there.
        if ( !QFile::exists(settingsPath+"/UiGuiSyntaxHighlightConfig.ini") ) {
            QFile::copy( globalFilesPath+"/config/UiGuiSyntaxHighlightConfig.ini", settingsPath+"/UiGuiSyntaxHighlightConfig.ini" );
        }
        indenterPath = globalFilesPath + "/indenters";
#ifdef Q_OS_WIN
        tempPath = QDir::tempPath() + "/UniversalIndentGUI";
#else
        tempPath = QDir::tempPath() + "UniversalIndentGUI";
#endif
        dirCreator.mkpath( tempPath );
    }

    alreadyInitialized = true;
}


const QString SettingsPaths::getApplicationBinaryPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return applicationBinaryPath;
}


const QString SettingsPaths::getSettingsPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return settingsPath;
}


const QString SettingsPaths::getGlobalFilesPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return globalFilesPath;
}


const QString SettingsPaths::getIndenterPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return indenterPath;
}


const QString SettingsPaths::getTempPath() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return tempPath;
}


bool SettingsPaths::getPortableMode() {
    if ( !alreadyInitialized ) {
        SettingsPaths::init();
    }
    return portableMode;
}
