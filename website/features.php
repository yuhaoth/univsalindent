<!DOCTYPE html PUBLIC "-//W3C//DTD XHTML 1.0 Transitional//EN"
"http://www.w3.org/TR/xhtml1/DTD/xhtml1-transitional.dtd">
<html>
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
					<li id="current"><a style="padding: 18px 0px 35px 0px;" href="features.php">Features / <br>Changelog</a></li>
					<li><a href="screenshots.php">Screenshots</a></li>
					<li><a href="development.php">Development</a></li>
				</ul>
			</div>
		</div>

		<div id="main">

			<div id="content">
				<h2>Features</h2>
				<ul>
					<li>Configurable ini file, open for many indenters. Not yet known indenters can be added by creating a new ini file for them</li>
					<li>Supported and included indenters right now are: <br>
                    <a class="external" href="http://astyle.sourceforge.net/" target="_blank">Artistic Styler</a>, 
                    <a class="external" href="http://invisible-island.net/bcpp/" target="_blank">BCPP</a>, 
                    <a class="external" href="http://csstidy.sourceforge.net/" target="_blank">CSSTidy</a>, 
                    <a class="external" href="http://www.gnu.org/software/indent/" target="_blank">GNU Indent</a>, 
                    <a class="external" href="http://sourceforge.net/projects/gcgreatcode" target="_blank">GreatCode</a>, 
                    <a class="external" href="http://www.waterproof.fr/products/phpCodeBeautifier/" target="_blank">phpCB</a> (executable not included) and 
                    <a class="external" href="http://uncrustify.sourceforge.net/" target="_blank">Uncrustify</a>
                    </li>
					<li>Live preview. Directly see how the code is affected by a indenter parameter</li>
					<li>Load the settings of an indenter from its existing config file</li>
					<li>Save parameters as indeters config file for later use</li>
					<li>ToolTip help for each indenter parameter</li>
					<li>Multiple language support. Currently english, german and taiwan/chinese (Thanks to Nelson Tai)</li>
					<li>Source code syntax highlightning for bash, batch, cpp, csharp, css, d, diff, html, idl, java, javascript, lua, makefile, perl, pov, ini, python, ruby, sql and tex.</li>
				</ul>
				
				<br>
				<h2>Changelog</h2>
				<h4>2007-03-21 Version 0.5.1 Beta</h4>
				<ul>
                    <li>[change] The live typing and indenting feature was not adapted to QScintilla. Now works again (depending on the used indenter).</li>
                    <li>[fix] Bug ID 1678783: Files other encoded than ascii were not displayed correctly. Now allways displaying files using UTF-8 encoding.</li>
                    <li>[fix] Bug ID 1678783: The new syntax highlighter selection menu was not translated.</li>
                    <li>[fix] When file was reloaded with other encoding the file was allways set to be modified even if it wasn't.</li>
                </ul>
                
				<h4>2007-03-14 Version 0.5.0 Beta</h4>
				<ul>
                    <li>[add/change] Using QScintilla as editing component, which is by far more mighty than the before uses QTextEdit. Have a look inside the development area for details.</li>
                    <li>[add] Because of QScintilla, added syntax highlightning for bash, batch, cpp, csharp, css, diff, html, idl, java, javascript, lua, makefile, perl, pov, ini, python, ruby, sql and tex.</li>
                    <li>[add] Also added code folding for previously mentioned languages.</li>
                    <li>[add] Added support for the indenter "phpCB" phpCodeBeatufier. (Thanks to Nelson Tai) The executable is not included in the UiGui release, because I am not sure about the license of phpCB right now.</li>
                    <li>[add] The output of an indenter can now be read from stdout, by setting "stdout" for "outputFileParameter".</li>
                    <li>[add] Feature Request ID 1673549 : The order of the indenter call parameters for input file, output file and options[file] is now selectable in three states: pio, ipo, iop.</li>
                    <li>[add] Feature Request ID 1675503 : The last set encoding is being remembered and used on loading last file.</li>
                    <li>[add] Feature Request ID 1669822 : The last window position, size and maximized state is stored in the settings and restored.</li>
                    <li>[add] Feature Request ID 1673670 : Made it possible to switch between visible and invisible white space characters, like spaces and tabs.</li>
                    <li>[change] The "personal" configuration files of each indenter (like gc.cfg) are no longer included in a release, so these settings can be loaded into UiGui after updating.</li>
                    <li>[fix] Bug ID 1678783: If the current opened file had no save name yet, only the path was written to the settings file, causing an error on next start.</li>
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
                    <li>[add] Feature Request ID 1651719: A binary release for Linux AMD64 is now available.</li>
                    <li>[add] Feature Request ID 1657251: If only a win32 executable exists under Linux, wine will be used to call it. Now a test will be mad if wine is installed and an error message will show up if not installed.</li>
                    <li>[change] Some changes mad to the Artistic Styler ini file as suggested by its author Jim Pattee.</li>
                    <li>[change] Put the file open button at first position in the toolbar, because normally this is the first step one would do, if he tries to find the optimal indenter and settings for himself.</li>
                    <li>[change] The background in the about dialog has been changed to look equal on all systems.</li>
                    <li>[change] The error message dialogs had small optical enhancements to be more readable.</li>
                    <li>[fix] Bug ID 1657255: If an error occurred during the try to call the indenter, a message box is being shown, but the mouse cursor still indicated that the application is being busy.</li>
                    <li>[fix] Bug ID 1651071: Changing the file encoding did not affect the code sent to the indenter, so the code while preview turned on, had wrong encoding (still seems to have some encoding problems under Linux).</li>
                    <li>[fix] Bug ID 1650222: On Windows systems there were two DLLs missing to run GNU Indent ("libiconv-2.dll" and "libintl-2.dll")</li>
                    <li>[fix] Bug ID 1649918: The indenter was not called after first program start, so preview updated first after changing a setting or the code.</li>
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
                    <li>[fix] Bug ID 1553601: If no uigui ini file was found the application stopped with an exception.</li>
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
</body>
</html>
