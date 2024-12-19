<html>
<head>
<meta http-equiv='refresh' content='1'/>
<title>Diesel's Data Logger</title>
</head>
<body>
<H1>Diesel's Data Logger</h1>

<?php if (file_exists("killpid.php")) {
	?><p><a href="killpid.php">STOP process <?php include "pid.php";?></a></p><?php 
} else { 
	?><p><a href="startprocess.php" target="_blank">START process</a></p><p><a href="killpid.php">STOP process (force)</a></p></p><?php 
}
?>
<div id="startstop">
</div>
<p><a href="resetdata.php">Reset Log</a></php>
</body>
</html>