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
                    <li><a style="padding: 15px 0px 6px 0px;" href="features.php">Features / <br />Changelog</a></li>
                    <li><a href="screenshots.php">Screenshots</a></li>
                    <li id="current"><a href="development.php">Development</a></li>
                </ul>
            </div>
        </div>

        <div id="main">

            <div id="content">
                <div style="font-family: Tahoma,serif; font-size: x-large; font-weight: bold;">Milestones and progress</div>
                <p>
                    The development organization has moved to Trac. Thus the information on this page here
                    is kinda outdated. Please visit the projects
                    <a href="http://sourceforge.net/apps/trac/universalindent/roadmap">roadmap</a>
                    on Trac to stay informed about the development progress.
                    
                <div style="font-family: Tahoma,serif; font-size: large; font-weight: bold;">Older milestones and progress (somehow deprecated)</div>
                <div style="font-family: Tahoma,serif; font-size:1em; font-weight: bold;">(Progress bars not correctly displayed with IE &lt;7)</div>

                <p>
                    Plans for the further development of UniversalIndentGUI will be shown here.
                    Each milestone presents its approximate progress state. Since this is a free time
                    project, there will be no explicit dates for each milestones targeted ending. As common
                    for most software projects: its done when its done ;-)
                </p><br />

                <h2>Using QScintilla for displaying source code</h2>
                <p>
                    Currently the source code is shown inside of a QTextEdit. But to have more possibilities
                    to integrate new syntax highlightning and other helpful features concerning the code,
                    I have decided to use
                    <a class="external" href="http://www.riverbankcomputing.co.uk/qscintilla/">QScintilla</a>.
                    This is a wrapper for Qt around the well known
                    <a class="external" href="http://www.scintilla.org/">Scintilla</a>
                    text edit. The Qt4 version of QScintilla I am using is a snapshot release and
                    also released under the GPL 2.
                    <br />
                </p>
                <h4>Features that will come with QScintilla</h4>
                <ul>
                    <li>Syntax highlightning for many languages.</li>
                    <li>Code folding.</li>
                    <li>Auto completion</li>
                    <li>...</li>
                </ul>

                <span>Progress:</span>
                <div id="progressframeQScintilla" class="progressframe">
                    <div id="progressQScintilla" class="progress p100">&nbsp;100%</div>
                </div>
                <p>20.03.2007: The branch using QScintilla has been merged into the main trunk. This means, up from the
                next release (version 0.5.1 Beta) QScintilla will allways be the editing component.</p>
                <br /><br />

                <h2>Refactoring the architecture of UniversalIndentGUI</h2>
                <p>
                    The composition of each UniversalIndentGUI component has to be reconsidered.
                    The current design has some inflexibilities, most concerning the expandability of
                    the ini file for the indenters parameters.
                    <br />The new architecture will be shown here.
                </p>
                <span>Progress:</span>
                <div id="progressframeRefactoring" class="progressframe">
                    <div id="progressRefactoring" class="progress p20">&nbsp;20%</div>
                </div>
                <p>20.05.2007: Plans are progressing and some refactoring is already done. Concrete
                the settings class, along with the coming settings dialog, is nearly finished.</p>
                <p>13.06.2007: As part of the refactoring the settings class and dialog are
                completly implemented.</p>
                <p>21.08.2007: Here an architecture overview as FMC diagram<br />
                <img src="images/fmc_overview.png" alt="Architecture FMC Overview" width="439"/><br />
                Its just a rough impression and details about interfaces and so on have to be put
                into a clean UML diagram. Will come soon.</p>
                <br /><br />

                <h2>Using a diff view to show differences between indented-nonindented code</h2>
                <p>
                    The preview feature is not allways enough to see all by the indenter made changes in the source code.
                    A split window view with not yet indented code on the left and indented code on the right showing
                    the changes somehow highlighted could help.
                </p>
                <span>Progress:</span>
                <div id="progressframeDiff" class="progressframe">
                    <div id="progressDiff" class="progress p0">&nbsp;0%</div>
                </div>
                <br /><br />

                <h2>A userinterface for creating and editing ini files</h2>
                <p>
                    Doing all configuration and even creation of a new ini file for an indenter
                    with a text editor is not very comfortable. Also it is rather error-prone. Users
                    do not want to cope with something like that. So there has to be a gui
                    (a gui inside of a gui ;-) ).
                </p>
                <span>Progress:</span>
                <div id="progressframeIniGui" class="progressframe">
                    <div id="progressIniGui" class="progress p0">&nbsp;0%</div>
                </div>


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
