<html>
<head>

<title>Diesel's Data Logger</title>
</head>
<body>
<H1>Diesel's Data Logger</h1>

<?php 

ob_start(); // begin collecting output
include 'pid.php';
$pid = ob_get_clean(); 

if (file_exists("killpid.php")) {
	?><p><a href="killpid.php">STOP process <?php echo $pid;?></a></p><?php 
} else { 
?>
	<p><a href="startprocess.php">START process</a></p>
	<p><a href="killpid.php">STOP process (force)</a></p></p><pre>

<?php 

}
$output=null;
$retval=null;
$cmd = "top -b -n1 -p " . $pid;

exec($cmd, $output, $retval);
print_r($output[7]);
print_r($retval);
?>
</pre><p><a href="resetdata.php">Reset Log</a></php>
</body>
</html>