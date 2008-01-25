######################################################################
# Automatically generated by qmake (2.00a) Di Mai 30 22:39:48 2006
# and editet by hand for additional dependencies
######################################################################
CONFIG -= debug
CONFIG -= release
CONFIG += debug_and_release
TEMPLATE = app
QT += network
QT += script 

unix:TARGET = universalindentgui
win32:TARGET = UniversalIndentGUI
macx:TARGET = UniversalIndentGUI

DEPENDPATH += resources \
              src \
              debug \
              release

INCLUDEPATH += src

LIBS += -lqscintilla2

macx {
 CONFIG += x86 ppc sdk
 QMAKE-MAC_SDK = /Developer/SDKs/MacOSX10.4u.sdk
}

TRANSLATIONS = translations/universalindent_de.ts \
               translations/universalindent_ja_JP.ts \
               translations/universalindent_zh_TW.ts

# Copy Qts own translation files to the local translation directory
unix:system(cp $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm ./translations/)
unix:system(cp $$[QT_INSTALL_TRANSLATIONS]/qt_ja_jp.qm ./translations/qt_ja_JP.qm)
win32:system(copy $$[QT_INSTALL_TRANSLATIONS]/qt_de.qm ./translations/)
win32:system(copy $$[QT_INSTALL_TRANSLATIONS]/qt_ja_jp.qm ./translations/qt_ja_JP.qm)

# Defining files that shall be installed
########################################
# Create and install man page
unix:system(rm doc/universalindentgui.1*)
unix:system(cp doc/universalindentgui.man doc/universalindentgui.1)
unix:system(gzip doc/universalindentgui.1)
unix:documentation.path = /usr/share/man/man1
unix:documentation.files = doc/universalindentgui.1.gz

# Install indenter ini files, examples and some indenters
unix:indenters.path = /etc/universalindentgui/indenters
unix:indenters.files = indenters/uigui_*.ini
unix:indenters.files += indenters/examples.*
unix:indenters.files += indenters/JsDecoder.js
unix:indenters.files += indenters/shellindent.awk

# Install translation files
unix:translation.path = /etc/universalindentgui/translations
unix:translation.files = translations/*.qm

# Install highlighter default config
unix:highlighterconfig.path = /etc/universalindentgui/config
unix:highlighterconfig.files = config/UiGuiSyntaxHighlightConfig.ini

# Install binary
unix:target.path = /usr/bin

# Set everything that shall be installed
unix:INSTALLS += target \
                 highlighterconfig \
                 indenters \
                 translation \
	             documentation



#######################
# remove linker flag "-mthreads" so the mingwm10.dll is no longer needed
#######################
#message(old flags:$${QMAKE_LFLAGS})
win32:parameters = $${QMAKE_LFLAGS}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_LFLAGS = $${newFlags}
#message(new flags:$${QMAKE_LFLAGS})

win32:parameters = $${QMAKE_LFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_LFLAGS_EXCEPTIONS_ON = $${newFlags}

win32:parameters = $${QMAKE_CXXFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
win32:QMAKE_CXXFLAGS_EXCEPTIONS_ON = $${newFlags}
#######################



CONFIG(debug, debug|release) {
    DESTDIR = ./debug
} else {
    DESTDIR = ./release
}

MOC_DIR = $${DESTDIR}/moc
UI_DIR = $${DESTDIR}/uic
OBJECTS_DIR = $${DESTDIR}/obj
RCC_DIR = $${DESTDIR}/qrc

#message ( destdir is $${DESTDIR}. uic is $${UI_DIR}. moc is $${MOC_DIR})

# Input
HEADERS += src/aboutdialog.h \
           src/highlighter.h \
           src/indenthandler.h \
           src/mainwindow.h \
           src/uiguierrormessage.h \
           src/uiguisettings.h \
           src/uiguisettingsdialog.h \
           src/updatecheckdialog.h

FORMS += src/indentgui.ui \
         src/toolBarWidget.ui \
         src/UiGuiSettingsDialog.ui \
         src/aboutdialog.ui \
         src/UpdateCheckDialog.ui

SOURCES += src/aboutdialog.cpp \
           src/highlighter.cpp \
           src/indenthandler.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/uiguierrormessage.cpp \
           src/uiguisettings.cpp \
           src/uiguisettingsdialog.cpp \
           src/updatecheckdialog.cpp

RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc
