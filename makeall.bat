qmake
nmake all
mt -manifest .\release\UniversalIndentGUI.exe.manifest -outputresource:".\release\UniversalIndentGUI.exe;#1"
mt -manifest .\debug\UniversalIndentGUI.exe.manifest -outputresource:".\debug\UniversalIndentGUI.exe;#1"
