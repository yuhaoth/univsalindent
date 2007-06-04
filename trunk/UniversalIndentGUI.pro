######################################################################
# Automatically generated by qmake (2.00a) Di Mai 30 22:39:48 2006
# and editet by hand for additional dependencies
######################################################################
CONFIG += debug_and_release
TEMPLATE = app
TARGET += 
DEPENDPATH += resources \
              src \
              debug \
              release

INCLUDEPATH += src

LIBS += -lqscintilla2

# remove linker flag "-mthreads" so the mingwm10.dll is no longer needed
#message(old flags:$${QMAKE_LFLAGS})
parameters = $${QMAKE_LFLAGS}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
QMAKE_LFLAGS = $${newFlags}
#message(new flags:$${QMAKE_LFLAGS})

parameters = $${QMAKE_LFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
QMAKE_LFLAGS_EXCEPTIONS_ON = $${newFlags}

parameters = $${QMAKE_CXXFLAGS_EXCEPTIONS_ON}
newFlags =
for(parameter, parameters) {
    !contains(parameter, -mthreads) {
        newFlags += $${parameter}
    }
}
QMAKE_CXXFLAGS_EXCEPTIONS_ON = $${newFlags}



CONFIG(debug, debug|release) {
    DESTDIR = ./debug
} else {
    DESTDIR = ./release
}

MOC_DIR = $${DESTDIR}/tmp/moc
UI_DIR = $${DESTDIR}/tmp/uic
OBJECTS_DIR = $${DESTDIR}/tmp/obj
RCC_DIR = $${DESTDIR}/tmp/qrc

#message ( destdir is $${DESTDIR}. uic is $${UI_DIR}. moc is $${MOC_DIR})

# Input
HEADERS += src/aboutdialog.h \
           src/highlighter.h \
           src/indenthandler.h \
           src/mainwindow.h \
           src/uiguisettings.h \
           src/uiguisettingsdialog.h

FORMS += src/indentgui.ui \
         src/toolBarWidget.ui \
         src/UiGuiSettingsDialog.ui \
         src/aboutdialog.ui

SOURCES += src/aboutdialog.cpp \
           src/highlighter.cpp \
           src/indenthandler.cpp \
           src/main.cpp \
           src/mainwindow.cpp \
           src/uiguisettings.cpp \
           src/uiguisettingsdialog.cpp

RESOURCES += resources/Icons.qrc
RC_FILE    = resources/programicon.rc
