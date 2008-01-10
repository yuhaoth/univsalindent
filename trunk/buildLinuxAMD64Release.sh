#!/bin/bash
echo setting Qt path to /usr/local/Trolltech/Qt-4.2.3/bin
echo ----------------------------------------------------
PATH=/usr/local/Trolltech/Qt-4.2.3/bin:$PATH
export PATH
echo calling rm -r release/debug
echo ---------------------------
rm -r release
rm -r debug
echo calling svn update
echo ------------------
#svn update
echo calling qmake
echo -------------
qmake
echo calling lrelease release
echo ------------------------
lrelease ./translations/universalindent_de.ts -qm ./translations/universalindent_de.qm
lrelease ./translations/universalindent_zh_TW.ts -qm ./translations/universalindent_zh_TW.qm
lrelease ./translations/universalindent_ja_JP.ts -qm ./translations/universalindent_ja_JP.qm
echo calling make release
echo --------------------
make release
echo calling rm -r UniversalIndentGUI_linuxAMD64
echo ----------------------------------------
rm -r UniversalIndentGUI_linuxAMD64
echo calling mkdir UniversalIndentGUI_linuxAMD64
echo ----------------------------------------
mkdir UniversalIndentGUI_linuxAMD64
mkdir UniversalIndentGUI_linuxAMD64/indenters
mkdir UniversalIndentGUI_linuxAMD64/doc
mkdir UniversalIndentGUI_linuxAMD64/translations
echo copying files to UniversalIndentGUI_linuxAMD64
echo ----------------------------------------
cp ./CHANGELOG.txt ./UniversalIndentGUI_linuxAMD64/
cp ./LICENSE.GPL ./UniversalIndentGUI_linuxAMD64/
cp ./README.txt ./UniversalIndentGUI_linuxAMD64/
cp ./INSTALL.txt ./UniversalIndentGUI_linuxAMD64/
#cp ./indenters/.astylerc ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/astyle ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/astyle.html ./UniversalIndentGUI_linuxAMD64/indenters/
#cp ./indenters/bcpp.cfg ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/bcpp ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/bcpp.txt ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/csstidy ./UniversalIndentGUI_linuxAMD64/indenters/
#cp ./indenters/gc.cfg ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/gc.exe ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/gc.txt ./UniversalIndentGUI_linuxAMD64/indenters/
#cp ./indenters/.indent.pro ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/indent ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/indent.html ./UniversalIndentGUI_linuxAMD64/indenters/
#cp ./indenters/uncrustify.cfg ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uncrustify ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uncrustify.txt ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_astyle.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_bcpp.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_csstidy.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_greatcode.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_phpCB.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_gnuindent.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/uigui_uncrustify.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/highlighter.ini ./UniversalIndentGUI_linuxAMD64/indenters/
cp ./indenters/example.cpp ./UniversalIndentGUI_linuxAMD64/indenters/

# Copy the translation files.
cp %QTDIR%/translations/qt_de.qm ./translations/
cp %QTDIR%/translations/qt_ja_jp.qm ./translations/qt_ja_JP.qm
cp ./translations/*.qm ./UniversalIndentGUI_linuxAMD64/translations/

cp ./doc/iniFileFormat.html ./UniversalIndentGUI_linuxAMD64/doc/
cp ./release/UniversalIndentGUI ./UniversalIndentGUI_linuxAMD64/
cd UniversalIndentGUI_linuxAMD64
echo compressing UniversalIndentGUI_linuxAMD64
echo ----------------------------------------
tar czf UniversalIndentGUI_0.6.0_Beta_linuxAMD64.tgz *
cd ..
read -p "press any key to continue"
