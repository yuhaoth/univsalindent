<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html xmlns="http://www.w3.org/1999/xhtml" xml:lang="en" lang="en">
<?php
include("htmlhead.html");
?>

<body>
	<div id="page">
		<div id="header">
			<div id="tabs">
				
				<a style="display:none;" href="#branding" >Skip Navigation</a>
				
				<ul>
					<li><a href="index.php">Home</a></li>
					<li id="current"><a style="padding: 18px 0px 35px 0px;" href="features.php">Features / <br />Changelog</a></li>
					<li><a href="screenshots.php">Screenshots</a></li>
					<li><a href="development.php">Development</a></li>
				</ul>
			</div>
		</div>

		<div id="main">

			<div id="content">
				<h2>Features</h2>
				<ul>
                    <li>Live preview. Directly see how the code is affected by a indenter parameter</li>
					<li>Configurable ini file, open for many indenters. Not yet known indenters can be added by creating
                        a new ini file for them</li>
					<li>Supported and included indenters right now are: <br />
                        <a class="external" href="http://astyle.sourceforge.net/" target="_blank">Artistic Styler</a>,
                        <a class="external" href="http://invisible-island.net/bcpp/" target="_blank">BCPP</a>,
                        <a class="external" href="http://csstidy.sourceforge.net/" target="_blank">CSSTidy</a>,
                        <a class="external" href="http://www.gnu.org/software/indent/" target="_blank">GNU Indent</a>,
                        <a class="external" href="http://sourceforge.net/projects/gcgreatcode/" target="_blank">GreatCode</a>,
                        <a class="external" href="http://code.gosu.pl/" target="_blank">JsDecoder</a>,
                        <a class="external" href="http://perltidy.sourceforge.net/" target="_blank">PerlTidy</a>,
                        <a class="external" href="http://www.waterproof.fr/products/phpCodeBeautifier/" target="_blank">phpCB</a> (executable not included in UiGUI),
                        <a class="external" href="http://www.bolthole.com/AWK.html" target="_blank">ShellIndent</a>,
                        <a class="external" href="http://tidy.sourceforge.net/" target="_blank">(HTML) Tidy</a> and
                        <a class="external" href="http://uncrustify.sourceforge.net/" target="_blank">Uncrustify</a>
                    </li>
                    <li>By integration of these indenters the following programming languages are supported : C, C++, C#, CSS,
                        D, HTML, JAVA, Javascript, ObjectiveC, Pawn, Perl, PHP, Shellscript and VALA</li>
                    <li>Syntax highlighting for all above languages (except for Pawn) and many other languages</li>
                    <li>Integration as external tool into any IDE or editor supporting external tool calls</li>
					<li>Load the settings of an indenter from its existing config file</li>
					<li>Save parameters as indeters config file for later use</li>
					<li>ToolTips for each indenter parameter to know what it does</li>
					<li>Multiple language support. Currently english, german, taiwan/chinese (thanks to Nelson Tai)
                        and partly japanese (thanks to Nirvash)</li>
				</ul>
				
				<br />
				<h2>Changelog</h2>

                <h4>2008-03-27 Version 0.8.1</h4>
				<ul>
                    <li>[add] Feature Request ID 1909450 <a href="issue/1909450" target="_blank">1909450</a> : Added support for the PEAR PHP beautifier. Thus PHP beautifying is also possible on Linux and Mac OS X. (Needs the PEAR package "PHP_Beautifier" to be installed and a path entry pointing to the PHP dir.)</li>
                    <li>[add] Added support and translations for Russian and Ukrainian language. Thanks to <a class="external" href="http://korytskyy.lviv.ua" target="_blank">Oleksandr</a>.</li>
                    <li>[add] Feature Request ID 1901935 <a href="issue/1901935" target="_blank">1901935</a> : From now on files are saved with the same encoding as they were opened and no longer always UTF-8. Also it is possible to save the file with any other encoding.</li>
                    <li>[change] The detection whether to run in portable mode now depends on where the "config" folder resides and no longer the "indenters" folder. This is needed to be able to correctly run on Windows multiuser systems.</li>
                    <li>[change] Removed the word "beta" whereever it occurred, since UiGUI isn't that beta anymore.</li>
                    <li>[fix] Bug ID 1910773 <a href="issue/1910773" target="_blank">1910773</a> : On Unix systems the globally available files, like indenters, translations etc are not longer installed to /etc/universalindentgui but to /usr/share/universalindentgui.</li>
                </ul>
                
				<h4>2008-01-16 Version 0.8.0 Beta</h4>
				<ul>
                    <li>[add] Feature Request ID <a href="issue/1849297" target="_blank">1849297</a> : Added support for indenters written in JavaScript using an internal interpreter. Have a look into the README.txt to get to know, what steps are needed for that.</li>
					<li>[add] Added support for a simple JavaScript indenter written in JavaScript itself.</li>
					<li>[add] Added support for a simple shell code indenter written in awk.</li>
                    <li>[add] Feature Request ID <a href="issue/1736946" target="_blank">1736946</a> : It is now possible to run any indenter that is available in the global environment via a path entry.</li>
                    <li>[add] Feature Request ID <a href="issue/1852483" target="_blank">1852483</a> : Indenters written in script languages, like perl, are now also supported, if they have a correct shebang.</li>
					<li>[add] Added support for the well known perltidy. The uigui ini file is completed up to the category and including "Line Break Control". Rest needs to be done.</li>
                    <li>[add] Feature Request ID <a href="issue/1866599" target="_blank">1866599</a> : Adapted the used paths for settings, temporary files and the one containing indenter ini files to be conform with systems that use strict user rights management. Most this concerns Linux, where the binary resides inside a bin dir and user specific settings are stored in his home dir. Short: now supporting multiuser systems and also have a portable mode, where modifications are only on local media.</li>
                    <li>[add] Feature Request ID <a href="issue/1730360" target="_blank">1730360</a> : Along with real support for multiuser systems, each user can have his own indenter settings. These won't be overwritten by an update, what was the case before.</li>
					<li>[add] Feature Request ID <a href="issue/1867628" target="_blank">1867628</a> : Added a button to the toolbar near the indenter selection that opens the indenters online manual.</li>
					<li>[add] Feature Request ID <a href="issue/1867660" target="_blank">1867660</a> : Created a simple man page for unix based systems.</li>
					<li>[add] The by the qmake projekt file resulting makefile has a directive for install on unix based systems now.</li>
					<li>[add] Added example files for most supported programming languages, so each indenter can be tested.</li>
                    <li>[change] The source code archive is now in a Unix conform format and naming convention.</li>
                    <li>[change] The used paths have changed. The "data" directory has been renamed to indenters and includes only the uigui ini files and maybe some indenter binaries. Futher a config and a temp directory are used.</li>
					<li>[change] In case of an error during calling the indenter, its standard output is appended to the error dialog.</li>
					<li>[change] Corrected some misspelling.</li>
					<li>[change] Renamed htmltidy to tidy because that is its official name and also the name of the binary in all Linux packages.</li>
                    <li>[fix] Bug ID <a href="issue/1855460" target="_blank">1855460</a> : The parameter settings of the indenter were only saved, if preview is turned on. Now the settings are always remembered.</li>
                    <li>[fix] Bug ID <a href="issue/1854156" target="_blank">1854156</a> : The syntax highlighter did not switch to perl if a file with suffix .pl or .pm was loaded.</li>
                </ul>

                <h4>2007-11-22 Version 0.7.1 Beta</h4>
				<ul>
                    <li>[add] Feature Request ID <a href="issue/1833814" target="_blank">1833814</a> : Added support for building on MacOSX. Also 10.5 Leopard. Currently contains only Artistic Styler and Uncrustify.</li>
                    <li>[change] Feature Request ID <a href="issue/1836486" target="_blank">1836486</a> : Added an option to clear the list of recently opened files.</li>
                    <li>[change] Update to latest version of Artistic Styler.</li>
                </ul>
                
                <h4>2007-11-11 Version 0.7.0 Beta</h4>
				<ul>
                    <li>[add] Feature Request ID <a href="issue/1826733" target="_blank">1826733</a> : If the called indenter returns an error message, you can chose to not show the same error again. Helpful for HTMLtidy since it often shows a long, repeating error list.</li>
                    <li>[add] Feature Request ID <a href="issue/1805974" target="_blank">1805974</a> : Added an option to clear the list of recently opened files.</li>
                    <li>[add] Feature Request ID <a href="issue/1760997" target="_blank">1760997</a> : Added some code lines to example.cpp to test whether an indenter correctly handles preprocessor defines.</li>
                    <li>[add] Feature Request ID <a href="issue/1752551" target="_blank">1752551</a> : If the user manually checks for updates a progress/busy dialog will be shown.</li>
                    <li>[add] Feature Request ID <a href="issue/1736932" target="_blank">1736932</a> : The user gets shown a dialog if he has manually checked for an update and no new one is available.</li>
                    <li>[change] The update check is no enabled by default.</li>
                    <li>[change] Slighty changes made to the settings dialog; put texts in front of the changed value.</li>
                    <li>[fix] Bug ID <a href="issue/1807179" target="_blank">1807179</a> : GNU Indent didn't work properly on Windows system, after the dlls libiconv-2.dll and libintl-2.dll were removed. Readded them.</li>
                    <li>[fix] Bug ID <a href="issue/1805976" target="_blank">1805976</a> : The HTML export didn't produce any output. Fixed that, but still has not syntax formatting.</li>
                    <li>[fix] Bug ID <a href="issue/1815023" target="_blank">1815023</a> : Mixed traditional with simplified chinese translation. Now preferring to use english text instead of mixing if no traditional translation is available.</li>
                    <li>[fix] Bug ID <a href="issue/1815120" target="_blank">1815120</a> : The font of the editor component was wrong (not monospaced), if syntax highlighting was turned off.</li>
                </ul>
				
				<h4>2007-06-13 Version 0.6.1 Beta</h4>
				<ul>
                    <li>[add] Feature Request ID <a href="issue/1729433" target="_blank">1729433</a> : Added a recently opened file menu.</li>
                    <li>[add] Feature Request ID <a href="issue/1704290" target="_blank">1704290</a> : Added possibiltiy to integrate into an IDE, editor or other as external tool. This is donw by creating a batch/shell script which accepts a file as parameter and calls the indenter with the in UniversalIndentGUI made settings.</li>
                    <li>[add] Feature Request ID <a href="issue/1673659" target="_blank">1673659</a> : The string parameter in the uigui ini files for each indenter can now also be a list of strings divided by "|" sign. As result the parameter name with value will be written to the indenter config file for each of the values.</li>
                    <li>[add] Feature Request ID <a href="issue/1730180" target="_blank">1730180</a> : UniversalIndentGUI can automatically check on start, whether a new version is available. This setting is disabled by default and can be enabled in the settings.</li>
                    <li>[add] Added support for HTML Tidy. The executable is contained in the release package.</li>
                    <li>[add] Feature Request ID <a href="issue/1736432" target="_blank">1736432</a> : The width of the dock widget containing  the indenter parameters is now restored on program start.</li>
                    <li>[change] The about dialog shows now a credits scroller to say thanks to all people, who have helped me somehow.</li>
                    <li>[fix] Bug ID <a href="issue/1733499" target="_blank">1733499</a> : UniversalIndentGui crashed if inside of an indenter ini file the category number of one parameter was higher than the available categories.</li>
                    <li>[fix] Bug ID <a href="issue/1735586" target="_blank">1735586</a> : Some dialogs had untranslated buttons. For example yes, no, cancel were not translated.</li>
                </ul>
                
                <h4>2007-06-03 Version 0.6.0 Beta</h4>
				<ul>
                    <li>[add] Feature Request ID <a href="issue/1700557" target="_blank">1700557</a> : Added a settings dialog for handling more possible settings with better usabiltiy.</li>
                    <li>[add] Feature Request ID <a href="issue/1700556" target="_blank">1700556</a> : Added option to load last file or not on startup.</li>
                    <li>[add] Feature Request ID <a href="issue/1684011" target="_blank">1684011</a> : Added option to set the width used to display tabs (default is four spaces).</li>
                    <li>[add] Feature Request ID <a href="issue/1651718" target="_blank">1651718</a> : Added multiple choices, for the indenter configuration, with for the user readable text. Before the text inside of the combo boxes for the indenter parameters was exactly the same text as the command line parameter of the indenter. Now with "ChoicesReadable" a list with the same length as "Choices" can be used to insert readable text into the combo box. If this list is not set, the content of "Choices" will be used.</li>
                    <li>[add] Partly Japanese translation. Thanks to Nirvash.</li>
                    <li>[add] The language selection show the countries flag in front of its name to identify the language faster.</li>
                    <li>[add] Feature Request ID <a href="issue/1657253" target="_blank">1657253</a> : If now configuration ini file for any indenter can be found, a warning will be displayed.</li>
                    <li>[change] Feature Request ID <a href="issue/1723228" target="_blank">1723228</a> : The default font for all syntax highlighters is now mono spaced, Courier.</li>
                    <li>[change] Along with the settings dialog cleaned up the user interface. Moved some not that often needed settings away to the settings dialog.</li>
                    <li>[change] The full text of the GPL in the about dialog did slow down the program startup and translation. Now only a hint and link to the GPL is given in the about dialog.</li>
                    <li>[fix] Bug ID <a href="issue/1692511" target="_blank">1692511</a> : UniversalIndentGui crashed, if a by the syntax highlighter unsupported file extension was opened.</li>
                    <li>[fix] Bug ID <a href="issue/1727538" target="_blank">1727538</a> : The setting for syntax highlighting was allways enabled on startup, even if disabled on closing.</li>
                    <li>[fix] Bug ID <a href="issue/1729026" target="_blank">1729026</a> : The highlighting style was not correct when changed to another highlighter than the current. Only after toggling preview for example the style was correct.</li>
                    <li>[fix] The editors column showing the line numbers did not adapt its width to the maximum line number.</li>
                </ul>
                
				<h4>2007-03-21 Version 0.5.1 Beta</h4>
				<ul>
                    <li>[change] The live typing and indenting feature was not adapted to QScintilla. Now works again (depending on the used indenter).</li>
                    <li>[fix] Bug ID <a href="issue/1681246" target="_blank">1681246</a> : Files other encoded than ascii were not displayed correctly. Now allways displaying files using UTF-8 encoding.</li>
                    <li>[fix] Bug ID <a href="issue/1681247" target="_blank">1681247</a> : The new syntax highlighter selection menu was not translated.</li>
                    <li>[fix] When file was reloaded with other encoding the file was allways set to be modified even if it wasn't.</li>
                </ul>
                
				<h4>2007-03-14 Version 0.5.0 Beta</h4>
				<ul>
                    <li>[add/change] Using QScintilla as editing component, which is by far more mighty than the before uses QTextEdit. Have a look inside the development area for details.</li>
                    <li>[add] Because of QScintilla, added syntax highlightning for bash, batch, cpp, csharp, css, diff, html, idl, java, javascript, lua, makefile, perl, pov, ini, python, ruby, sql and tex.</li>
                    <li>[add] Also added code folding for previously mentioned languages.</li>
                    <li>[add] Added support for the indenter "phpCB" phpCodeBeatufier. (Thanks to Nelson Tai) The executable is not included in the UiGui release, because I am not sure about the license of phpCB right now.</li>
                    <li>[add] The output of an indenter can now be read from stdout, by setting "stdout" for "outputFileParameter".</li>
                    <li>[add] Feature Request ID <a href="issue/1673549" target="_blank">1673549</a> : The order of the indenter call parameters for input file, output file and options[file] is now selectable in three states: pio, ipo, iop.</li>
                    <li>[add] Feature Request ID <a href="issue/1675503" target="_blank">1675503</a> : The last set encoding is being remembered and used on loading last file.</li>
                    <li>[add] Feature Request ID <a href="issue/1669822" target="_blank">1669822</a> : The last window position, size and maximized state is stored in the settings and restored.</li>
                    <li>[add] Feature Request ID <a href="issue/1673670" target="_blank">1673670</a> : Made it possible to switch between visible and invisible white space characters, like spaces and tabs.</li>
                    <li>[change] The "personal" configuration files of each indenter (like gc.cfg) are no longer included in a release, so these settings can be loaded into UiGui after updating.</li>
                    <li>[fix] Bug ID <a href="issue/1678783" target="_blank">1678783</a> : If the current opened file had no save name yet, only the path was written to the settings file, causing an error on next start.</li>
                </ul>
                
 				<h4>2007-02-25 Version 0.4.2 Beta</h4>
                <ul>
                    <li>[add] Support for the indenter CSSTidy has been added.</li>
                    <li>[add] Support for new language: Taiwan (Chinese). Thanks to Nelson Tai!</li>
                    <li>[change] For easier translation the about dialog has been redesigned a bit.</li>
                </ul>
                
 				<h4>2007-02-11 Version 0.4.1 Beta</h4>
                <ul>
                    <li>[add] Support for the indenter Uncrustify has been added.</li>
                    <li>[add] Feature Request ID <a href="issue/1651719" target="_blank">1651719</a> : A binary release for Linux AMD64 is now available.</li>
                    <li>[add] Feature Request ID <a href="issue/1657251" target="_blank">1657251</a> : If only a win32 executable exists under Linux, wine will be used to call it. Now a test will be mad if wine is installed and an error message will show up if not installed.</li>
                    <li>[change] Some changes mad to the Artistic Styler ini file as suggested by its author Jim Pattee.</li>
                    <li>[change] Put the file open button at first position in the toolbar, because normally this is the first step one would do, if he tries to find the optimal indenter and settings for himself.</li>
                    <li>[change] The background in the about dialog has been changed to look equal on all systems.</li>
                    <li>[change] The error message dialogs had small optical enhancements to be more readable.</li>
                    <li>[fix] Bug ID <a href="issue/1657255" target="_blank">1657255</a> : If an error occurred during the try to call the indenter, a message box is being shown, but the mouse cursor still indicated that the application is being busy.</li>
                    <li>[fix] Bug ID <a href="issue/1651071" target="_blank">1651071</a> : Changing the file encoding did not affect the code sent to the indenter, so the code while preview turned on, had wrong encoding (still seems to have some encoding problems under Linux).</li>
                    <li>[fix] Bug ID <a href="issue/1650222" target="_blank">1650222</a> : On Windows systems there were two DLLs missing to run GNU Indent ("libiconv-2.dll" and "libintl-2.dll")</li>
                    <li>[fix] Bug ID <a href="issue/1649918" target="_blank">1649918</a> : The indenter was not called after first program start, so preview updated first after changing a setting or the code.</li>
                </ul>	
                
				<h4>2007-02-01 Version 0.4 Beta</h4>
                <ul>
                    <li>[add] All parameters of GNU Indent have been added.</li>
                    <li>[add] From now on the indenter BCPP is included.</li>
                    <li>[add] The loaded source file encoding is now convertable by the menu entry "Reopen File with other Encoding". This way chinese encoding etc. is possible to load. (Suggest for this submitted by Nelson, Bug ID 1643541)</li>
                    <li>[add] A subdirectory "doc" has been added, containing the file "iniFileFormat.html" with info how to write ini files for new indenters.</li>
                    <li>[change] Updated the astyle executable and the ini file to version 1.20.1</li>
                    <li>[change] Restructured the gui design a little bit. Using a moveable dock widget for the indenter settings now. Using a toolbar that can also be placed at the bottom.</li>
                    <li>[change] Made it possible to change the language without the need to restart the application.</li>
                    <li>[change] Line numbers are now geyed out, because this is more decent and selecting the numbers is no longer possible.</li>
                    <li>[change] The link to the homepage in the about dialog now really opens the website.</li>
                    <li>[change] The linux version is now statically linked against Qt so there should be no problems with wrong Qt versions installed or even no Qt libraries installed.</li>
                    <li>[fix] Bug ID <a href="issue/1553601" target="_blank">1553601</a> : If no uigui ini file was found the application stopped with an exception.</li>
                </ul>		
                
				<h4>2006-08-11 Version 0.3.1 Beta</h4>
                <ul>
					<li>[add] It is now possible to select the program language. English and german available, others on request.</li>
                </ul>
                				
				<h4>2006-08-11 Version 0.3 Beta</h4>
                <ul>
					<li>[add] A message box will show up at opening a source file or program exit, if the source code has been changed, and ask whether to save the file or not.</li>
					<li>[add] The symbol "*" will be shown in the window title in front of the source file name if the source code has been changed.</li>
					<li>[add] By a settings menu entry the tool tips for the indenter parameter settings can be en/disabled because they can be annoying.</li>
					<li>[add] A settings file is from now on used to remember the last selected indenter and last opened source code file.</li>
					<li>[add] The source code can now be exported as a PDF or HTML document.</li>
					<li>[fix] Some commands are now faster available by keyboard shortcuts.</li>
					<li>[fix] The calling name in the indenter ini file for GreatCode was written lower case while the real file name was upper case, which resulted in a program calling error.</li>
					<li>[fix] Corrected the tab order for the gui elements.</li>
					<li>[change] Optimized speed when switching between the available indenters. If the indenters ini file is large, this still takes some time.</li>
                </ul>
                
				<h4>2006-08-11 Version 0.2.5 Alpha</h4>
                <ul>
					<li>[add] when loading or saving either a source code or config file the load/save dialog will open in the files directory and not allways in the UniversalIndentGUI dir</li>
					<li>[add] currently made settings for the indenter can be saved to any file</li>
					<li>[add] the menue entries for loading and saving the indenter config file have now icons</li>
					<li>[add] the currently opened source code file name and path will be shown in the window title bar</li>
					<li>[add] a error dialog that will be shown once if the indenter executable for the selected indenter does not exist</li>
					<li>[fix] turning syntax highlight on/off did first take effect after toggle of preview</li>
					<li>[fix] the image in the about dialog box was not allways visible</li>
                </ul>
				
				<h4>2006-08-03 Version 0.2.4 Alpha</h4>
                <ul>
					<li>the windows version does no longer need any dlls due to static linking and removing bind to mingwm10.dll</li>
					<li>the windows version of the GNU Indent is replaced by a DJGPP compiled GNU Indent version, that needs no further DLLs</li>
					<li>added possibility to save the shown/indented source code</li>
					<li>fixed the file extension used to call the selected indenter. It is now equal to the loaded source file extension so the indenter recognizes the source code language</li>
					<li>fixed unnecessary calls of the indenter</li>
					<li>fixed indenter was not called if preview was activated and indenter changed</li>
					<li>fixed under linux the file mask for the open source file dialog did not work correctly</li>
                </ul>
				
                <h4>2006-07-16 Version 0.2.3 Alpha</h4>
                <ul>
                    <li>added possibility to edit the source code while preview is active and see how it is formatted. 
                    So that is real live ;-)</li>
                </ul>				
                
                <h4>2006-07-14 Version 0.2.2 Alpha</h4>
                <ul>
					<li>added ini file for GNU Indent with some first settings (if any one wants to write more in it let me know)</li>
					<li>added a menu bar for more pleasent and common use of the program (some not yet used menu items are disabled)</li>
					<li>added an about dialog</li>
					<li>added some icons for better look and feel (can you feel it ;-) )</li>
					<li>fixed a bug under linux which caused an endless loop if text was edited</li>
                </ul>
				
				<h4>2006-07-07 Version 0.2.1.1 Alpha (win32 only)</h4>
                <ul>
                    <li>Previously compiled using Visual Studio 2005 Express, whereby an additional installation of a 
                    redistributable package was needed to run UniversalIndentGUI. To avoid this and not force the user 
                    to install anything, now using mingw to compile.</li>
                </ul>
				
				<h4>2006-07-04 Version 0.2.1 Alpha</h4>
                <ul>
                    <li>added function to load config file (not the uigui ini file) of every indenter (and reenabled load config file button)</li>
                    <li>added use of a default value when loading a indenter config file and not all parameters are set.</li>
                    <li>added all documented default parameters of GreatCode and Astyle to their ini files.</li>
                    <li>fixed loading of file mask for source code file after changing indenter</li>
                    <li>fixed call of indenter failed under win32 if a linux binary exists.</li>
                </ul>

                <h4>2006-06-30 Version 0.2.0 Alpha</h4>
                <ul>
                    <li>added missing parameters for flexible indenter ini file format</li>
                    <li>added complete Astyle ini file</li>
                    <li>added checkbox in front of numeric and string parameters to enable/disable them. If disabled indenters default value is used</li>
                    <li>added multiple choice combobox for parameters</li>
                </ul>
                
                <h4>2006-06-30 Version 0.1.3</h4>
                <ul>
                    <li>added working ini file for Astyle (right now only few boolean parameters)</li>
                    <li>changed ini file format for booleans to reach the planned flexibility</li>
                </ul>
                
                <h4>2006-06-23 Version 0.1.2</h4>
                <ul>
                    <li>added combobox in the upper left showing available indenters (found by searching for 
                    uigui_*.ini file). at the moment only GreatCode and testwise Astyle</li>
                    <li>added more detailed info if the indenter call results in an error</li>
                    <li>added icon for executable</li>
                    <li>changed style/format of the ini file. Still not all planned features implemented</li>
                    <li>some bugfixes, as you can say this in a preAlpha</li>
                </ul>
                <h4>2006-06-07 Version 0.1.1</h4>
                <ul>
                    <li>unnecessary gui menu removed</li>
                    <li>corrected ini file for great code (only in executable release)</li>
                    <li>made call of windows version of GreatCode under Linux possible by using wine</li>
                </ul>
                <h4>2006-06-02 Version 0.1.0</h4>
                <ul>
                    <li>First release of UniversalIndentGUI</li>
                </ul>
			</div>

<?php
include("sidebar_footer.html");
?>

		</div>
	</div>
<script type="text/javascript">
var gaJsHost = (("https:" == document.location.protocol) ? "https://ssl." : "http://www.");
document.write(unescape("%3Cscript src='" + gaJsHost + "google-analytics.com/ga.js' type='text/javascript'%3E%3C/script%3E"));
</script>
<script type="text/javascript">
var pageTracker = _gat._getTracker("UA-3962697-1");
pageTracker._initData();
pageTracker._trackPageview();
</script>
</body>
</html>
