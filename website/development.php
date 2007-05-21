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
					<li><a style="padding: 15px 0px 6px 0px;" href="features.php">Features / <br>Changelog</a></li>
					<li><a href="screenshots.php">Screenshots</a></li>
					<li id="current"><a href="development.php">Development</a></li>
				</ul>
			</div>
		</div>

		<div id="main">

			<div id="content">
				<h1>
                    Milestones and progress <br>
                    <div style="font-size:0.5em;">(Progress bars not correctly displayed with IE)</div>
                </h1>
				<p>
                    Plans for the further development of UniversalIndentGUI will be shown here.
                    Each milestone presents its approximate progress state. Since this is a free time
                    project, there will be no explicit dates for each milestones targeted ending. As common
                    for most software projects: its done when its done ;-)
                </p><br>
				
				<h2>Using QScintilla for displaying source code</h2>
                <p>
                    Currently the source code is shown inside of a QTextEdit. But to have more possibilities
                    to integrate new syntax highlightning and other helpful features concerning the code,
                    I have decided to use
                    <a class="external" href="http://www.riverbankcomputing.co.uk/qscintilla/" target="_blank">QScintilla</a>.
                    This is a wrapper for Qt around the well known
                    <a class="external" href="http://www.scintilla.org/" target="_blank">Scintilla</a>
                    text edit. The Qt4 version of QScintilla I am using is a snapshot release and
                    also released under the GPL 2.
                    <br>
                    <h4>Features that will come with QScintilla</h4>
                    <ul>
                        <li>Syntax highlightning for many languages.</li>
                        <li>Code folding.</li>
                        <li>Auto completion</li>
                        <li>...</li>
                    </ul>
                </p>
                <span>Progress:</span>
                <div id="progressframe">
                    <div id="progress" class="p95">&nbsp;95%</div>
                </div>
                <p>20.03.2007: The branch using QScintilla has been merged into the main trunk. This means, up from the
                release (version 0.5.1 Beta) QScintilla will allways be the editing component.</p>
                <br><br>
                
                <h2>Refactoring the archtitecture of UniversalIndentGUI</h2>
                <p>
                    The composition of each UniversalIndentGUI component has to be reconsidered.
                    The current design has some inflexibilities, most concerning the expandability of
                    the ini file for the indenters parameters.
                    <br>The new architecture will be shown here.
                </p>
                <span>Progress:</span>
                <div id="progressframe">
                    <div id="progress" class="p5">&nbsp;5%</div>
                </div>
                <br><br>

                <h2>Using a diff view to show differences between indented-nonindented code</h2>
                <p>
                    The preview feature is not allways enough to see all by the indenter made changes in the source code.
                    A split window view with not yet indented code on the left and indented code on the right showing
                    the changes somehow highlighted could help.
                </p>
                <span>Progress:</span>
                <div id="progressframe">
                    <div id="progress" class="p0">&nbsp;0%</div>
                </div>
                <br><br>

                <h2>A userinterface for creating and editing ini files</h2>
                <p>
                    Doing all configuration and even creation of a new ini file for an indenter
                    with a text editor is not very comfortable. Also it is rather error-prone. Users
                    do not want to cope with something like that. So there has to be a gui
                    (a gui inside of a gui ;-) ).
                </p>
                <span>Progress:</span>
                <div id="progressframe">
                    <div id="progress" class="p0">&nbsp;0%</div>
                </div>
                

			</div>

<?php
include("sidebar_footer.html");
?>
			
		</div>
	</div>	
</body>
</html>
