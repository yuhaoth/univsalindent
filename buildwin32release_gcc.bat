rd UniversalIndentGUI_win32 /S /Q
md UniversalIndentGUI_win32
cd UniversalIndentGUI_win32
md data
cd ..
copy .\data\.astylerc .\UniversalIndentGUI_win32\data
copy .\data\AStyle.exe .\UniversalIndentGUI_win32\data
copy .\data\astyle.html .\UniversalIndentGUI_win32\data
copy .\data\indent.exe .\UniversalIndentGUI_win32\data
copy .\data\indent.html .\UniversalIndentGUI_win32\data
copy .\data\.indent.pro .\UniversalIndentGUI_win32\data
rem copy .\data\libiconv-2.dll .\UniversalIndentGUI_win32\data
rem copy .\data\libintl-2.dll .\UniversalIndentGUI_win32\data
copy .\data\gc.cfg .\UniversalIndentGUI_win32\data
copy .\data\gc.exe .\UniversalIndentGUI_win32\data
copy .\data\gc.txt .\UniversalIndentGUI_win32\data
copy .\data\uigui_astyle.ini .\UniversalIndentGUI_win32\data
copy .\data\uigui_gnuindent.ini .\UniversalIndentGUI_win32\data
copy .\data\uigui_greatcode.ini .\UniversalIndentGUI_win32\data
copy .\data\example.cpp .\UniversalIndentGUI_win32\data
copy .\changelog.txt .\UniversalIndentGUI_win32\
copy .\LICENSE.GPL .\UniversalIndentGUI_win32\
rem copy .\QtCore4.dll .\UniversalIndentGUI_win32\
rem copy .\QtGui4.dll .\UniversalIndentGUI_win32\
rem copy .\mingwm10.dll .\UniversalIndentGUI_win32\
copy .\readme.txt .\UniversalIndentGUI_win32\
copy .\release\UniversalIndentGUI.exe .\UniversalIndentGUI_win32\
cd UniversalIndentGUI_win32
7z.exe a -tzip UniversalIndentGUI_0.2.5_Alpha_win32.zip
cd ..
pause
