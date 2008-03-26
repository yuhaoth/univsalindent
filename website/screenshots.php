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
					<li><a style="padding: 15px 0px 6px 0px;" href="features.php">Features / <br />Changelog</a></li>
					<li id="current"><a href="screenshots.php">Screenshots</a></li>
					<li><a href="development.php">Development</a></li>
				</ul>
			</div>
		</div>

		<div id="main">

			<div id="content">
				<h2>Screenshots</h2>
				<br />
				<a href="images/screenshot1.png" target="_blank">
                    <img src="images/screenshot1.jpg" alt="WinXPScreenshot1" width="500"/>
                </a>
				<p>UniversalIndentGUI under XP with preview turned on.</p>

				<br />
				<a href="images/screenshot2.png" target="_blank">
                    <img src="images/screenshot2.jpg" alt="WinXPScreenshot2" width="500"/>
                </a>
				<p>UniversalIndentGUI under XP showing up a tooltip.</p>

				<br />
				<a href="images/screenshot3.png" target="_blank">
                    <img src="images/screenshot3.jpg" alt="LinuxScreenshot3" width="500"/>
                </a>
				<p>UniversalIndentGUI under Linux showing the indenter select menu.</p>
				
				<br />
				<a href="images/screenshot4.png" target="_blank">
                    <img src="images/screenshot4.jpg" alt="LinuxScreenshot4" width="500"/>
                </a>
				<p>UniversalIndentGUI under Linux with outdocked indenter parameter menu.</p>
				
				<br />
				<a href="images/screenshot5.png" target="_blank">
                    <img src="images/screenshot5.jpg" alt="AboutDialogScreenshot5" width="500"/>
                </a>
				<p>The about dialog of UniversalIndentGUI.</p>
				
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
