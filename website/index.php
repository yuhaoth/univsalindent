<?php
include("htmlhead.html");
?>

<body>
	<div id="page">
		<div id="header">
			<div id="tabs">
				<ul>
					<li id="current"><a href="index.php">Home</a></li>
					<li><a style="padding: 15px 0px 6px 0px;" href="features.php">Features / <br />Changelog</a></li>
					<li><a href="screenshots.php">Screenshots</a></li>
					<li><a href="development.php">Development</a></li>
				</ul>
			</div>
		</div>

		<div id="main">
			<div id="content">
    			<p style="margin-left:5pt; font-size:xx-small; color:grey;">
                    Ever concerned about how your code looks like?<br />
                    &nbsp;Ever heard of different indenting styles, for example K&amp;R?<br />
                    &nbsp;&nbsp;&nbsp;Ever received code from someone else who didn't care about code formatting?<br />
                    &nbsp;&nbsp;&nbsp;&nbsp;Ever tried to configure a code indenter to convert such code to your coding style?<br />
                    &nbsp;&nbsp;&nbsp;&nbsp;&nbsp;Ever got bored by that tedious &quot;changing a parameter&quot;-&quot;call the indeter&quot;-&quot;try and error&quot; procedure?<br />
                    <br />Help is close to you.
                </p>
				<h1>UniversalIndentGUI</h1>
				<p>
                    ... offers a live preview for setting the parameters of nearly any indenter.
                    You change the value of a parameter and directly see how your reformatted code will look like. Save your beauty looking
                    code or create an anywhere usable batch/shell script to reformat whole directories or just one file even out of the
                    editor of your choice that supports external tool calls.<br /><br />

                    Many free available code beautifier, formatter and indenter are currently supported, like GNU Indent, Uncrustify,
                    Artistic Styler, PHP Stylist, Ruby Beautify, HTML Tidy and many other (look at <a href="features.php">features</a> for complete list).
                    Currently not supported indenters can be easily added by creating a configuration file for them.<br /><br />

                    Thus UniversalIndentGUI is open for nearly any new indenter and programming language. Give it a try.
                    Perhaps you'll also find an indenter for your programming language you even didn't know that it exists.
                </p>
				<br />
				<h2>News</h2>

				<h3>2009-02-05 UniversalIndentGUI Version 1.0.2 released</h3>
				<p>Fixed a security issue that could add a vulnerability to the used temp directory on Unix based systems.
				As nearly in every release a new indenter found its way in. From now on Fortran is supported by using the
				indenter <a class="external" href="ftp://ftp.ifremer.fr/ifremer/ditigo/fortran90/">F90ppr</a>.<br />
				Seems as if there are more people interested in a Notepad++ integration than I had thought. And even worse
				the plugin package was broken. Thats fixed now and the plugin was slightly improved. Feedback is welcome.<br />
				Starting from this release UiGUI for Linux will be distributed as Debian packages and no longer static
				compiled. If someone likes to have the static version, please let me know.
                </p>
                
				<h3>2008-10-20 UniversalIndentGUI Version 1.0.1 released</h3>
				<p>UniversalIndentGUI has grown up over time and includes already many features that makes
                in useful in a productive way. Thats why I decided to completely leave the beta state and
                also indicate this by a version number jump. So here is the first public release
                of the 1.x.x version. Say welcome to it!<br />
                Since the version 1.0.0 was only released for the Qt Centre programming contest, the publicity
                shall have some advantages of that: Added support for three more indenters and of course fixed
                some bugs.<br />
                And there is a novelty: UniversalIndetGUI as a plug in for Notepad++! But it is in an alpha
                state and still has some bugs. Let me know if you like it in general.
                </p>
				
				<h3>2008-10-01 UniversalIndentGUI applied for the <a class="external" href="http://www.qtcentre.org/contest-">Qt Centre programming contest</a></h3>
				<p>Last year I missed the by Qt Centre organized Qt Programming contest. But they started another
                contest for 2008 and this time I applied UiGUI for it in the category Automation.
                The submission closed on 2008-09-30 and now I am awaiting the jury's results being announced
                at the <a class="external" href="http://fscons.org/">FSCONS'08</a> in G&ouml;teborg
                on friday 2008-10-24.<br />
                That far I really have and want to say thank you to everyone who helped me advancing UiGUI!
                And please keep your fingers crossed, so that UiGUI may get a good placing at the contest.</p>
				
				<h3>2008-05-26 UniversalIndentGUI Version 0.8.2 released</h3>
				<p>As so often a new release and some new supported indenters. These are 
				<a class="external" href="http://packages.debian.org/de/lenny/hindent">hindent</a>, 
				<a class="external" href="http://www.digital-mines.com/htb/">HTB</a> and 
				<a class="external" href="http://sourceforge.net/projects/phpstylist/">phpstylist</a>. 
				Also perltidy is now supported completely.<br />
				The created shell scripts were improved to handle complete directories. Via command line a startup
				file can be set and Drag'n Drop has been implemented.<br />
				For those of you who do not know which indenter to choose for what programming language, 
				the supported programming language is displayed behind each available indenter in the list.
				Hope you enjoy the new version and it was worth to wait about two month for it!</p>
                
                <h3>2008-03-27 UniversalIndentGUI Version 0.8.1 released</h3>
				<p>A new version, a new supported indenter, that is 
                <a class="external" href="http://pear.php.net/package/PHP_Beautifier">PHP_Beautifier</a><br />
                Also Russian and Ukrainian language support has been added. Thanks to 
                <a class="external" href="http://korytskyy.lviv.ua">Oleksandr</a>.<br />
                And files are no longer saved with UTF-8 encoding by default, but with the same encoding as
                they were opened. In addition you can now "save as" with any encoding.<br />
                Many thanks go out to <a class="external" href="http://hartwork.org/">Sebastian Pipping</a>
                who created and mantains the Debian package of UiGUI. THANK YOU GUY!
                </p>
                
                <h3>2008-01-16 UniversalIndentGUI Version 0.8.0 Beta released</h3>
				<p>The new version comes with support for three more indenters, that are 
                <a class="external" href="http://code.gosu.pl/">JsDecoder</a>, 
                <a class="external" href="http://perltidy.sourceforge.net/">PerlTidy</a> and 
                <a class="external" href="http://www.bolthole.com/AWK.html">ShellIndent</a>.
                Along with support for JsDecoder comes an interface to be able to write any indenter in JavaScript
                code and use it with UiGUI, which calls it with its internal interpreter.<br />
                Another important new thing is that UiGUI now makes correct use of the predefined directories of
                multi user operating systems, where user settings are stored in the users home dir. Also UiGUI
                can run in a portable mode, where changes are only written to the local media, like an USB drive.</p>
                
                <h3>2007-11-22 <img src="images/apfel.png" style="vertical-align:middle" alt="Apple Logo"/> UniversalIndentGUI Version 0.7.1 Beta with MacOSX binary released</h3>
				<p>There are two big news coming with this version. First is that a MacOSX universal binary
                package is now available for download. It's been short tested on Leopard so more feedback
                is welcome.<br />
                At second a long needed update to the latest version of Uncrustify and Artistic Styler
                has been done. Enjoy your well looking code.</p>
                
                <h3>2007-11-11 UniversalIndentGUI Version 0.7.0 Beta released</h3>
				<p>Here comes a new version! Rather delayed but at least it is there. The time I have left
                to develop on UniversalIndentGUI is very small, so may be the releases will not come out too
                often. But the update functionality of UiGUI will inform you, if a new version is available.
                So go on and enable it, if not already done.</p>
				
				<h3>2007-08-22 Development state updated</h3>
				<p>Even the development has slown down, work is going on. The planning for refactoring
                is continuing but diagrams have to be put into a clean form before they will be
                published. I suggest that the next release will get out on the second september week.
                So Stay tuned until then.</p>
                
				<h3>2007-06-13 UniversalIndentGUI Version 0.6.1 Beta released</h3>
				<p>Some new features found its way into this release, that should be really useful.
                So there is the possibitly to integrate UniversalIndentGUI into any IDE or editor
                supporting calls of external tools.<br />
                Also support for HTML Tidy has been added and the user can be notified if a new
                update is available.</p>
				
                <h3>2007-06-03 UniversalIndentGUI Anniversary Version 0.6.0 Beta released</h3>
				<p>Yesterday one year ago the first version of UniversalIndentGui was released.
                So I would like to say thank you to anyone who has helped me with good critics,
                translation, writing an indenter or what ever. THANKS!<br />
                And here is therefore a new release. Enjoy it!</p>
                
				<h3>2007-05-30 Next release coming soon</h3>
				<p>Developing has speed up again and a new release can be expected soon. Refactoring
                the code architecture consumes more time than I thought. Good code needs time.</p>
				
				<h3>2007-04-15 Summerlike weather against development ;-)</h3>
				<p>Looking out of my window I can see a clearly blue sky and sunshine. So I look out of the
                window for only a short time and find myself lying on the grass enjoying the warm sunbeams.
                Means developings slows down, but I hope anybody can understand my decision to not
                in front of the computer.</p>
                
                <h3>2007-04-13 Bug in release 0.5.1</h3>
				<p>This version has a bug that causes a crash if a file with an unknown extension is opened.
                Have a look in the forum 
                <a href="http://sourceforge.net/forum/forum.php?thread_id=1704191&amp;forum_id=571438">here</a> 
                for details. Next release has this fixed.</p>
				
				<h3>2007-03-21 UniversalIndentGUI Version 0.5.1 Beta released</h3>
				<p>Another rounded up number: rechead revision 300. With this release some bugs
                concerning the new QScintilla edit box are fixed and a highlighter for D
                has been added. So no big steps, but keeping on going.</p>
				
				<h3>2007-03-14 UniversalIndentGUI Version 0.5.0 Beta released using QScintilla</h3>
                <p>Increasing functionality dramatic! ;-) Using QScintilla as editing component brings a lot
                of comfort with it. It has support for many programming and scripting languages and therefore
                different syntax highlightning, code folding and many more. This release of UniversalIndentGUI
                is a rather hot release, because originally I wanted to test it more before switching to the
                QScintilla-version as release version. But on the other hand I do no longer want to withhold
                it to the public. Hope you enjoy it and give feedback. The AMD64 and win32_installer releases
                of this version will delay a bit due to few time last days.<br />
                And by the way many thanks to Ben Gardner for including support for UniversalIndentGUI in
                Uncrustify (see link on the right) and his and also Nelson Tai's good feedbacks! Could need
                more guys like you.</p>
                
                <h3>2007-03-02 <img src="images/vista_logo_icon_new.png" style="vertical-align:middle" alt="Vista Logo"/>Vista tested and new development area</h3>
				<p>Since nearly every new computer comes along with Windows Vista installed, I wondered if
                UniversalIndentGUI would be executable on it. And it does. So Vista users, enjoy UIGUI!<br />
                There is a new section at this homepage named "Development", if someone not already noticed
                it. At this page will be shown, where the development in future will lead to. So if you are interested
                in that, have a look. Any suggestions are welcome.</p>
                
                <h3>2007-02-25 UniversalIndentGUI Version 0.4.2 Beta released</h3>
				<p>Well actually some more things have happened but could not be updated here because the SourceForge
                server was down. There is a package translation tools in the download area which contains tools 
                for translating UiGUI. These tools and a translation files package for each release
                version with actualized translation files are needed. A taiwan (chinese) translation is
                available since this release (thanks to Nelson Tai). Everybody can feel invited to translate
                UniversalIndentGUI into his native language.<br />
                Further support for CSSTidy has been added. Enjoy it!</p>
                
                <h3>2007-02-11 UniversalIndentGUI Version 0.4.1 Beta released</h3>
				<p>As proposed a new indenter has been added: Uncrustify! Give it a try. Also for Linux AMD64 a
                binary will be released from now on.</p>
                
                <h3>2007-02-01 After a while of quiescence: UniversalIndentGUI Version 0.4 Beta released</h3>
				<p>Well its been a bit quiet around my project for a while. Hope everyone had a good start
                into the new year.<br />
                So don't lets waste to many words: a new version is out! As always read the changelog for Details.<br />
                The next aim for UniversalIndentGUI is to add support for Uncrustify, which is promising indenter
                and also available here at SourceForge. And the feature request list has to shrink, so there
                is also some work to do.<br />
                So, let's go!</p>
				
				<h3>2006-09-06 Subversion repository restructured</h3>
				<p>Changed the structure of the subversion repository to be compliant with the quasi standard of 
                subversion repositories. If you want to check out the latest version still use the path 
                https://svn.sourceforge.net/svnroot/universalindent and then not the whole directory but only the 
                trunk dir.</p>
				
				<h3>2006-09-04 UniversalIndentGUI Version 0.3.1 Beta released</h3>
				<p>New weekly release with some latency. Missed the pre weekend release but however here
                it is: Version 0.3.1 beta. The only new thing is a program language selector. So if you
                are interested in translating the user interface or just want it in your language,
                leave me note.</p>
                				
				<h3>2006-08-25 UniversalIndentGUI Version 0.3 <b>Beta</b> released</h3>
				<p>So far, so good. The project has reached beta state and so most requested features
                are implemented. As allways the changelog shows you all the new things. If anyone finds 
                bugs or annoying, curious behavior let me now to be able to fix it and get forward to
                a stable release. Thanks to all people that have shown interest in the project.</p>
                
				<h3>2006-08-22 Source code documentation online</h3>
				<p>The link on the right leads to the source code documentation generated with doxygen. This will help any interested developers to understand the code.</p>

				<h3>2006-08-11 UniversalIndentGUI Version 0.2.5 Alpha released (beta in sight)</h3>
				<p>The weekend comes, a new release comes. With fixing some bugs and adding some
                of the missing features the state of UniversalIndentGUI is progressing to become
                beta soon. There are still feature requests that might be implemented until the beta version
                will be released. Please report any found errors to me.</p>
				
				<h3>2006-08-03 UniversalIndentGUI Version 0.2.4 Alpha released</h3>
				<p>With this release the revision stepped beyond 100, so lets celebrate it.<br />
                Some new features are added in this release and some bugs were fixed. For the windows version all dlls
                have become unnecessary, so the package is a bit smaller than before. Please have a look at the 
                changelog for details.</p>
				
				<h3>2006-07-24 Reports and suggestions needed</h3>
				<p>Last week there was no time to continue developing UniversalIndentGUI. Now some steps ahead are 
                needed. So I would be very thankful for any suggestions to make the project reach beta state. This 
                means fix bugs, make it more usable by menu cleanup or more logical stepwise usage. Please use the bug 
                report and feature request functions here at sourceforge for this. <br /><br />Now raise your voice!</p>
				
				<h3>2006-07-16 UniversalIndentGUI Version 0.2.3 Alpha released</h3>
				<p>Just a little new feature, but it might be helpful. Added possibility to edit the source code 
                while preview is active and see how it is formatted. So that is real live ;-)</p>
				
				<h3>2006-07-14 UniversalIndentGUI Version 0.2.2 Alpha released</h3>
				<p>This release is more an optical fresh up. Some standardized icons are used and a menu bar has been 
                added to provide the common windows usability. Also a rudimentary support for GNU Indent is now 
                present which can be extendend with the use of the ini file format docu.</p>
				
				<h3>2006-07-13 Documentation of ini file fomat online</h3>
				<p>The documentation of the ini file fomat used to describe the parameters of any indenter is online.
                This way you can write your own ini file, perhaps using an existing uigui_*.ini file as draft, and add
                your favorite indenter to use it with UniversalIndentGUI. Please let me know if you wrote an ini file
                for a new indenter or send the file to me so I can add it to the next release.<br />
                If you have any trouble, just ask me in the forum.</p>
				
				<h3>2006-07-07 Version 0.2.1.1 Alpha win32only released</h3>
				<p>This version solves the previous mentioned problems compiling and deploying with Visual Studio 2005
                Express. The new win32 release is compiled with mingw-g++. If there are still problems running
                UniversalIndentGUI tell me.</p>

                <h3>2006-07-07 Problems with missing libraries/dlls under windows</h3>
				<p>There may occur problems running UniversalIndentGUI under windows. In order to run an application 
                built with Visual Studio 2005 you need some librariers, which are available by installing the 
                "Microsoft Visual C++ 2005 Redistributable Package". I did'nt know this, and Microsoft makes it not 
                very easy this way, to deploy applications. <br /> 
                The needed package for x86 is downloadable by the link:<br /> 
                <a href="http://www.microsoft.com/downloads/details.aspx?FamilyId=32BC1BEE-A3F9-4C13-9C99-220B62A191EE&amp;displaylang=en">MS Visual C++ 2005 Redistributable Package x86</a>
                <br />Perhaps I will compile future versions using mingw gcc to get rid of this problem and not force 
                users to install anything.<br />
                If there are any other problems please let me know, either post in forum or by mail. As fast as 
                possible I will try to fix any bugs.</p>

                <h3>2006-07-04 Version 0.2.1 Alpha released</h3>
				<p>Another little step forward. Have a look at the changelog to see whats new and what has changed.</p>
                
                <h3>2006-06-30 Heureka! Version 0.2.0 Alpha released</h3>
				<p>Ole, ole! Germany wins the match against Argentina and here comes the promised Alpha-version.
                So it would be time for some alpha testing and comments in the forum or buglist. Soon I will
                put a documentation of the ini-file-format online, so anyone can write one for his
                favorite indenter. Or tell me and I will write it, it its not too much work.</p>
                
                <h3>2006-06-30 Version 0.1.3 released</h3>
				<p>Small steps brings you forward... Have a look at the changelog to see whats new. Then download it
                and leave me a note if you want to. I like to read!! ;-)</p>
                
				<h3>2006-06-23 Version 0.1.2 released but still preAlpha </h3>
				<p>Ok I am a liar. This is the next release and it is still preAlpha instead of alpha. It is due to lack
                of time. There are no big feature changes but the technic (format of the ini file) is developing.
                But I wanted to release at least these minor changes to the publicity. Try it!</p>
				
				<h3>2006-06-19 Next Release will be alpha state</h3>
				<p>The planning for a flexible, universal ini file for each indenter is mostly done. The implementation 
                will follow and along with it an ini file for AStyle. With this step the project state will change from 
                preAlpha to Alpha. Release might come this week. I will be happy about any post in the forum!</p>
            
				<h3>2006-06-13 WM and so on</h3>
				<p>The WM here in germany causes some latencies with releases ;-) Means that I am watching more
                soccer than do some programming. But there will be releases, I promise. Hope you enjoy the WM too.</p>
				
				<h3>2006-06-07 PreAlpha release 0.1.1 (Update 2006-06-08)</h3>
				<p>(Update: Had some trouble with the upload so that files were not accessible. Fixed that, so download
				should work now. Enjoy it! ) <br />
                A new version has been released for Windows, Linux and as source code. It is still in an very early state
                but should work to generate a reusable GreatCode config file. See Docs or readme.txt in release.</p>

				<h3>2006-06-05 Website is online</h3>
				<p>The website went online with short info about the UniversalIndentGUI</p>

				<h3>2006-06-02 First release of UniversalIndentGUI</h3>
				<p>A first version of the UniversalIndentGUI for win32 has been released. Give it a try and if you want, useful comments. Remember this is a very, very pre release and will undergo many changes ;-)</p>

				<h3>2006-05-19 Startup of UniversalIndentGUI</h3>
				<p>Soon here will be more activity and also a first version and documentation of my tool. From now on it supports only the indenter GreatCode, which I have only compiled for Windows. So there has to be done some work to make all more flexible for more indenters and beautifiers. Stay tuned for upcoming code, news and so on...</p>
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
